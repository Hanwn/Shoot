#include "Epoll.hpp"
#include <sys/epoll.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "logger.h"

const int EVENT_SUM = 4096;


Epoll::Epoll() 
    : epoll_fd_(epoll_create1(EPOLL_CLOEXEC))
    , event_array_(EVENT_SUM) {

}

Epoll::~Epoll() {
    close(epoll_fd_);
}

void Epoll::epoll_add(Channel* _channel, int op) {
    struct epoll_event event;
    ::memset(&event, 0, sizeof event);
    event.data.ptr = _channel;
    event.events = _channel->get_events();
    int _fd = _channel->get_fd();
    if (::epoll_ctl(epoll_fd_, op, _fd, &event) < 0) {
        LOG<<"ADD ERROR\n";
    }
}


void Epoll::epoll_del(Channel* _channel, int op) {
    struct epoll_event event;
    ::memset(&event, 0, sizeof event);
    event.data.ptr = _channel;
    event.events = _channel->get_events();
    int _fd = _channel->get_fd();
    if (::epoll_ctl(epoll_fd_, op, _fd, &event) < 0) {
        LOG<<"EPOLL DEL ERROR";
    }
}


void Epoll::epoll_mod(Channel* _channel, int op) {
    struct epoll_event event;
    ::memset(&event, 0, sizeof event);
    event.data.ptr = _channel;
    event.events = _channel->get_events();
    int _fd = _channel->get_fd();
    if (::epoll_ctl(epoll_fd_, op, _fd, &event) < 0) {
        LOG<<"MOD ERROR\n";
    }
    
}


void Epoll::poll(channel_vector& v) {

    int cnt_events = ::epoll_wait(epoll_fd_, &*event_array_.begin()\
                                           , static_cast<int>(event_array_.size()), -1);
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


