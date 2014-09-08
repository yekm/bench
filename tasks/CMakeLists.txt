cmake_minimum_required(VERSION 2.8)

if(CUDA_FOUND)
    cuda_compile(THRUST_SORT_O simple_sorting/thrust_sort.cu)
    list(APPEND ADDITIONAL_O ${THRUST_SORT_O})
endif(CUDA_FOUND)

include_directories(simple_sorting/swenson-sort)

set(TASKS_SRC
    simple_sorting/sorting_task.cpp
    vector_length_test/vec_task.cpp
    popcnt/popcnt.cpp
)

add_library(tasks ${TASKS_SRC} ${ADDITIONAL_O})
