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

#include <iostream>
#include <string>

#include <process/defer.hpp>
#include <process/dispatch.hpp>
#include <process/future.hpp>
#include <process/http.hpp>
#include <process/process.hpp>

#include <stout/strings.hpp>

#include "base_adt.h"

using namespace process;

using namespace process::http;

using std::string;

// if in ditribued env, choose ProtobuffProcess

class ComputeActor : public Process<ComputeActor>
{
public:
  ComputeActor(): ProcessBase("ComputeActor") {}
  ~ComputeActor() override {}

  // async handler for inference
  void InferTask(const TaskMsg& task)
  {
    std::cout<<" infer task "<<task.task<<std::endl;
    string body = "... infer task ...";
    // may need to dispatch a ack msg to other actors
  } 

  // async handler for control message
  void RunCmd(const CmdMsg& cmd)
  {
    std::cout<<" cmd msg "<<cmd.ctr<<std::endl;
    // may need to dispatch a ack msg to other actors
  }

  void Stop()
  {
    terminate(self());
  }

protected:
  //void initialize() override
  //{
  //}

private:
  //Promise<int> promise;
};
