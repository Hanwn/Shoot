#ifndef UTILS_H_
#define UTILS_H_

#include "async_logging.h"
#include <pthread.h>

static pthread_once_t thread_run_once = PTHREAD_ONCE_INIT;
static AsyncLogging* async_log;

void once_init() {
    async_log = new AsyncLogging();
    async_log->start();
}


void output(const char* msg, int len) {
    pthread_once(&thread_run_once, once_init);
    // TODO:complement async append()
    // async_log->append();
}
#endif