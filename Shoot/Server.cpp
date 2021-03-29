//BUG:把epoll和time_改为全局变量，方便再do_request中使用

#include "Server.h"


Server::Server() {
    this->epoll = new Epoll();
    this->tq = new task_queue_t();
    this->pool = new Thread_pool();
    // 4个线程
    this->pool->thread_init(4, this->tq);
    this->time_ = new time_s_t();
    this->http = new Http();
    this->listen_fd = Utils::socket_bind_listen(9618);
    Utils::set_nonblocking_io(this->listen_fd);

}
Server::~Server() {
    delete tq;
    delete pool;
    delete http;
    delete epoll;
    delete time_;
}

void Server::start() {
    //
    http_request_t* req = new http_request_t();
    req->fd = this->listen_fd;
    epoll->epoll_event_add(this->listen_fd, req ,(EPOLLIN | EPOLLET));

    while (1) {
        int events_count = epoll->epoll_event_wait(MAXEVENTS, -1);
        time_->handle_expire_time_ns() ;
        this->handle_events(events_count);
    }
}

void Server::accept_connection() {
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = 0;
    int accpet_fd = accept(this->listen_fd, (struct sockaddr*)&client_addr, &client_addr_len);
    if (accpet_fd == -1) {
        perror("accpet");
    }
    std::cout<<accpet_fd<<"\n";
    //TODO:错误处理
    Utils::set_nonblocking_io(accpet_fd);

    //TODO:初始化request_t类型的节点
    http_request_t* request = new http_request_t();
    request->fd = accpet_fd;
    epoll->epoll_event_add(accpet_fd, request, (EPOLLIN | EPOLLET | EPOLLONESHOT));
    handler_pq close_conn_ = Http::close_conn;
    time_->time_n_add(request, TIMEOUT_DEFAULT, close_conn_);

}

void Server::handle_events(int events_count) {
    for (int i = 0; i < events_count; ++i) {
        struct epoll_event *tmp = (epoll_event*)epoll->get_events(i);
        http_request_t* request = (http_request_t *)(tmp->data.ptr);
        int fd = request->fd;
        if (fd == this->listen_fd) {
            this->accept_connection();
        }else{
            if ((tmp->events & EPOLLERR) || (tmp->events & EPOLLHUP) || (!(tmp->events & EPOLLIN))) {
                close(fd);
                continue;
            }
            //TODO:向任务队列中加入任务
            this->tq->add(Http::do_request, tmp->data.ptr);
        }
    }
}