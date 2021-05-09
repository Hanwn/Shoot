#include "TcpServer.hpp"
#include "EventLoop.hpp"

int main() {
    EventLoop* loop = new EventLoop();
    std::string name = "test";
    int port = 9618;
    TCPServer* ts = new TCPServer(loop, name, port);
    ts->start();
    loop->loop();
    delete loop;
    delete ts;
    return 0;
}