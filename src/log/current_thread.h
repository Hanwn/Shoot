#ifndef _CURRENT_THREAD_H_
#define _CURRENT_THREAD_H_

#include <iostream>

thread_local pid_t thread_id_ = 0;
thread_local std::string thread_name_ = "default";
thread_local std::string thread_status_ = "default";


#endif