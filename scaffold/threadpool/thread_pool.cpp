#include "thread_pool.h"
#include <pthread.h>
#include "task_queue.h"

using namespace TASK_QUEUE;

namespace THREAD_POOL {
    void* Thread_pool::thread_handler(void * arg) {
        Task_queue * tq = (Task_queue*)arg;
        task_t* task;
        while (1) {
            pthread_mutex_lock(&(tq->mut));
            while (tq->size == 0 && tq->close == 0) {
                pthread_cond_wait(&(tq->cond),&(tq->mut));
            }
            if (tq->close) {
                pthread_mutex_unlock(&(tq->mut));
                break;
            }
            tq->handle();
            pthread_mutex_unlock(&(tq->mut));
        }
        pthread_exit(nullptr);
    }

    void* Thread_pool::thread_init(int thread_num, Task_queue* tq) {
        this->threads = new pthread_t[thread_num];
        for (int i = 0; i < thread_num; ++i) {
            pthread_create(this->threads + i, nullptr, Thread_pool::thread_handler, (void*)tq);
        }
    }
}