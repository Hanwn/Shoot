#include "TcpServer.hpp"
#include "EventLoop.hpp"
#include <cctype>
#include <cstdlib>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

bool is_digital(const char* para) {
    char* ptr = const_cast<char*>(para);
    while (*ptr != '\0') {
        if (!std::isdigit(*ptr)) return false;
        ptr++;
    }
    return true;
}


int main(int argc, char** argv) {
    int thread_number = 4;
    int port = 9618;
    for (int i = 1; i < argc;++i) {
        if (strcmp(argv[i], "-p") == 0) {
            if (i >= argc || !is_digital(argv[i + 1])) {
                perror("port number error");
                exit(1);
            }
            ++i;
            port = atoi(argv[i]);
        } else if (strcmp(argv[i], "-t") == 0) {
            if (i >= argc || !is_digital(argv[i + 1])) {
                perror("port number error");
                exit(1);
            }
            ++i;
            thread_number = atoi(argv[i]);
        }
    }
    EventLoop* loop = new EventLoop();
    std::string name = "test";
    TCPServer* ts = new TCPServer(loop, name, port);
    ts->start(thread_number);
    loop->loop();
    delete loop;
    delete ts;
    return 0;
}
