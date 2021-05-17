#include "http_parse.hpp"


void HTTPParse::parse_body() {

}

bool HTTPParse::parse_header(char* buf, int len) {
    int idx = 0;
    if (buf[0] == 'G' && buf[1] == 'E'&& buf[2] == 'T') {
        this->method_ = HTTP_METHOD::GET;
        idx+=3;
    }else if(buf[idx] == 'P'){
        this->method_ = HTTP_METHOD::POST;
        idx+=4;
    }else {
        //TODO:更多的请求方式
        return false;
    }
    std::string _path;
    while (idx < len && buf[idx] != ' ') {
        _path += buf[idx];
        ++idx;
    }
    path_ = std::move(_path);
    int path_idx = path_.length();
    while (path_[path_idx] != '/') {
        filename_ += path_[path_idx];
        path_idx--;
    }
    filename_.reserve();
    ++idx;        
    if (idx + 3 < len) {
        if (buf[idx] != 'H' || buf[idx + 1] != 'T' || buf[idx + 2] != 'T' || buf[idx + 3] != 'P') {
            // 说明不是一个http协议
            return false;
        }
    }
    idx+=7;
    if (idx < len) {
        if (buf[idx] == '0') {
            version = HTTP_VERSION::HTTP_10;
        }else {
            version = HTTP_VERSION::HTTP_11;
        }
    }
    //TODO:解析全部的http头文件
    while (idx < len) {

    }
    return true;


// GET / HTTP/1.1
// Host: 127.0.0.1:9618
// Connection: keep-alive
// sec-ch-ua: " Not A;Brand";v="99", "Chromium";v="90", "Google Chrome";v="90"
// sec-ch-ua-mobile: ?0
// Upgrade-Insecure-Requests: 1
// User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/90.0.4430.212 Safari/537.36
// Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9
// Sec-Fetch-Site: none
// Sec-Fetch-Mode: navigate
// Sec-Fetch-User: ?1
// Sec-Fetch-Dest: document
// Accept-Encoding: gzip, deflate, br
// Accept-Language: zh-CN,zh;q=0.9

}
