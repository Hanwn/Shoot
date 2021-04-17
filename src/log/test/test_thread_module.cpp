#include <iostream>
#include "../Thread.h"

using namespace std;

void test() {

    cout<<"running in test function"<<endl;

}

int main() {
    string thread_name = "test";
    Thread t(test, thread_name);
    t.start();
    t.join();

    return 0;
}