#ifndef RANDOMDATA_H
#define RANDOMDATA_H

#include <vector>
#include <limits>
#include "genericdata.h"

#include <random>
#include <algorithm>

namespace common
{

template <typename T>
class RandomData : public GenericData<std::vector<T>>
{
public:
    typedef GenericData<std::vector<T>> base_type;

    explicit RandomData(std::size_t n)
        : base_type("uniform random", n)
        , m_n(n)
        , m_rd()
        , m_gen(m_rd())
    {
        base_type::m_data.resize(m_n);
        generate(std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
    }

    RandomData (const RandomData & o)
        : base_type("uniform random", o.m_n)
        , m_n(o.m_n)
        , m_rd()
        , m_gen(o.m_gen)
    {
        base_type::m_data.resize(m_n);
        generate(std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
    }

    virtual std::shared_ptr<TaskData> clone_copy() const
    {
        return std::make_shared<RandomData<T>>(*this);
        //return std::make_shared<RandomData<T>>(m_n);
    }

protected:

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
};

} // ns common

#endif // RANDOMDATA_H
