cmake_minimum_required(VERSION 2.8)

project(bench)

find_package(CUDA QUIET)

if(CUDA_FOUND)
    add_definitions("-DCUDA_FOUND")
    include_directories(${CUDA_INCLUDE_DIRS})
    list(APPEND ADDITIONAL_LIBS ${CUDA_LIBRARIES})
endif(CUDA_FOUND)


set(CMAKE_VERBOSE_MAKEFILE 1)
add_definitions("-DDEBUG")

set(SRC
    main.cpp
    algorithm.cpp
    task.cpp
    taskcollection.cpp
    taskdata.cpp
    aresult.cpp
    )

include_directories(${PROJECT_SOURCE_DIR})
include_directories(tasks)
#add_definitions("-std=c++0x -ggdb3 -O0 -Wall -Wextra")
#add_definitions("-std=c++0x -ggdb2 -O3 -Wall -Wextra -Wa,-alhnd=/tmp/bench/$(notdir $@.lst)")
add_definitions("-std=c++0x -O3 -Wall -Wextra")

add_subdirectory(tasks)
add_subdirectory(utils)

add_executable(${CMAKE_PROJECT_NAME} ${SRC})

target_link_libraries(${CMAKE_PROJECT_NAME}
    -Wl,--whole-archive tasks -Wl,--no-whole-archive
    utils
    -lrt
    ${ADDITIONAL_LIBS}
    )
