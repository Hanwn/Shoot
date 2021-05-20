#include "Epoll.hpp"
#include <sys/epoll.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "logger.h"
#include <error.h>
#include <errno.h>
#include <sys/syscall.h>

const int EVENT_SUM = 4096;


Epoll::Epoll(EventLoop* _loop) 
    : epoll_fd_(epoll_create1(EPOLL_CLOEXEC))
    , event_array_(EVENT_SUM)
    , own_loop_(_loop) {
}

Epoll::~Epoll() {
    ::close(epoll_fd_);
}

void Epoll::epoll_add(Channel* _channel, int op) {
    // LOG<<static_cast<int>(::syscall(SYS_gettid))<<"--->8";
    struct epoll_event event;
    ::memset(&event, 0, sizeof event);
    event.data.ptr = _channel;
    event.events = _channel->get_events();
    int _fd = _channel->get_fd();
    // LOG<<"add listen fd "<<_fd;
    if (::epoll_ctl(epoll_fd_, op, _fd, &event) < 0) {
        // LOG<<"ADD ERROR\n";
    }
}


void Epoll::epoll_del(Channel* _channel, int op) {
    struct epoll_event event;
    ::memset(&event, 0, sizeof event);
    event.data.ptr = _channel;
    event.events = _channel->get_events();
    int _fd = _channel->get_fd();
    if (::epoll_ctl(epoll_fd_, op, _fd, &event) < 0) {
        // LOG<<"EPOLL DEL ERROR "<<errno;
    }
}


void Epoll::epoll_mod(Channel* _channel, int op) {
    struct epoll_event event;
    ::memset(&event, 0, sizeof event);
    event.data.ptr = _channel;
    event.events = _channel->get_events();
    int _fd = _channel->get_fd();
    if (::epoll_ctl(epoll_fd_, op, _fd, &event) < 0) {
        // LOG<<"MOD ERROR\n";
    }
    
}


void Epoll::poll(channel_vector& v) {
    // LOG<<"poll--->7";
    // LOG<<static_cast<int>(::syscall(SYS_gettid))<<"--->10";
    int cnt_events = ::epoll_wait(epoll_fd_, &*event_array_.begin()\
                                           , static_cast<int>(event_array_.size()), -1);
    // LOG<<"epoll_wait return";
    for (int i = 0; i < cnt_events; ++i) {
        Channel* _channel = static_cast<Channel*>(event_array_[i].data.ptr);
        // set_revents 可以获取当前fd上的事件，将这个事件保存在channel中，在调用handle_events这个回调的
        // 时候可以根据这个事件判断具体回调哪个函数
        _channel->set_revents(event_array_[i].events);
        v.push_back(_channel);
    }
    // 扩容操作：
    if (cnt_events == static_cast<int>(event_array_.size())) {
        event_array_.resize(event_array_.size()*2);
    }
}

void Epoll::handle_overtimer() {


}


