#include "TcpServer.hpp"
#include "tools.hpp"
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
    }


TCPServer::~TCPServer() {

}

void TCPServer::start() {
    event_loop_pool_->start(cb_);
    accpet_channel_->set_read_callback(std::bind(&TCPServer::handle_new_conn, this));
    // 将当前channel加入到loop监听中
    accpet_channel_->enable_read();

    started_ = true;
}

void TCPServer::handle_new_conn() {
    // 
    struct sockaddr_in client_addr;
    ::memset(&client_addr, 0, sizeof(struct sockaddr_in));
    socklen_t client_addr_len = sizeof (client_addr);
    int _accept_fd = 0;
    while ((_accept_fd = accept(listen_fd_, (struct sockaddr *)&client_addr, &client_addr_len)) > 0) {
        EventLoop* _loop = event_loop_pool_->get_next_loop();
        LOG<<"New Connection from" << inet_ntoa(client_addr.sin_addr)<<":"<<ntohs(client_addr.sin_port);
        if (_accept_fd >= MAXFDS) {
            ::close(_accept_fd);
            continue;
        }
    }
    if (set_non_blocking(_accept_fd) < 0) {
        LOG<<"Set non blocking io failed";
        return;
    }
    // TODO:new a connection


}