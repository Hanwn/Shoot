#include <iostream>
#include "task_queue.h"
#include "thread_pool.h"
#include <unistd.h>

using namespace TASK_QUEUE;
using namespace THREAD_POOL;
#define THREAD_NUM 4

void* handle(void * arg) {
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

static void test(void *arg) {
    std::cout<<"test is running!"<<std::endl;
}

/*
int main() {

    Task_queue* t = new Task_queue();
    pthread_t* threads = new pthread_t[THREAD_NUM];
    for (int i = 0; i < THREAD_NUM; ++i) {
        pthread_create(threads + i,nullptr, handle, (void *)t);
    }
    int u = 0;
    t->add(test, (void *)u);

    sleep(1);

    t->close = 1;
    pthread_cond_broadcast(&(t->cond));

    for (int i = 0; i < THREAD_NUM; ++i) {
        pthread_join(threads[i], nullptr);
    }

    sleep(1);
    

    delete t;
    return 0;
}
*/
int main() {


    Task_queue* tq = new Task_queue();
    Thread_pool* tp = new Thread_pool();

    tp->thread_init(4, tq);

    int u = 0;
    tq->add(test, (void *)u);

    sleep(2);
    tq->close = 1;
    pthread_cond_broadcast(&(tq->cond));
    sleep(1);
    delete tq;
    delete tp;

    return 0;
}