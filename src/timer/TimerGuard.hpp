#ifndef TIMER_GUARD_H_
#define TIMER_GUARD_H_

#include "TimerNode.hpp"
#include <iostream>
#include <queue>

template<typename T>
struct timer_cmp{
    bool operator() (std::shared_ptr<TimerNode<T>>& a,
                     std::shared_ptr<TimerNode<T>>& b) {
                         return a->get_over_time() > b->get_over_time();
                     }
};

template<typename T>
class TimerGuard{
public:
    using smp_sp_timer_node = std::shared_ptr<TimerNode<T>>;
    TimerGuard() = default;
    ~TimerGuard() = default;

public:
    void handle_overtime() {
        while (!timer_node_heap.empty()) {
            smp_sp_timer_node now = timer_node_heap.top();
            if (now->is_deleted()) {
                timer_node_heap.pop();
            } else if (now->is_valid()) {
                timer_node_heap.pop();
            } else {
                break;
            }
        }
    }

    void add_time_node(std::shared_ptr<T> _data, int timeout) {
        smp_sp_timer_node new_time_node(new TimerNode<T>(_data, timeout));
        timer_node_heap.push(new_time_node);
    }

private:

    std::priority_queue<smp_sp_timer_node, std::deque<smp_sp_timer_node>, timer_cmp<T>> timer_node_heap;

};


#endif