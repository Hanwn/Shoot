#include <iostream>
#include <string>
#include <string.h>
#include <algorithm>
#include "log_stream.h"

// Efficient Integer to String Conversions, by Matthew Wilson.
// copy from moduo
// QUESTION:
const char digits[] = "9876543210123456789";
const char* zero = digits + 9;

template<typename T>
size_t convert(char buf[], T value) {
    T i = value;
    char* p = buf;

    do {
        int lsd = static_cast<int>(i % 10);
        i /= 10;
        *p++ = zero[lsd];
    } while (i != 0);

    if (value < 0) {
       *p++ = '-';
    }
    *p = '\0';
    std::reverse(buf, p);
    return p - buf;
}

LogStream& LogStream::operator<<(bool v) {
    buffer_.append(v ? "1" : "0", 1);
    return *this;
}

LogStream& LogStream::operator<<(short v) {
    *this << static_cast<int>(v);
    return *this;
}

LogStream& LogStream::operator<<(unsigned short v) {
    *this << static_cast<unsigned int>(v);
    return *this;
}

LogStream& LogStream::operator<<(int v) {
    format_integer(v);
    return *this;
}

LogStream& LogStream::operator<<(unsigned int v) {
    format_integer(v);
    return *this;
}

LogStream& LogStream::operator<<(long v) {
    format_integer(v);
    return *this;
}

LogStream& LogStream::operator<<(unsigned long v) {
    format_integer(v);
    return *this;
}

LogStream& LogStream::operator<<(long long v) {
    format_integer(v);
    return *this;
}

LogStream& LogStream::operator<<(unsigned long long v) {
    format_integer(v);
    return *this;
}

LogStream& LogStream::operator<<(float v) {
    *this<<static_cast<double>(v);
    return *this;
}

LogStream& LogStream::operator<<(double v) {
    if (buffer_.avail() >= number_size_limit) {
        //QUESTION:snprintf();
        int len = snprintf(buffer_.current(), number_size_limit, "%.12g", v);
        buffer_.add(len);
    }
    return *this;
}

LogStream& LogStream::operator<<(long double v) {
    if (buffer_.avail() >= number_size_limit) {
        int len = snprintf(buffer_.current(), number_size_limit, "%.12Lg", v);
        buffer_.add(len);
    }
    return *this;
}

LogStream& LogStream::operator<<(char v) {
    buffer_.append(&v, 1);
    return *this;
}

LogStream& LogStream::operator<<(const char* v) {
    if (v) {
        buffer_.append(v, strlen(v));
    }else{
        buffer_.append("(null)", 6);
    }
    return *this;
}

//QUESTION:const unsigned char*?
//WARNING: reinterpret_cast
LogStream& LogStream::operator<<(const unsigned char* v) {
    return operator<<(reinterpret_cast<const char*>(v));
}

LogStream& LogStream::operator<<(std::string& v) {
    buffer_.append(v.c_str(), v.size());
    return *this;
}

LogStream& LogStream::operator<<(std::string&& v) {
    buffer_.append(v.c_str(), v.size());
    return *this;
}

void LogStream::append(const char* _data, int _len) {
    buffer_.append(_data, _len);
}

const LogStream::Buffer& LogStream::buffer() const {
    return buffer_;
}

void LogStream::reset_buffer() {
    buffer_.reset();
}


// QUESTION:向buffer_中添加的内容是否要以'\0'结尾
template<typename T>
void LogStream::format_integer(T v) {
    if (buffer_.avail() >= number_size_limit) {
        size_t len = convert(buffer_.current(), v);
        buffer_.add(len);
    }
}