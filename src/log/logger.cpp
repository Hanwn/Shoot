#include "logger.h"
#include "utils.h"
#include <sys/time.h>


Logger::Logger(const char* _file_name, int _line) 
    : file_name_(_file_name),
      line_(_line),
      stream_(){
          format_time();
      }

LogStream& Logger::stream() {
    return stream_;
}


// QUESTION: time module
void Logger::format_time() {
    struct timeval tv;
    time_t time;
    char str_t[26] = {0};
    ::gettimeofday(&tv, nullptr);
    time = tv.tv_sec;
    struct tm* p_time = localtime(&time);
    strftime(str_t, 26, "%Y-%m-%d %H:%M:%S\t", p_time);
    stream_ << str_t;
}


Logger::~Logger() {
    stream_ <<" -- " << file_name_ << ":" << line_ << '\n';
    // 将一个Logger对象中的所有日志信息取出
    const LogStream::Buffer& buf(stream().buffer());
    // utils.h/output function 将一个Logger中对象的日志输出到Async_logging的生产者队列
    output(buf.data(), buf.size());
}

