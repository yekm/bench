#ifndef THRUST_POPCNT_HPP
#define THRUST_POPCNT_HPP

#include <vector>

// TODO: move cuda_get_free_mem() from thrust_generate_random.cu to somewhere else
size_t cuda_get_free_mem();

template<typename T>
size_t thrust_popcnt_ex(typename std::vector<T>::const_iterator begin,
                   typename std::vector<T>::const_iterator end);

template<typename T>
size_t thrust_popcnt(const std::vector<T> &v)
{
    size_t left = v.size();
    size_t free = cuda_get_free_mem() / sizeof(T);
    free -= free/8;
    size_t done = 0;
    size_t popcount = 0;
    do
    {
        size_t todo = left > free ? free : left;
        popcount += thrust_popcnt_ex<T>(v.cbegin() + done, v.cbegin() + done + todo);
        done += todo;
        left -= todo;
    } while (left != 0);
    return popcount;
}

#endif // THRUST_POPCNT_HPP
