cmake_minimum_required(VERSION 3.0)

set(LIB_SRC
    async_logging.cpp
    CountDownLatch.cpp
    file2disk.cpp
    log_stream.cpp
    log2file.cpp
    logger.cpp
    Thread.cpp
)

add_library(libLOG ${LIB_SRC})
target_link_libraries(libLOG pthread rt)

set_target_properties(libLOG PROPERTIES OUTPUT_NAME "LOGMODULE")

add_subdirectory(test)