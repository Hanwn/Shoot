#include "Channel.hpp"
#include "logger.h"
#include <sys/syscall.h>
#include <unistd.h>

const int Channel::none_event = 0;
// QUESTION:EPOLLPRI 什么时候使用?
const int Channel::read_event = EPOLLIN | EPOLLONESHOT |EPOLLPRI;
const int Channel::write_event = EPOLLOUT | EPOLLONESHOT;

Channel::Channel(EventLoop* _loop, int _fd) 
    : loop_(_loop)
    , fd(_fd) 
    , status(channelStatus::NEW)
    , event_(0)
    , revent_(0) { }

Channel::~Channel() {

}

int Channel::get_fd() {
    return fd;
}

void Channel::set_events(int op) {
    event_ = op;
}

int Channel::get_events() {
    return event_;
}

channelStatus Channel::get_status() {
    return status;
}

void Channel::set_status(channelStatus _status) {
    status = _status;
}

void Channel::remove() {
    // 将此Channel从epoll中移除
    loop_->remove_channel(this);
}

void Channel::update() {
    // LOG<<static_cast<int>(::syscall(SYS_gettid))<<"--->6";
    // 将此Channel在epoll中修改或者添加
    loop_->update_channel(this);
}

void Channel::handle_events() {
    // LOG<<"handle_events--->8";
    // QUESTION: EPOLLPRI?
    // QUESTION:为什么EPOLLHUP需要放在读和写的前面
    if((revent_&EPOLLHUP) && !(revent_&EPOLLIN)) {
        // LOG<<"^_^";
       if (close_handler_) close_handler_();
       return;
    }
    // LOG<<static_cast<int>(::syscall(SYS_gettid))<<"--->11";
    if (revent_ & (EPOLLERR)) {
        // LOG<<"handle error events";
        // LOG<<static_cast<int>(::syscall(SYS_gettid))<<"--->14";
        if (error_handler_) error_handler_();
    }
    if (revent_ & EPOLLIN) {
        // EPOLLIN 可读事件，当peer关闭连接的时候也会触发EPOLLIN事件
        // LOG<<"handle read events";
        // LOG<<static_cast<int>(::syscall(SYS_gettid))<<"--->12";
        if (read_handler_) read_handler_();
        // else // LOG<<static_cast<int>(::syscall(SYS_gettid))<<"---->12";
    }
    if (revent_ & EPOLLOUT) {
        // LOG<<"handle write events";
        // LOG<<static_cast<int>(::syscall(SYS_gettid))<<"--->13";
        if (write_handler_) write_handler_();
        // else // LOG<<static_cast<int>(::syscall(SYS_gettid))<<"---->13";
    }
}

void Channel::enable_read() {
    // LOG<<static_cast<int>(::syscall(SYS_gettid))<<"--->5";
    event_ |= read_event;
    update();
}

void Channel::enable_write() {
    event_ |= write_event;
    update();
}

void Channel::disable_read() {
    event_ &= ~(read_event);
    update();
}

void Channel::disable_write() {
    event_ &= ~(write_event);
    update();
}

void Channel::disalbel_all() {
    event_ = none_event;
    update();
}

