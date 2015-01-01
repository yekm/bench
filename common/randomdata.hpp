#ifndef RANDOMDATA_H
#define RANDOMDATA_H

#include <vector>
#include <limits>
#include "genericdata.hpp"
#include "utils/not_so_random_device.hpp"
#include "common/thrust_generate_random.hpp"
#include "utils/simple_histogram.hpp"

#include <random>
#include <algorithm>
#include <type_traits>

namespace common
{

template <typename T>
class RandomData : public GenericData<std::vector<T>>
{
public:
    typedef GenericData<std::vector<T>> base_type;

    explicit RandomData(std::size_t n,
                        T min = std::numeric_limits<T>::min(),
                        T max = std::numeric_limits<T>::max())
        : base_type("uniform random " + range_str(min, max), n)
        , m_seed(utils::not_so_random_device()())
        , m_gen(m_seed)
        , m_min(min)
        , m_max(max)
    {
        base_type::m_data.resize(base_type::get_n());
        generate(m_min, m_max);

#ifdef DEBUG_RANDOM
        auto h = utils::simple_histogram(base_type::m_data, 50);
        utils::draw_histogram(h);
#endif
    }

    RandomData (const RandomData & o)
        : base_type(o.get_name(), o.get_n())
        , m_seed(o.m_seed)
        , m_gen(o.m_gen)
        , m_min(o.m_min)
        , m_max(o.m_max)
    {
        base_type::m_data.resize(base_type::get_n());
        generate(m_min, m_max);
    }

    virtual std::shared_ptr<TaskData> clone_copy() const override
    {
        return std::make_shared<RandomData<T>>(*this);
    }

protected:
    explicit RandomData(const std::string name,
                        std::size_t n,
                        T min = std::numeric_limits<T>::min(),
                        T max = std::numeric_limits<T>::max())
        : base_type(name, n)
        , m_seed(utils::not_so_random_device()())
        , m_gen(m_seed)
        , m_min(min)
        , m_max(max)
    {
        base_type::m_data.resize(base_type::get_n());
        generate(m_min, m_max);
    }

    std::string range_str(T min, T max) const
    {
        return "[" + std::to_string(min) + ", " + std::to_string(max) + "]";
    }

    T min() const
    {
        return m_min;
    }

    T max() const
    {
        return m_max;
    }

private:
    template <typename U = T,
              typename std::enable_if<std::is_integral<U>::value, int>::type = 0>
    void generate(U min, U max)
    {
#ifdef CUDA_FOUND
        thrust_generate_random(base_type::m_data, m_seed, min, max);
#else
        std::uniform_int_distribution<U> dis(min, max);
        std::generate_n(base_type::m_data.begin(), base_type::m_data.size(),
                        std::bind(dis, m_gen));
#endif
    }

    template <typename U = T,
              typename std::enable_if<std::is_floating_point<U>::value, int>::type = 0>
    void generate(U min, U max)
    {
#ifdef CUDA_FOUND
        thrust_generate_random(base_type::m_data, m_seed, min, max);
#else
        std::uniform_real_distribution<U> dis(min, max);
        std::generate_n(base_type::m_data.begin(), base_type::m_data.size(),
                        std::bind(dis, m_gen));
#endif
    }

    std::random_device::result_type m_seed;
    std::mt19937 m_gen;
    const T m_min, m_max;
};

} // ns common

#endif // RANDOMDATA_H
