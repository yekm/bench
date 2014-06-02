cmake_minimum_required(VERSION 2.8)

FILE(GLOB_RECURSE COMMON_SRC  *.cpp)

add_library(common ${COMMON_SRC})