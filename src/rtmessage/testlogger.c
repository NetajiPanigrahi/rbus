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
  rdk_logger_init("/etc/debug.ini");
  rtLog_SetOption(RT_USE_RDKLOGGER);
  rtLogSetLogHandler(NULL);
  while(1) {
      sleep(10);
      rtLog_Error("TEST Message...%ld: %ld", (long)getpid(), (long)getppid());
  }
}
