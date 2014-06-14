#ifndef RANDOMDATA_H
#define RANDOMDATA_H

#include <vector>
#include <limits>
#include "genericdata.hpp"

#include <random>
#include <algorithm>

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
        : base_type("uniform random " + range(min, max), n)
        , m_n(n)
        , m_rd()
        , m_gen(m_rd())
        , m_min(min)
        , m_max(max)
    {
        base_type::m_data.resize(m_n);
        generate(m_min, m_max);
    }

    RandomData (const RandomData & o)
        : base_type(o.get_name(), o.m_n)
        , m_n(o.m_n)
        , m_rd()
        , m_gen(o.m_gen)
        , m_min(o.m_min)
        , m_max(o.m_max)
    {
        base_type::m_data.resize(m_n);
        generate(m_min, m_max);
    }

    virtual std::shared_ptr<TaskData> clone_copy() const
    {
        return std::make_shared<RandomData<T>>(*this);
        //return std::make_shared<RandomData<T>>(m_n);
    }

protected:
    explicit RandomData(const std::string name,
                        std::size_t n,
                        T min = std::numeric_limits<T>::min(),
                        T max = std::numeric_limits<T>::max())
        : base_type(name, n)
        , m_n(n)
        , m_rd()
        , m_gen(m_rd())
        , m_min(min)
        , m_max(max)
    {
        base_type::m_data.resize(m_n);
        generate(m_min, m_max);
    }

    std::string range(T min, T max) const
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
    void generate(T max, T min);
    /* segfault
    {
        std::uniform_int_distribution<T> dis(min, max);
        std::generate_n(base_type::m_data.begin(), base_type::m_data.size(),
                        std::bind(dis, m_gen));
    }
    */

    std::size_t m_n;
    std::random_device m_rd;
    std::mt19937 m_gen;
    T m_min, m_max;
};

} // ns common

#endif // RANDOMDATA_H
