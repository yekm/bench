#ifdef CUDA_FOUND

#include <thrust/device_vector.h>

template <typename T>
struct thrust_popcnt_kernel
{
    __host__ __device__
    T operator()(const T & x) const
    {
        return __popc(x);
    }
};

/* cant use dbg.h with nvcc. removed #include and copied declaration here */
template<typename T>
size_t thrust_popcnt_ex(typename std::vector<T>::const_iterator begin,
                   typename std::vector<T>::const_iterator end);

template<>
size_t thrust_popcnt_ex<int>(typename std::vector<int>::const_iterator begin,
                          typename std::vector<int>::const_iterator end)
{
    thrust::device_vector<int> d_vec(begin, end);
    return thrust::transform_reduce(d_vec.begin(), d_vec.end(),
                                    thrust_popcnt_kernel<int>(),
                                    size_t(), thrust::plus<int>());
}

#endif
