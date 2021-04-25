#include "Epoll.hpp"
#include <sys/epoll.h>

const int EVENT_SUM = 4096;


Epoll::Epoll() 
    : epoll_fd_(epoll_create1(EPOLL_CLOEXEC))
    , event_array_(EVENT_SUM) {

}


void Epoll::epoll_add(std::shared_ptr<Channel> _channel, int timeout) {
    
}


void Epoll::epoll_del(std::shared_ptr<Channel> _channel) {

}


void Epoll::epoll_mod(std::shared_ptr<Channel> _channel, int timeout) {


}


std::vector<std::shared_ptr<Channel>> Epoll::poll() {

}

void Epoll::handle_overtimer() {


}


