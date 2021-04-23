#ifndef TIME_H_
#define TIME_H_

#include "heap.h"
#include <sys/time.h>
#include <stdlib.h>
#include "http.h"


//using handler_pq = int (*)(http_request_t*);
#define TIMEOUT_DEFAULT 500
//TODO:需要好好组织time和堆中的使用到的结构体和函数定义等信息

class time{
public:
    time();
    ~time();
public:
    int time_init();
    void handle_expire_time_ns();
    void time_n_add(http_request_t*,size_t, handler_pq);
    void time_n_del(http_request_t*);
    void time_update();
private:
    pq_t* pq;    
    size_t current_time;
};

int time_comp(void*, void*);

using time_s_t = class time;

#endif