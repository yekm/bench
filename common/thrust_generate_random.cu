#include <vector>
#include <typeinfo>

#include <thrust/device_vector.h>
#include <thrust/random.h>
#include <thrust/iterator/counting_iterator.h>
#include <thrust/functional.h>

__device__
unsigned int hash(unsigned int a)
{
    a = (a+0x7ed55d16) + (a<<12);
    a = (a^0xc761c23c) ^ (a>>19);
    a = (a+0x165667b1) + (a<<5);
    a = (a+0xd3a2646c) ^ (a<<9);
    a = (a+0xfd7046c5) + (a<<3);
    a = (a^0xb55a4f09) ^ (a>>16);
    return a;
}

template<typename T>
class RealRandomNumberFunctor : public thrust::unary_function<unsigned int, T>
{
public:
    RealRandomNumberFunctor(unsigned int seed, T min, T max)
        : m_seed(seed)
        , m_min(min)
        , m_max(max)
    {}

    __device__
    T operator()(unsigned int thread_id)
    {
        thrust::default_random_engine rng(m_seed + hash(thread_id));
        thrust::uniform_real_distribution<T> distribution(m_min, m_max);
        return distribution(rng);
    }

private:
    unsigned int m_seed;
    T m_min, m_max;
};

template<typename T>
class IntRandomNumberFunctor : public thrust::unary_function<unsigned int, T>
{
public:
    IntRandomNumberFunctor(unsigned int seed, T min, T max)
        : m_seed(seed)
        , m_min(min)
        , m_max(max)
    {}

    __device__
    T operator()(unsigned int thread_id)
    {
        thrust::default_random_engine rng(m_seed + hash(thread_id));
        thrust::uniform_int_distribution<T> distribution(m_min, m_max);
        return distribution(rng);
    }

private:
    unsigned int m_seed;
    T m_min, m_max;
};

template<typename T>
void thrust_generate_random_ex(typename std::vector<T>::iterator,
                               size_t, unsigned int, T, T);
/*
{
    std::string message("thrust random generator do not support ");
    throw std::logic_error(message + typeid(T).name());
    // and then I realised that it will be never used.
}
*/

template<>
void thrust_generate_random_ex<float>(std::vector<float>::iterator begin,
                                   size_t size,
                                   unsigned int seed,
                                   float min, float max)
{
    thrust::device_vector<float> d_vec(size);
    thrust::transform(thrust::counting_iterator<int>(0),
            thrust::counting_iterator<int>(size),
            d_vec.begin(), RealRandomNumberFunctor<float>(seed, min, max));

    thrust::copy(d_vec.begin(), d_vec.end(), begin);
}

template<>
void thrust_generate_random_ex<int>(std::vector<int>::iterator begin,
                                 size_t size,
                                 unsigned int seed,
                                 int min, int max)
{
    thrust::device_vector<int> d_vec(size);
    thrust::transform(thrust::counting_iterator<int>(0),
            thrust::counting_iterator<int>(size),
            d_vec.begin(), IntRandomNumberFunctor<int>(seed, min, max));

    thrust::copy(d_vec.begin(), d_vec.end(), begin);
}

template<>
void thrust_generate_random_ex<char>(std::vector<char>::iterator begin,
                                  size_t size,
                                  unsigned int seed,
                                  char min, char max)
{
    thrust::device_vector<char> d_vec(size);
    thrust::transform(thrust::counting_iterator<int>(0),
            thrust::counting_iterator<int>(size),
            d_vec.begin(), IntRandomNumberFunctor<char>(seed, min, max));

    thrust::copy(d_vec.begin(), d_vec.end(), begin);
}

size_t cuda_get_free_mem()
{
    size_t mem_tot;
    size_t mem_free;
    cudaMemGetInfo(&mem_free, &mem_tot);
    return mem_free;
    //std::cout << "Free memory : " << mem_free << std::endl;
    //std::cout << "Total memory : " << mem_tot << std::endl;
}
