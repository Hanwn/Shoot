#include "task_queue.h"
#include <iostream>

namespace TASK_QUEUE{
    Task_queue::Task_queue() {
        //无参构造函数
        pthread_mutex_init(&(this->mut), nullptr);
        pthread_cond_init(&(this->cond), nullptr);
        this->tasks = new task_t;
        this->size = 0;
        this->close = 0;

    }
    Task_queue::~Task_queue() {
        //默认析构函数
        task_t* p = this->tasks;
        while(p) {
            this->tasks = p->next;
            delete p;
            p = this->tasks;
        }
        pthread_cond_destroy(&(this->cond));
        pthread_mutex_destroy(&(this->mut));
    }

    int Task_queue::add(void (*func)(void*), void* arg) {
        // 向任务队列中添加任务
        pthread_mutex_lock(&(this->mut));
        task_t* task = new task_t;
        task->func = func;
        task->arg = arg;
        task->next = this->tasks->next;
        this->tasks->next = task;
        this->size++;
        pthread_mutex_unlock(&(this->mut));
        pthread_cond_signal(&(this->cond));
    }

    int Task_queue::handle() {
        // 从任务队列中取下任务
        task_t* task = this->tasks->next;
        this->tasks->next = task->next;
        this->size--;
        (*(task->func))(task->arg);
        delete task;
    }
}
