#ifndef THREAD_POOL_
#define THREAD_POOL_
#include "task_queue.h"
using namespace TASK_QUEUE;

namespace THREAD_POOL{
    class Thread_pool{
    public:
        static void* thread_handler(void *);
        void* thread_init(int thread_num, Task_queue*);
    public:
        pthread_t* threads;
    };    
}


#endif