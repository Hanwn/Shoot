// @author:hanwn
// @filename: log2file.h
// @description: 

#ifndef LOG2FILE_H_
#define LOG2FILE_H_

#include <memory>
#include <string>
#include "mutex_lock.h"
#include "file2disk.h"


class Log2File {
public:
    Log2File(const std::string& file_name, int flush_freq = 1024);
    ~Log2File() = default;

public:
    void append(const char* log_line, int len);
    void flush();

private:
    void append_unlocked(const char* log_line, int len);

private:
    const std::string file_name_;
    const int flush_freq_;

    int count_;

    std::unique_ptr<MutexLock> mutex_;
    std::unique_ptr<WriteFile> file_;
};

#endif