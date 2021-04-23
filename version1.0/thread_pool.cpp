#include "thread_pool.h"
#include "task_queue.h"

void* Thread_pool::thread_handler(void * arg) {
    task_queue_t * tq = (task_queue_t*)arg;
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

void* Thread_pool::thread_init(int thread_num, task_queue_t* tq) {
    this->threads = new pthread_t[thread_num];
    for (int i = 0; i < thread_num; ++i) {
        pthread_create(this->threads + i, nullptr, Thread_pool::thread_handler, (void*)tq);
    }
}