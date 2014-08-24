#include "thrust_sort.hpp"

#ifdef CUDA_FOUND

#include <thrust/device_vector.h>
#include <thrust/sort.h>

template<>
void thrust_sort_ex<int>(std::vector<int> & d)
{
    thrust::device_vector<int> d_vec(d.begin(), d.end());
    thrust::sort(d_vec.begin(), d_vec.end());
    thrust::copy(d_vec.begin(), d_vec.end(), d.begin());
}

template<>
void thrust_sort_ex<char>(std::vector<char> & d)
{
    thrust::device_vector<char> d_vec(d.begin(), d.end());
    thrust::sort(d_vec.begin(), d_vec.end());
    thrust::copy(d_vec.begin(), d_vec.end(), d.begin());
}

#endif
