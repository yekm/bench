#ifndef THRUST_GENERATE_RANDOM_HPP
#define THRUST_GENERATE_RANDOM_HPP

size_t cuda_get_free_mem();

template<typename T>
void thrust_generate_random_ex(typename std::vector<T>::iterator,
                            size_t, unsigned int, T, T);

template<typename T>
void thrust_generate_random(std::vector<T> &v, unsigned int seed, T min, T max)
{
    size_t left = v.size();
    size_t free = cuda_get_free_mem() / sizeof(T);
    free -= free/8; // dont eat up all, just in case
    do
    {
        size_t todo = left > free ? free : left;
        // TODO: sort out seed
        thrust_generate_random_ex(v.begin(), todo, seed^left, min, max);
        left -= todo;
    } while (left != 0);
}

#endif // THRUST_GENERATE_RANDOM_HPP
