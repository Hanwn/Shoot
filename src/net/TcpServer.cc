#include "TcpServer.hpp"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <functional>
#include "TimerGuard.hpp"
#include <sys/syscall.h>


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
            // LOG<<"set non_blocking";
        }
        // LOG<<"new listen fd: "<<listen_fd_<<" port "<<port;
    }


TCPServer::~TCPServer() {
    //TODO: release resource
    for (auto& item : connections_) {
        std::shared_ptr<TCPConnection> conn(item.second);
        item.second.reset();
        conn->get_loop()->run_in_loop(std::bind(&TCPConnection::destroy_conn, conn));
    }
}

void TCPServer::start(int thread_num) {
    // LOG<<static_cast<int>(::syscall(SYS_gettid))<<"--->1";
    // LOG<<"Server is RUNNING:"<<"--->1";
    event_loop_pool_->set_thread_nums(4);
    event_loop_pool_->start(cb_);
    accpet_channel_->set_read_callback(std::bind(&TCPServer::handle_new_conn, this));
    // 将当前channel加入到loop监听中
    accpet_channel_->enable_read();

    started_ = true;
}

void TCPServer::handle_new_conn() {
    // LOG<<"handle_new_conn--->9";
        // LOG<<static_cast<int>(::syscall(SYS_gettid))<<"--->15";
    struct sockaddr_in client_addr;
    ::memset(&client_addr, 0, sizeof(struct sockaddr_in));
    socklen_t client_addr_len = sizeof (client_addr);
    int _accept_fd = 0;
    if ((_accept_fd = accept(listen_fd_, (struct sockaddr *)&client_addr, &client_addr_len)) > 0) {
        EventLoop* _loop = event_loop_pool_->get_next_loop();
        // LOG<<"New Connection from " << inet_ntoa(client_addr.sin_addr)<<":"<<ntohs(client_addr.sin_port);
        if (_accept_fd >= MAXFDS) {
            ::close(_accept_fd);
            //retuen ? continue?
        }
    
        if (set_non_blocking(_accept_fd) < 0) {
            // LOG<<"Set non blocking io failed";
            return;
        }
        std::shared_ptr<TimerGuard<TCPConnection>> _timer = loop_->get_timer_();
        std::shared_ptr<TCPConnection> conn_ptr(new TCPConnection(_loop, _accept_fd, _timer));
        // TODO:conn_ptr->set_read_callback();
        // conn_ptr->set_read_callback();
        conn_ptr->set_close_callback(
                std::bind(&TCPServer::remove_conn, this, std::placeholders::_1)
                );

        // DONE:conn_ptr 被释放了
        // connections_["a"] = conn_ptr;
        // connections_[_accept_fd] = conn_ptr;
        _loop->run_in_loop(std::bind(&TCPConnection::new_event, conn_ptr));
        
    }
    accpet_channel_->enable_read();
    // DONE:new a connection
}

// main thread 负责回收已经建立的连接
// 由channel回调close_cb进行关闭
void TCPServer::remove_conn(const std::shared_ptr<TCPConnection>& conn) {
    loop_->run_in_loop(std::bind(&TCPServer::remove_conn_in_loop, this, conn));
}

void TCPServer::remove_conn_in_loop(std::shared_ptr<TCPConnection>& conn) {
    size_t n = connections_.erase(conn->get_fd());
    EventLoop* _loop = conn->get_loop();
    _loop->queue_in_loop(std::bind(&TCPConnection::destroy_conn, conn));

}
