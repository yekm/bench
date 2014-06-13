cmake_minimum_required(VERSION 2.8)

project(bench)

set(CMAKE_VERBOSE_MAKEFILE 1)
add_definitions("-DDEBUG")

set(SRC
    main.cpp
    algorithm.cpp
    task.cpp
    taskcollection.cpp
    taskdata.cpp
    )

include_directories(${PROJECT_SOURCE_DIR})
include_directories(tasks)
#include_directories(common)
#add_definitions("-std=c++0x -ggdb3 -O0 -Wall -Wextra")
add_definitions("-std=c++0x -O3 -Wall -Wextra")

add_subdirectory(tasks)
add_subdirectory(common)
add_subdirectory(utils)

add_executable(${CMAKE_PROJECT_NAME} ${SRC})

target_link_libraries(${CMAKE_PROJECT_NAME}
    -Wl,--whole-archive tasks -Wl,--no-whole-archive
    common
    utils
    -lrt
    )
