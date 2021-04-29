#include "EventLoop.hpp"


EventLoop::EventLoop() {

}


EventLoop::~EventLoop() {

}


void EventLoop::loop() {
    
}


void EventLoop::update_channel(Channel* _channel) {
    // 函数调用，分别负责epoll_add,epoll_del,epoll_mod
    // 要从_channel中获取当前channel感兴趣的事件
    channelStatus _status = _channel->get_status();
    // QUESTION:为什么要将DEL的fd加入到epoll中？，此外，什么情况会发生一个文件描述符会被加入两次的情况
    if (_status == channelStatus::NEW || _status == channelStatus::DEL) {
        _channel->set_status(channelStatus::ADD);
        poll_->epoll_add(_channel, EPOLL_CTL_ADD);
    }else if(_status == channelStatus::ADD) {
        //TODO:channel->isNoneEvent();
        poll_->epoll_mod(_channel,EPOLL_CTL_MOD);
    }
}


void EventLoop::remove_channel(Channel* _channel) {

}


void EventLoop::run_in_curr_loop() {

}

void EventLoop::queue_in_loop() {

}

void EventLoop::do_pending_functions() {

}

void EventLoop::wake_up_cur_thread()  {


}