cmake_minimum_required(VERSION 2.8)

project(bench)

message("CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}")
set(CMAKE_VERBOSE_MAKEFILE 1)
IF(CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions("-DDEBUG")
ELSEIF(CMAKE_BUILD_TYPE MATCHES Release)
    message("Release build.")
ELSE()
    message("Some other build type.")
ENDIF()


find_package(CUDA QUIET)

if(CUDA_FOUND)
    add_definitions("-DCUDA_FOUND")
    set(CUDA_NVCC_FLAGS "${CUDA_NVCC_FLAGS};-gencode arch=compute_12,code=sm_12")
    include_directories(${CUDA_INCLUDE_DIRS})

    CUDA_ADD_LIBRARY(MainCudaLib common/thrust_generate_random.cu)

    list(APPEND ADDITIONAL_LIBS ${CUDA_LIBRARIES} MainCudaLib)
endif(CUDA_FOUND)


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
set(CMAKE_C_FLAGS_DEBUG     "-ggdb3 -O0 -Wall -Wextra")
set(CMAKE_C_FLAGS_RELEASE   "-march=native -Ofast -Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG   "-std=c++0x -ggdb3 -O0 -Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "-std=c++0x -march=native -Ofast -Wall -Wextra")

add_subdirectory(tasks)
add_subdirectory(utils)

add_executable(${CMAKE_PROJECT_NAME} ${SRC})

target_link_libraries(${CMAKE_PROJECT_NAME}
    -Wl,--whole-archive tasks -Wl,--no-whole-archive
    utils
    -lrt
    -pthread
    ${ADDITIONAL_LIBS}
    )
