cmake_minimum_required(VERSION 2.8)

#set(TRIALS_SRC
#    t1.cpp
#    t2.cpp
#    t3.cpp
#    )

# no good
include_directories(simple_sorting/swenson-sort)

FILE(GLOB_RECURSE TASKS_SRC  *.cpp)

add_library(tasks ${TASKS_SRC})
