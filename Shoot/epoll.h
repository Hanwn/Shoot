#ifndef EPOLL_H_
#define EPOLL_H_

#include <sys/epoll.h>
#include "utils.h"
#define MAXEVENTS 1024

class Epoll{
public:
    Epoll();
    ~Epoll();
public:
    int epoll_event_add(int, http_request_t*, int );
    int epoll_event_mod(int, http_request_t*, int );
    int epoll_event_del(int, http_request_t*, int );
    int epoll_event_wait(int, int);
    void* get_events(int);

private:
    int epoll_create_(int);

private:
    int epoll_fd;
    //TODO:开一个接口访问events
    struct epoll_event* events;
};


#endif