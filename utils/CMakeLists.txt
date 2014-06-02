cmake_minimum_required(VERSION 2.8)

FILE(GLOB_RECURSE UTILS_SRC  *.cpp)

add_library(utils ${UTILS_SRC})

