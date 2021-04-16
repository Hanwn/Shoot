#include "log2file.h"

Log2File::Log2File(const std::string& _file_name, int _flush_freq = 1023) 
    : file_name_(_file_name),
      flush_freq_(_flush_freq),
      count_(0),
      mutex_(new MutexLock),
      file_(new WriteFile(_file_name)) {}


//NOTE:append to file cache, When added to a certain number of times, this file in memory will be writed to disk.
void Log2File::append(const char* log_line, int len) {
    MutexLockGuard lock(*mutex_);
    append_unlocked(log_line, len);
}


// why need lock?
// I think on the reactor loops will be multi-thread environment and multi thread will exist race condition.
void Log2File::flush() {
    MutexLockGuard lock(*mutex_);
    file_->flush();
}


void Log2File::append_unlocked(const char* log_line, int len) {
    file_->append(log_line, len);
    ++count_;
    if (count_ >= flush_freq_) {
        count_ = 0;
        file_->flush();
    }
}