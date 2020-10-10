#ifndef THREADPOOL_H_
#define THREADPOOL_H_

#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdint.h>
typedef struct task{
    void (*func)(void *);
    void* arg;
    struct task* next;
}task_t;
typedef struct threadpool{
    pthread_mutex_t lock;
    pthread_cond_t cond;
    pthread_t* threads;
    task_t* head;
    int thread_count;
    int queue_size;
    int shutdown;
    int started;
}threadpool_t;

static int threadpool_free(threadpool_t*);
static void* threadpool_work(void* arg);
int threadpool_add(threadpool_t* ,void(*func)(void*),void* arg);
int threadpool_destroy(threadpool_t*,int graceful);
threadpool_t* threadpool_init(int thread_num);

typedef enum{
    immediate_shutdown=1,
    graceful_shutdown=2
}threadpool_st_t;
#endif