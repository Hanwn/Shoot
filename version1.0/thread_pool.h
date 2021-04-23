#ifndef THREAD_POOL_H_
#define THREAD_POOL_H_

#include <pthread.h>
#include "task_queue.h"
#include <iostream>

class Thread_pool{
public:
    static void* thread_handler(void *);
    void* thread_init(int thread_num, task_queue_t*);
public:
    pthread_t* threads;
};    

#endif