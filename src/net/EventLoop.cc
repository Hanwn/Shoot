#include "logger.h"
#include "EventLoop.hpp"
#include <syscall.h>
#include <unistd.h>
#include <sys/eventfd.h>

// 使用eventfd的读写实现线程唤醒的目的
int create_eventfd() {
    // QUESTION:
    int _fd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    return _fd;
}

EventLoop::EventLoop() 
    : poll_(new Epoll(this))
    , cur_thread_id_(static_cast<int>(::syscall(SYS_gettid)))
    , handle_pending_functions(false)
    , wake_up_fd_(create_eventfd())
    , wake_up_channel_(new Channel(this, wake_up_fd_))
    , quit_(true)
    {
        // wake_up_channel_->set_events(EPOLLIN | EPOLLET);
        wake_up_channel_->set_read_callback(std::bind(&EventLoop::wake_up_response, this));
        // 这里的调用会自动将这个channel_加入到Epoll的监控中
        wake_up_channel_->enable_read();
        // poll_->epoll_add(wake_up_channel_.get(), EPOLL_CTL_ADD);
    }


EventLoop::~EventLoop() {
    wake_up_channel_->disalbel_all();
    wake_up_channel_->remove();
    ::close(wake_up_fd_);
}


void EventLoop::loop() {
    LOG<<"loop--->5";
    quit_ = false;
    while(!quit_) {
        active_vector.clear();
        poll_->poll(active_vector);
        for (auto item : active_vector) {
            item->handle_events();
        }
        do_pending_functions();
        // TODO:处理超时请求
        handle_expired_time();
    }
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
    channelStatus _status = _channel->get_status();
    poll_->epoll_del(_channel, EPOLL_CTL_DEL);
    //QUESTION:
    _channel->set_status(channelStatus::NEW);
}


bool EventLoop::is_in_cur_loop() {
    return cur_thread_id_ == static_cast<int>(::syscall(SYS_gettid));
}

void EventLoop::run_in_loop(callback&& cb) {
    if (is_in_cur_loop()) {
        cb();
    }else {
        queue_in_loop(std::move(cb));
    }
}

void EventLoop::queue_in_loop(callback&& cb) {
    {
        MutexLockGuard _lock(mutex_);
        pending_functions_.emplace_back(std::move(cb));
    }
    if (!is_in_cur_loop() || handle_pending_functions) {
        wake_up_cur_thread();
    }
}

// 可能是别的线程加入当前线程的函数
void EventLoop::do_pending_functions() {
    std::vector<callback> functors;
    handle_pending_functions = true;
    {
        MutexLockGuard _lock(mutex_);
        functors.swap(pending_functions_);
    }
    for (int i = 0; i < functors.size(); ++i) {
        functors[i]();
    }
    handle_pending_functions = false;
}

void EventLoop::wake_up_cur_thread()  {
    LOG<<"new conn wake up";
    uint64_t one = 1;
    // BUG:write and read maybe a bug when ET
    ssize_t n = write(wake_up_fd_, (char*)(&one), sizeof one);
}

void EventLoop::wake_up_response() {
    uint64_t one = 1;
    ssize_t n = read(wake_up_fd_, &one, sizeof one);
}

//TODO:finish quit function
void EventLoop::quit() {

}

void EventLoop::handle_expired_time() {

}