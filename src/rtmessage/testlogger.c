#include "rtLog.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#define testLog_WarnPrint(mod,FORMAT...)   rtLogPrintf(RT_LOG_WARN, mod, __FILE__, __LINE__, FORMAT)
#define testLog_ErrorPrint(mod,FORMAT...)  rtLogPrintf(RT_LOG_ERROR, mod, __FILE__, __LINE__, FORMAT)
#define testLog_FatalPrint(mod,FORMAT...)  rtLogPrintf(RT_LOG_FATAL, mod, __FILE__, __LINE__, FORMAT)
  
#define testLog_Debug(FORMAT,...) testLog_DebugPrint("TESTLOGGER",FORMAT"\n", ##__VA_ARGS__)
#define testLog_Info(FORMAT,...)  testLog_InfoPrint("TESTLOGGER",FORMAT"\n", ##__VA_ARGS__)
#define testLog_Warn(FORMAT,...)  testLog_WarnPrint("TESTLOGGER",FORMAT"\n", ##__VA_ARGS__)
#define testLog_Error(FORMAT,...) testLog_ErrorPrint("TESTLOGGER",FORMAT"\n", ##__VA_ARGS__)
#define testLog_Fatal(FORMAT,...) testLog_FatalPrint("TESTLOGGER",FORMAT"\n", ##__VA_ARGS__)
int main(int argc, char * argv[])
{
  rdk_logger_init("/etc/debug.ini");
  rtLog_SetOption(RT_USE_RDKLOGGER);
  rtLogSetLogHandler(NULL);
  while(1) {
      sleep(10);
     testLog_Error("TEST Message...%ld: %ld", (long)getpid(), (long)getppid());
  }
}
