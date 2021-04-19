#include "TimerGuard.hpp"
#include <iostream>
#include <unistd.h>
#include <sys/time.h>
class A{
public:
    A() {
        std::cout<<"test"<<std::endl;
    }

    ~A() {
        std::cout<<"~A()"<<std::endl;
    }
};

void add(std::shared_ptr<A>& a, TimerGuard<A>& guard) {
    guard.add_time_node(a,10000);
}

void test(TimerGuard<A>& guard) {
    std::shared_ptr<A> a(new A());
    guard.add_time_node(a,0);
    add(a, guard);
}

int main() {

    TimerGuard<A> guard;
    test(guard);
    while(1) {
        guard.handle_overtime();
    }
    return 0;
}