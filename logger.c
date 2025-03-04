#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>
#include <execinfo.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/syscall.h>
#include "logger.h"

#define MAX_LEN 255
#define SIZE_BUF 100

extern long int syscall (long int __sysno, ...) __THROW;
static volatile int logger_activate;
static volatile LogLevel logLevel_t = LogLevel_INFO;
static pthread_mutex_t mutex;

static struct {
    FILE* output;
    char filename[MAX_LEN + 1];
} log_t;


void logger_setLevel(LogLevel level)
{
    logLevel_t = level;
}

LogLevel logger_getLevel(void)
{
    return logLevel_t;
}

int logger_levelenough(LogLevel level)
{
    return logLevel_t <= level;
}

static char * getLevel(LogLevel level)
{
    switch (level) {
        case LogLevel_DEBUG: return "DEBUG";
        case LogLevel_INFO:  return "INFO";
        case LogLevel_WARN:  return "WARN";
        case LogLevel_ERROR: return "ERROR";
        default: return "";
    }
}

int logger_init(const char * filename){
    if (logger_activate) {
        fprintf(stderr, "logger allready initializated!");
        return 1;
    }
    if (filename == NULL) {
        assert(0 && "filename must not be NULL");
        return 0;
    }
    if (strlen(filename) > MAX_LEN) {
        assert(0 && "filename exceeds the maximum number of characters");
        return 0;
    }
    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_lock(&mutex);
    if (log_t.output != NULL) {
        fclose(log_t.output);
    }
    log_t.output = fopen(filename, "a");
    if (log_t.output == NULL) {
        fprintf(stderr, "ERROR: logger: Failed to open file: `%s`\n", filename);
        pthread_mutex_unlock(&mutex);
        return 0;
    }
    strncpy(log_t.filename, filename, sizeof(filename) + 1);

    logger_activate = 1;
    pthread_mutex_unlock(&mutex);
    return 1;
}

static void logger_stacktraceprint(FILE* fp){
    int nptrs;
    void *buffer[SIZE_BUF];
    char **strings;
    nptrs = backtrace(buffer, SIZE_BUF);
    fprintf(fp, "\tbacktrace() returned %d addresses\n", nptrs);
    strings = backtrace_symbols(buffer, nptrs);
    if (strings == NULL) {
        perror("backtrace_symbols");
        return;
    }
    for (int j = 0; j < nptrs; j++)
        fprintf(fp, "\t%s\n", strings[j]);
    free(strings);
}

static void write_log(FILE* fp, char * levelstr, int threadID,
        const char* file, int line, const char* fmt, va_list arg){
    fprintf(fp, "%s: threadId:%d %s:%d: ", levelstr, threadID, file, line);
    vfprintf(fp, fmt, arg);
    fprintf(fp, "\n");
    if (!strcmp(levelstr, "ERROR")){
        logger_stacktraceprint(fp);
    }
    return;
}

void logger_log(LogLevel level, const char* file, int line, const char* fmt, ...){

    char * levelstr;
    va_list arg;
    int threadID;

    if (!logger_activate) {
        assert(0 && "logger is not initialized");
        return;
    }

    if (!logger_levelenough(level)) {
        return;
    }
    threadID = syscall(SYS_gettid);
    pthread_mutex_lock(&mutex);
    levelstr = getLevel(level);

    va_start(arg, fmt);
    write_log(log_t.output, levelstr, threadID, file, line, fmt, arg);
    va_end(arg);

    pthread_mutex_unlock(&mutex);
}

void logger_exit(){
    pthread_mutex_destroy(&mutex);
    if (log_t.output) {
        fclose(log_t.output);
    }
}
