
/*
##########################################################################
# If not stated otherwise in this file or this component's LICENSE
# file the following copyright and licenses apply:
#
# Copyright 2016 RDK Management
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
##########################################################################
*/
#include "rtLog.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#define rtLog_WarnPrint(mod,FORMAT...)   rtLogPrintf(RT_LOG_WARN, mod, __FILE__, __LINE__, FORMAT)
#define rtLog_ErrorPrint(mod,FORMAT...)  rtLogPrintf(RT_LOG_ERROR, mod, __FILE__, __LINE__, FORMAT)
#define rtLog_FatalPrint(mod,FORMAT...)  rtLogPrintf(RT_LOG_FATAL, mod, __FILE__, __LINE__, FORMAT)
  
#define rtLog_Debug(FORMAT,...) rtLog_DebugPrint("TESTLOGGER",FORMAT"\n", ##__VA_ARGS__)
#define rtLog_Info(FORMAT,...)  rtLog_InfoPrint("TESTLOGGER",FORMAT"\n", ##__VA_ARGS__)
#define rtLog_Warn(FORMAT,...)  rtLog_WarnPrint("TESTLOGGER",FORMAT"\n", ##__VA_ARGS__)
#define rtLog_Error(FORMAT,...) rtLog_ErrorPrint("TESTLOGGER",FORMAT"\n", ##__VA_ARGS__)
#define rtLog_Fatal(FORMAT,...) rtLog_FatalPrint("TESTLOGGER",FORMAT"\n", ##__VA_ARGS__)
int main(int argc, char * argv[])
{
    #ifdef ENABLE_RDKLOGGER
    rdk_logger_init("/etc/debug.ini");
#endif

  rtLog_SetOption(RT_USE_RDKLOGGER);
  rtLogSetLogHandler(NULL);
  while(1) {
      sleep(5);
      rtLog_Error("TEST Message...%ld: %ld", (long)getpid(), (long)getppid());
  }
}
