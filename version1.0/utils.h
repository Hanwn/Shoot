#ifndef UTILS_H_
#define UTILS_H_

#include <string>

class Utils{
public:
    static int set_nonblocking_io(int);
    static int socket_bind_listen(int);
    static int read_conf();

};

struct http_request;

typedef void (*handler_pq)(http_request*);
typedef struct time_node {
    size_t key;
    int deleted;
    handler_pq handler;
    http_request* request;
}time_n_t;

typedef enum state_ {
   state_parse_uri = 0,
   state_parse_header,
   state_parse_body,
   state_parse_analysis,
   state_parse_fin
}stat_t;

enum parse_uri_flag{
    parse_uri_again = 0,
    parse_uri_error,
    parse_uri_success
};

enum parse_header_flag {
    parse_header_again = 0,
    parse_header_error,
    parse_header_success
};

enum parse_analysis_flag {
    parse_analysis_again = 0,
    parse_analysis_error,
    parse_analysis_success
};

enum method_{
    method_get = 0,
    method_post,
    method_delete,
    method_put
};

enum HTTP_V {
    HTTP_10 = 0,
    HTTP_11
};

class Epoll;
using time_s_t = class time;

typedef struct http_request
{
    /* data */
    //此结构体中应该包含一个存储http头的结构，一个位置指针，用来记录发送文件的位置
    std::string uri;
    HTTP_V http_v;
    std::string inbuf_;
    std::string outbuf_;
    state_ state;
    method_ method;
    time_n_t* timer;
    std::string cli_ip;
    int resolve;
    int fd;
}http_request_t;

#endif