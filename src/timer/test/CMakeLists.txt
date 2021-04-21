project(Test)

add_executable(${PROJECT_NAME} test.cc)

target_link_libraries(${PROJECT_NAME} timer)

