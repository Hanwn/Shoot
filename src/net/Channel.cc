#include "Channel.hpp"
#include "logger.h"

const int Channel::none_event = 0;
// QUESTION:EPOLLPRI 什么时候使用?
const int Channel::read_event = EPOLLIN | EPOLLET;
const int Channel::write_event = EPOLLOUT | EPOLLOUT;

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
    // 将此Channel在epoll中修改或者添加
    loop_->update_channel(this);
}

void Channel::handle_events() {
    LOG<<"handle_events--->8";
    // QUESTION: EPOLLPRI?
    // QUESTION:为什么EPOLLHUP需要放在读和写的前面
    // if((revent_&EPOLLHUP) && !(revent_&EPOLLIN)) {
    //    if (close_handler_) close_handler_();
    //    return;
    // }
    if (revent_ & (EPOLLERR)) {
        LOG<<"handle error events";
        if (error_handler_) error_handler_();
    }
    if (revent_ & EPOLLIN) {
        // EPOLLIN 可读事件，当peer关闭连接的时候也会触发EPOLLIN事件
        LOG<<"handle read events";
        if (read_handler_) read_handler_();
    }
    if (revent_ & EPOLLOUT) {
        LOG<<"handle write events";
        if (write_handler_) write_handler_();
    }
}

void Channel::enable_read() {
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

