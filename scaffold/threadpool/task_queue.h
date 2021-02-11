#ifndef TASK_QUEUE_H_
#define TASK_QUEUE_H_
#include <pthread.h>

//TODO:根据任务队列的负载调节线程池的大小

namespace TASK_QUEUE{
    //定义任务结构
    typedef struct  Task{
        void (*func)(void*);
        void* arg;
        struct Task* next;
    }task_t;
    
    class Task_queue{
    public:
        Task_queue();
        ~Task_queue();
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
}


#endif