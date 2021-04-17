cmake_minimum_required(VERSION 3.0)

project(LOGModule)

aux_source_directory(. DIR_SRCS)

add_executable(LOG ${DIR_SRCS})