#ifndef _LOGGER_H_
#define _LOGGER_H_

#include "log_stream.h"

class Logger {
public:
    Logger(const char* _file_name, int line);
    ~Logger();

public:
    LogStream& stream();
    void format_time();

private:
    // QUESTION:内部类的好处?
    int line_;
    LogStream stream_;
    std::string file_name_;
};

 
#define LOG Logger(__FILE__, __LINE__).stream()
#endif