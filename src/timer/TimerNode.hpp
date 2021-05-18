#ifndef TIMERNODE_HPP_
#define TIMERNODE_HPP_

#include <memory>
#include <sys/time.h>
#include <iostream>

template<typename T>
class TimerNode{
public:
    // timeout 单位为毫秒
    TimerNode(std::shared_ptr<T> _data, int timeout) 
        : data_(_data)
        , is_deleted_(false) {
            struct timeval tv;
            ::gettimeofday(&tv, nullptr);
            over_time_ = (((tv.tv_sec % 10000) * 1000) + (tv.tv_usec / 1000)) + timeout;
    }
    TimerNode(const TimerNode& time_node) {

    }    
    ~TimerNode()  {
        // std::cout<<"~TimerNode()"<<std::endl;
    };
public:
    void set_deleted() { is_deleted_ = true; }
    bool is_deleted() { return is_deleted_; };
    bool is_valid() {
        struct timeval tv;
        ::gettimeofday(&tv, nullptr);
        size_t t = (((tv.tv_sec % 10000) * 1000) + (tv.tv_usec / 1000));
        if (t < over_time_) return false;
        else {
            this->set_deleted();
            return false;
        }
    }
    size_t get_over_time() { return over_time_; }
private:
    bool is_deleted_;
    std::shared_ptr<T> data_;
    size_t over_time_;
};


#endif