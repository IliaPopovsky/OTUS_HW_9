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

static struct {
    FILE* output;
    char filename[MAX_LEN + 1];
} log_t;

