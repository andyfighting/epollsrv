#ifndef SRVLOG_H
#define SRVLOG_H
#include <stdarg.h>

#define LDR_LOG_PATH        "/var/log/epollsrv.log"


#define LDR_LOG_MAX_SIZE    (1024*1024*4)

void logprintf(char *fmt, ...);

#if 0
#define ldr_log logprintf
#else
#define ldr_log(format,...) \
		do{ \
			char buf[4096]; \
			snprintf(buf,4096,\
				"file:[%s]/func:[%s]/line:[%d]\n",\
				__FILE__,__func__,__LINE__); \
			printf(buf);\
			logprintf(format,##__VA_ARGS__); \
		} while(0)
#endif


void open_log(int argc, char* argv[]);

void close_log();

#endif
