#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>
#include "srvlog.h"


static FILE *fp_log = NULL;

void logprintf(char *fmt, ...)
{
    struct tm *tm;
    time_t t = 0;
    int cnt;
    char buffer[1024];
    if (!fp_log) return;

    t=time(&t);
    tm = localtime(&t);
    strftime(buffer, sizeof(buffer), "%D %T", tm);

    fprintf(fp_log, "%s ", buffer);

    va_list argptr;
    va_start(argptr, fmt);
    cnt = vsnprintf(buffer, sizeof(buffer), fmt, argptr);
    va_end(argptr);

    fprintf(fp_log, "%s", buffer);
    fflush(fp_log);
}

void open_log(int argc, char* argv[])
{
    struct stat st;

    if (stat(LDR_LOG_PATH, &st) == 0 && st.st_size > LDR_LOG_MAX_SIZE)
        remove(LDR_LOG_PATH);

    fp_log = fopen(LDR_LOG_PATH, "a");

    if (fp_log) {
        fclose(fp_log);
        chmod(LDR_LOG_PATH, 0666);
        fp_log = fopen(LDR_LOG_PATH, "a");
    }

    if (fp_log) {
        int i;
        fprintf(fp_log, "==================================================================\n");
        ldr_log("epollsrv start:");
        for(i = 0; i < argc; i++) {
            fprintf(fp_log, " %s", argv[i]);
        }
        fprintf(fp_log, "\n");
    }
    else
        fprintf(stderr, "epollsrv open log error %s", strerror(errno));

}

void close_log()
{
    if (fp_log) {
        fprintf(fp_log, "==================================================================\n\n");
        fclose(fp_log);
        fp_log = NULL;
    }
}
