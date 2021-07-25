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
#include "qs.h"
#include "rpc.pb.h"

using namespace process;
using namespace process::http;
using std::string;
using namespace QSmsg;

// How to set up work flow ?
// 1. spawn pre compute actor
// 2. spawn compute actors base on DAG config
// 3. spawn device actors

// How to comunicate with laplace?
// thru promise future api, dispatch with future?

void InferCallBack(const TaskMsg& taskmsg) {
  std::cout<<"finish task id "<<taskmsg.taskid()<<std::endl;
}

void CtrCallBack(const CtrMsg& ctrmsg) {
  std::cout<<"finish ctr id "<<ctrmsg.ctrid()<<std::endl;
}

int main(int argc, char** argv)
{
  const string message = "stop";
  // create a actor
  QS qs;
  spawn(&qs);
  qs.Initialize("./config.json", InferCallBack, CtrCallBack);

  string task1 = "task1";
  std::cout<<"==========SubmitTask========="<<std::endl;
  qs.SubmitTask(1, task1);

  sleep(2);
 
  qs.Stop();
  // option 2, terminate in main thread;
  //terminate(pid);

  // then wait
  wait(qs.self());
  return 0;
}

