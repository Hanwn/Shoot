cmake_minimum_required(VERSION 3.0)

set(NET_SRC
    Channel.cc
    Epoll.cc
    EventLoop.cc
    EventLoopPool.cc
    EventLoopThread.cc
    TcpConnection.cc
    TcpServer.cc
    tools.cc
    http_parse.cc
)

add_library(libnet ${NET_SRC})

# target_link_libraries(libnet pthread rt)
target_link_libraries(libnet PUBLIC libLOG timer)
target_include_directories(libnet PUBLIC ./)

add_subdirectory(test)