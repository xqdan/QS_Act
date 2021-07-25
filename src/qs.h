// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License

#ifndef QS_
#define QS_

#include <iostream>
#include <string>

#include <process/defer.hpp>
#include <process/dispatch.hpp>
#include <process/future.hpp>
#include <process/http.hpp>
#include <process/process.hpp>
#include <process/protobuf.hpp>

#include <stout/json.hpp>
#include <stout/stringify.hpp>
#include <stout/strings.hpp>
#include <stout/os/read.hpp>
#include <stout/os.hpp>
#include <stout/path.hpp>
#include "compute_actor.h"
#include "base_adt.h"
#include "rpc.pb.h"

using namespace process;

using namespace process::http;

using std::string;

using namespace QSmsg;

// QS can be interface class for laplace
// QS can 
// 1, set up a DAG job
// 2, submit a infer task to job
// 3, wait infer task finish

// each QS instance hold a DAG
class QS : public ProtobufProcess<QS>
{
public:
  // param json is for seting up DAG actors
  // param task_result is for notice main thread for infer result
  QS() : ProcessBase(ID::generate("QS")) {}
  ~QS() override {}

  void Initialize(string json, const InferHook& task_result, const CtrHook& ctr_hook) {
    infercallback_ = task_result;
    ctrcallback_ = ctr_hook;
    SetUpDag(json);
  }

  // async submit a task to job, QS will call callback when this task is done
  void SubmitTask(int taskid, const string& task) {
    TaskMsg taskmsg;
    taskmsg.set_taskid(taskid);

    Tensor input;
    input.set_name(task);
    input.set_addr("0x12345");
    input.set_dtype("fp32");
    input.add_shape(128);
    input.add_shape(128);
    input.add_shape(128);
    input.add_shape(128);

    taskmsg.set_taskid(taskid);
    if(dag_.size() == 0) {
       std::cout<<"dag is not set up"<<std::endl;
    }
    UPID pid =  dag_[0]->self();
    send(pid, taskmsg);
  }

  // async handler for control message
  void RunCmd(const CtrMsg& cmd)
  {
    std::cout<<" cmd msg "<<cmd.ctrid()<<std::endl;
    // may need to dispatch a ack msg to other actors
  }

  void Stop()
  {
    ShutDownDag();
    terminate(self());
  }

  void CommitTask(const UPID& from, const TaskMsg& taskmsg)
  {
    infercallback_(taskmsg);
  }

  void CommitCtr(const UPID& from, const CtrMsg& ctrmsg) 
  {
    ctrcallback_(ctrmsg);
  }

protected:
  void initialize() override
  {
    install<TaskMsg>(&QS::CommitTask);
    install<CtrMsg>(&QS::CommitCtr);
  }

private:
  // map for DAG
  std::vector<std::shared_ptr<ComputeActor>> dag_;
  // id : node
  std::map<string, UPID> idx_actors_;
  // callback for task result
  InferHook infercallback_;
  // callback for ctr msg
  CtrHook ctrcallback_;

  // setup dag actors based on json file 
  void SetUpDag(string json) {
    // read file
    Try<string> config = os::read(json);
    if (config.isError()) {
      // Need to check if file exists AFTER we open it to guarantee
      // process hasn't terminated.
      if (!os::exists(json)) {
        std::cout<<json<<" is not exists!"<<std::endl;
        return;
      }
      std::cout<<json<<" read file error "<<std::endl;
      return;
    }
    
    Try<JSON::Object> parse = JSON::parse<JSON::Object>(config.get());
    JSON::Array dag = parse->values["dag"].as<JSON::Array>();

    std::cout<<"dag size "<<dag.values.size()<<std::endl;

    // create nodes
    for(int i = 0; i < dag.values.size(); i++) {
      auto node = std::make_shared<ComputeActor>(this->self());
      spawn(node.get());
      dag_.push_back(node);
      std::cout<<"set up dag======="<<std::endl;
      JSON::Object obj = dag.values[i].as<JSON::Object>();
      JSON::String key = obj.values["id"].as<JSON::String>();
      idx_actors_[key.value] = node->self();
    }

    Try<JSON::Value> null_preds = JSON::parse(
      "{"
      "  \"preds\" : null"
      "}");

    Try<JSON::Value> null_succs = JSON::parse(
      "{"
      "  \"succs\" : null"
      "}");

    // build dependency
    for(int i = 0; i < dag.values.size(); i++) {
      auto node = dag_[i];
      JSON::Value val = dag.values[i].as<JSON::Value>();
      JSON::Object obj = dag.values[i].as<JSON::Object>();

      JSON::Array preds;
      if (!val.contains(null_preds.get())) {
        preds = obj.values["preds"].as<JSON::Array>();
      }

      JSON::Array succs;
      if (!val.contains(null_succs.get())) {
        succs = obj.values["succs"].as<JSON::Array>();
      }

      for (int i=0; i<preds.values.size(); i++) {
        JSON::String jpred = preds.values[i].as<JSON::String>();
        if(!jpred.value.empty()) {
          UPID pred = idx_actors_[jpred.value];
          node->preds_.push_back(pred);
        }
      } // update preds

      for (int i=0; i<succs.values.size(); i++) {
        JSON::String jsucc = succs.values[i].as<JSON::String>();
        if(!jsucc.value.empty()) {
          UPID succ = idx_actors_[jsucc.value];
          node->succs_.push_back(succ);
        }
      } // update succs

    } // build dependency 
  }

  void ShutDownDag()
  {
    StopMsg stop;
    stop.set_stop(true);
    // stop
    for (auto i : dag_) {
      send(i->self(), stop);
    }
    // wait
    for (auto i : dag_) {
      wait(i->self());
    }
  }
};

#endif
