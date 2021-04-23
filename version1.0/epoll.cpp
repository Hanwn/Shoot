#include "epoll.h"


Epoll::Epoll() {
    this->epoll_create_(0);
}

Epoll::~Epoll() {
    delete this->events;
}

int Epoll::epoll_create_(int flags) {
    this->epoll_fd = epoll_create1(flags);
    if (this->epoll_fd == -1) {
        return -1;
    }
    this->events = new struct epoll_event[MAXEVENTS];    
}

void* Epoll::get_events(int index) {
    return this->events + index;
}

int Epoll::epoll_event_add(int fd, http_request_t* request, int events) {
    struct epoll_event event;
    event.data.ptr = (void*)request;
    event.events = events;
    if (epoll_ctl(this->epoll_fd, EPOLL_CTL_ADD, fd, &event) == -1) {
        return -1;
    }
}

int Epoll::epoll_event_mod(int fd, http_request_t* request, int events) {
    struct epoll_event event;
    event.data.ptr = (void*)request;
    event.events = events;
    if (epoll_ctl(this->epoll_fd, EPOLL_CTL_MOD, fd, &event) == -1) {
        return -1;
    }

}

//TODO:delete http_request_t* parameter
int Epoll::epoll_event_del(int fd, http_request_t* request, int events) {
    struct epoll_event event;
    event.data.ptr = (void*)request;
    event.events = events;
    if (epoll_ctl(this->epoll_fd, EPOLL_CTL_DEL, fd, &event) == -1) {
        return -1;
    }
}

int Epoll::epoll_event_wait(int max_events, int timeout) {
    int ret_count = epoll_wait(this->epoll_fd, this->events, max_events, timeout);
    return ret_count;
}