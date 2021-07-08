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
#include "compute_actor.h"

#include <process/defer.hpp>
#include <process/dispatch.hpp>
#include <process/future.hpp>
#include <process/http.hpp>
#include <process/process.hpp>

#include <stout/strings.hpp>

using namespace process;
using namespace process::http;
using std::string;

int main(int argc, char** argv)
{
  // create a actor
  ComputeActor process;
  PID<ComputeActor> pid = spawn(&process);

  TaskMsg task;
  task.task = "hello";

  // send a local msg to it
  for(int i=0; i<5; i++) {
    dispatch(pid, &ComputeActor::InferTask, task);
  }

  CmdMsg cmd;
  // control msg
  dispatch(pid, &ComputeActor::RunCmd, cmd);
  
  // How to terminate a actor? 

  // option 1, send msg to it, let it terminate itself
  dispatch(pid, &ComputeActor::Stop);
  
  // option 2, terminate in main thread;
  //terminate(pid);

  // then wait
  wait(pid);
  return 0;
}

