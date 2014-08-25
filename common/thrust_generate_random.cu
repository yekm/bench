#include <vector>
#include <typeinfo>

#include <thrust/device_vector.h>
#include <thrust/random.h>
#include <thrust/iterator/counting_iterator.h>
#include <thrust/functional.h>

__host__ __device__
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

    __host__ __device__
        float operator()(unsigned int thread_id)
    {
        thrust::default_random_engine rng(m_seed + hash(thread_id));
        thrust::uniform_real_distribution<T> urd(m_min, m_max);
        return urd(rng);
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

    __host__ __device__
        float operator()(unsigned int thread_id)
    {
        thrust::default_random_engine rng(m_seed + hash(thread_id));
        thrust::uniform_int_distribution<T> urd(m_min, m_max);
        return urd(rng);
    }

private:
    unsigned int m_seed;
    T m_min, m_max;
};

template<typename T>
void thrust_generate_random(std::vector<T> &, unsigned int, T, T);
/*
{
    std::string message("thrust random generator do not support ");
    throw std::logic_error(message + typeid(T).name());
    // and then I realised that it will be never used.
}
*/

template<>
void thrust_generate_random<float>(std::vector<float> & v, unsigned int seed, float min, float max)
{
    thrust::device_vector<float> d_vec(v.size());
    thrust::transform(thrust::counting_iterator<int>(0),
            thrust::counting_iterator<int>(v.size()),
            d_vec.begin(), RealRandomNumberFunctor<float>(seed, min, max));

    thrust::copy(d_vec.begin(), d_vec.end(), v.begin());
}

template<>
void thrust_generate_random<int>(std::vector<int> & v, unsigned int seed, int min, int max)
{
    thrust::device_vector<int> d_vec(v.size());
    thrust::transform(thrust::counting_iterator<int>(0),
            thrust::counting_iterator<int>(v.size()),
            d_vec.begin(), IntRandomNumberFunctor<int>(seed, min, max));

    thrust::copy(d_vec.begin(), d_vec.end(), v.begin());
}

template<>
void thrust_generate_random<char>(std::vector<char> & v, unsigned int seed, char min, char max)
{
    thrust::device_vector<char> d_vec(v.size());
    thrust::transform(thrust::counting_iterator<int>(0),
            thrust::counting_iterator<int>(v.size()),
            d_vec.begin(), IntRandomNumberFunctor<char>(seed, min, max));

    thrust::copy(d_vec.begin(), d_vec.end(), v.begin());
}
