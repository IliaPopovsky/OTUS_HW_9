#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include "logger.h"

int logger_init(const char * filename);

void* thread_body(void* arg)
{
    for(int i = 0; i < 2; ++i)
    {
        puts(arg);
        char *test_block = NULL;
        sleep(1);
        test_block = malloc(10000000000 * 10000000000);
        if (test_block == NULL){
            LOG_ERROR("%s", "malloc test_block error!");
        }
        sleep(2);
    }
    return NULL;
}

int main(void)
{
    logger_init("logfile.txt");
    LogLevel level = logger_getLevel();
    if (level < LogLevel_INFO) {
        printf("$$$$$$");
    }
    logger_setLevel(LogLevel_DEBUG);
    char *test_block_main = NULL;
    test_block_main = malloc(10000000000 * 10000000000);
    if (test_block_main == NULL){
        LOG_ERROR("%s", "malloc test_block_main error!");
    }
    pthread_t thread1, thread2;
    pthread_create(&thread1, NULL, thread_body, "Test string1");
    pthread_create(&thread2, NULL, thread_body, "Test string2");
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    printf("End test.\n");
    char *test_string = "Ffd";
    LOG_INFO("console %s logging", "test");
    int l = logger_getLevel();
    LOG_DEBUG("format example: %d%c%s", 1, '2', "3");
    LogLevel f = LogLevel_WARN;
    printf("Test print %p\n", test_string);
    printf("f: %d\n", f);
    printf("l: %d\n", l);
    return 0;
}
