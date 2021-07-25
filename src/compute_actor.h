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

#ifndef COMPUTE_ACTOR_
#define COMPUTE_ACTOR_

#include <iostream>
#include <string>

#include <process/defer.hpp>
#include <process/dispatch.hpp>
#include <process/future.hpp>
#include <process/http.hpp>
#include <process/process.hpp>
#include <process/protobuf.hpp>

#include <stout/strings.hpp>

#include "base_adt.h"
#include "qs.h"

using namespace process;

using namespace process::http;

using namespace QSmsg;

using std::string;

class ComputeActor : public ProtobufProcess<ComputeActor>
{
public:
  ComputeActor(UPID qs): ProcessBase(ID::generate("ComputeActor")), qs_(qs) {}
  ~ComputeActor() override {}

  // async handler for inference
  void InferTask(const UPID& from, const TaskMsg& task)
  {
    std::cout<<this->self()<<": infer task "<<task.taskid()<<std::endl;
    // may need to dispatch a ack msg to other actors
    if (succs_.size() == 0) {
       send(qs_, task);
    } else {
       for(auto i : succs_) {
         send(i, task);
       }
    }
  }

  // async handler for control message
  void RunCmd(const UPID& from, const CtrMsg& cmd)
  {
    std::cout<<" cmd msg "<<cmd.ctrid()<<std::endl;
    // may need to dispatch a ack msg to other actors
  }

  void Stop(const UPID& from, const StopMsg& body)
  {
    terminate(self());
  }

  // preds
  std::vector<UPID> preds_;
  // input tensors
  std::map<UPID, std::vector<Tensor>> inputs_;
  // succs
  std::vector<UPID> succs_;
  // output tensors
  std::map<UPID, std::vector<Tensor>> outputs_;

protected:
  void initialize() override
  {
    install<StopMsg>(&ComputeActor::Stop);
    install<TaskMsg>(&ComputeActor::InferTask);
    install<CtrMsg>(&ComputeActor::RunCmd);   
  }

private:
  //Promise<int> promise;
  // QS
  UPID qs_;
  // device type
  DeviceType device_;
};

#endif
