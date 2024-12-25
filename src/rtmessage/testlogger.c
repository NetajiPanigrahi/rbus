#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include "rdk_debug.h"

int main()
{
  rdk_logger_init("/etc/debug.ini");
  while(1) {
      sleep(5);
      RDK_LOG(RDK_LOG_ERROR, "LOG.RDK.TESTLOGGER","Message...%ld: %ld\n", (long)getpid(), (long)getppid());
  }
}
