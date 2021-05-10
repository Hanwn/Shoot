add_executable(test_net_module test.cc)
# add_executable(test_net_soc test_sock.cc)
target_link_libraries(test_net_module libnet timer libLOG)