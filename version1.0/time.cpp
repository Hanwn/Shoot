#include "time.h"
#include <iostream>
time::time() {
    this->time_init();
}

time::~time() {
    delete this->pq;
}

int time::time_init() {
    this->pq = new pq_t();
    this->time_update();
}

void time::handle_expire_time_ns() {
    while (!this->pq->pq_is_empty()) {
        this->time_update();
        time_n_t* t_n = (time_n_t*)this->pq->pq_front();
        if (t_n->deleted) {
            //TODO:改进实现方式，不需要使用delete，可以通过堆的直接覆盖，然后从顶向下进行调整
            this->pq->pq_pop();
            continue;
        }

        if (t_n->key > this->current_time) {
            return;
        }

        if (t_n->handler) {
            t_n->handler(t_n->request);
        }
        this->pq->pq_pop();
    }
}

void time::time_n_add(http_request_t* request ,size_t timeout , handler_pq handler) {
    //request 中的timer指针和时间节点中的request是互相指向的，有点双向链表的感觉
    this->time_update();
    time_n_t* t_n = new time_n_t;
    request->timer = t_n;
    t_n->key = this->current_time + timeout;
    t_n->deleted = 0;
    t_n->handler = handler;
    t_n->request = request; 
    int rc = this->pq->pq_insert(t_n);
    //TODO:判断是否插入成功
}

void time::time_n_del(http_request_t* request) {
    this->time_update();
    time_n_t* t_n = request->timer;
    //惰性删除
    //标记为已删除，在检查队列时进行真正的删除
    t_n->deleted = 1;
}

void time::time_update() {
    struct timeval tv;
    int rc = gettimeofday(&tv, nullptr);
    if (rc == -1) {
        return;
    }
    this->current_time = ((tv.tv_sec * 1000) + (tv.tv_usec/1000));
}
