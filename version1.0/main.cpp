#include "Server.h"

time_s_t time_;
Epoll epoll;

int main() {
    Server* s = new Server();
    s->start();
    delete s;
}