#ifndef HTTP_PARSE_H_
#define HTTP_PARSE_H_

#include "tools.hpp"

class HTTPParse{
public:


public:

    void parse_header();
    void parse_body();
    HTTP_METHOD get_method() {return method_ ;};

private:
    
    HTTP_METHOD method_;

};


#endif