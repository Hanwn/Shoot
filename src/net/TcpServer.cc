#include "TcpServer.hpp"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

TCPServer::TCPServer(EventLoop* _loop, std::string _thread_name, int port)
    : loop_(_loop)
    , thread_name(_thread_name)
    , event_loop_pool_(new EventLoopPool(loop_, _thread_name))
    , started_(false)
    , port_(port)
    , listen_fd_(socket_bind_listen(port_)) 
    , accpet_channel_(new Channel(loop_, listen_fd_)) {

        handle_for_sigpipe();
        if (set_non_blocking(listen_fd_) < 0) {
            LOG<<"set non_blocking";
        }
        LOG<<"new listen fd:"<<listen_fd_;
    }


TCPServer::~TCPServer() {
    //TODO: release resource
}

void TCPServer::start() {
    event_loop_pool_->start(cb_);
    event_loop_pool_->set_thread_nums(4);
    accpet_channel_->set_read_callback(std::bind(&TCPServer::handle_new_conn, this));
    // 将当前channel加入到loop监听中
    accpet_channel_->enable_read();

    started_ = true;
    LOG<<"Server is RUNNING";
}

void TCPServer::handle_new_conn() {
    LOG<<"new connection";
    struct sockaddr_in client_addr;
    ::memset(&client_addr, 0, sizeof(struct sockaddr_in));
    socklen_t client_addr_len = sizeof (client_addr);
    int _accept_fd = 0;
    while ((_accept_fd = accept(listen_fd_, (struct sockaddr *)&client_addr, &client_addr_len)) > 0) {
        EventLoop* _loop = event_loop_pool_->get_next_loop();
        LOG<<"New Connection from" << inet_ntoa(client_addr.sin_addr)<<":"<<ntohs(client_addr.sin_port);
        if (_accept_fd >= MAXFDS) {
            ::close(_accept_fd);
            //retuen ? continue?
            continue;
        }
    
        if (set_non_blocking(_accept_fd) < 0) {
            LOG<<"Set non blocking io failed";
            return;
        }

        std::shared_ptr<TCPConnection> conn_ptr(new TCPConnection(loop_, _accept_fd));
        loop_->run_in_loop(std::bind(&TCPConnection::new_event, conn_ptr));
        
    }
    // TODO:new a connection


}