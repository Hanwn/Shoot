#ifndef FILE2DISK_H_
#define FILE2DISK_H_

#include <string>

class WriteFile{
public:
    explicit WriteFile(std::string file_name);
    ~WriteFile();
    void append(const char* log_line, const size_t len);
    void flush();

private:
    size_t write_unlocked_(const char* log_line, size_t len);
    FILE* fp_;
    char buf_[64*1024];
};


#endif