#include "log_stream.h"
#include <iostream>


int main() {

    LogStream ll;
    ll<<"this << is << test << sentence."<<'\n';
    ll.display();
}
