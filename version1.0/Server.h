#ifndef SERVER_H_
#define SERVER_H_

#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "epoll.h"
#include "utils.h"
#include "http.h"
#include "time.h"
#include "task_queue.h"
#include "thread_pool.h"

class Server{
public:
    Server();
    ~Server();

public:
    void start();
    void end();

public:
    
    void accept_connection();
    void handle_events(int);


private:
    Epoll* epoll;
    task_queue_t *tq;
    Thread_pool* pool;
    Http* http;
    int listen_fd;
    time_s_t* time_;

};


#endif