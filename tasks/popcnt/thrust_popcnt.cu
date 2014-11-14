#include "thrust_popcnt.hpp"

#ifdef CUDA_FOUND

#include <thrust/device_vector.h>

template <typename T>
struct thrust_popcnt
{
    __host__ __device__
    T operator()(const T & x) const
    {
        return __popc(x);
    }
};

template<>
int thrust_popcnt_ex<int>(const std::vector<int> & d)
{
    thrust::device_vector<int> d_vec(d.begin(), d.end());
    return thrust::transform_reduce(d_vec.begin(), d_vec.end(),
                                    thrust_popcnt<int>(), 0, thrust::plus<int>());
}

#endif
