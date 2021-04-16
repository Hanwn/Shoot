#include "file2disk.h"

WriteFile::WriteFile(std::string file_name) :fp_(fopen(file_name.c_str(), "ae")) {
    // set fp_ buffer
    setbuffer(fp_, buf_, sizeof buf_);
}


WriteFile::~WriteFile() {
    fclose(fp_);
}


void WriteFile::append(const char* log_line, size_t len) {
    size_t n = this->write(log_line, len);
    size_t remain = len - n;
    // Maybe not all at once
    while (remain > 0) {
        size_t x = this->write(log_line + n, remain);
        if (x == 0) {
            int err = ferror(fp_);
            if (err) fprintf(stderr, "WriteFile::append() failed!\n");
            break;
        }
        n += x;
        remain = len - n;
    }
}


void WriteFile::flush() {
    fflush(fp_);
}


size_t WriteFile::write(const char* log_line, size_t len) {
    // fwrite_unlocked is nonlocking stdio functions,
    // the parameter 1 means each data size, not begin pos
    // no race condition
    return fwrite_unlocked(log_line, 1, len, fp_);
}
