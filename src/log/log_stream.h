#ifndef LOG_STREAM_H_
#define LOG_STREAM_H_

#include "fixed_buffer.h"


class LogStream{
public:
    using Buffer = FixedBuffer<small_buffer>;
    // typedef FixedBuffer<small_buffer> Buffer;
    LogStream& operator<<(bool v);
    LogStream& operator<<(short);
    LogStream& operator<<(unsigned short);
    LogStream& operator<<(int);
    LogStream& operator<<(unsigned int);
    LogStream& operator<<(long);
    LogStream& operator<<(unsigned long);
    LogStream& operator<<(long long);
    LogStream& operator<<(unsigned long long);
    LogStream& operator<<(float);
    LogStream& operator<<(double);
    LogStream& operator<<(long double);
    LogStream& operator<<(char);
    LogStream& operator<<(const char*);

    //QUESTION:const unsigned char*?
    LogStream& operator<<(const unsigned char*);
    LogStream& operator<<(std::string&);
    LogStream& operator<<(std::string&&);

    void append(const char* _data, int _len);
    const Buffer& buffer() const;
    void reset_buffer();

    //for test and debug function
    void display() {
        std::cout<<"+++++++++++++testing++++++++++++++++"<<std::endl;
        buffer_.display();
        std::cout<<"+++++++++++++testing++++++++++++++++"<<std::endl;
    }

private:
    
    template<typename T>
    void format_integer(T);

    Buffer buffer_;
    static const int number_size_limit = 32;
};


#endif