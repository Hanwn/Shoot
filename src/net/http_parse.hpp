#ifndef HTTP_PARSE_H_
#define HTTP_PARSE_H_

#include "tools.hpp"
#include <string>
#include <unordered_map>

class HTTPParse{
public:


public:

    bool parse_header(char* buf, int len);
    void parse_body();
    HTTP_METHOD get_method() {return method_ ;};
    std::string& get_filename() {return filename_;}

private:
    
    HTTP_METHOD method_;
    HTTP_VERSION version;
    std::string path_;
    std::string filename_;
    bool is_keep_alive;
    std::unordered_map<std::string,std::string> http_header_;

};


#endif