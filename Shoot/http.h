
#ifndef HTTP_H_
#define HTTP_H_

#include <unistd.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "time.h"
#include "utils.h"
#include "epoll.h"

#define BUFSIZE 4096




class Http{
public:
    static void do_request(void*);
    static void http_parse(void*);
    static void do_error();
    static void close_conn(http_request_t*);
    static int read_(int ,std::string&, int&);
    static int write_(int,std::string&);
    static void http_write(int, std::string&);
    static parse_uri_flag parse_uri(http_request_t*);
    static parse_header_flag parse_headers(http_request_t*);
    static parse_analysis_flag parse_analysis(http_request_t*);
    //static void parse_();

private:

};


#endif