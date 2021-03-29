#ifndef TASK_QUEUE_H_
#define TASK_QUEUE_H_

#include <pthread.h>

typedef struct  Task{
    void (*func)(void*);
    void* arg;
    struct Task* next;
}task_t;
class task_queue_t{

public:
    task_queue_t();
    ~task_queue_t();
public:
    int add(void (*func)(void*), void*);
    int handle();
    //TODO:finish
    int destroy();
    int free();
public:
    // TODO:将所有的属性操作进行封装
    int size;
    int close;
    task_t* tasks;
    pthread_cond_t cond;
    pthread_mutex_t mut;

};

#endif