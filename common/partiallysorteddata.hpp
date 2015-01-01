#ifndef SORTEDDATA_HPP
#define SORTEDDATA_HPP

#include "genericdata.hpp"
#include "utils/not_so_random_device.hpp"

#include <limits>
#include <algorithm>
#include <random>

namespace common
{

template <typename T>
class PartiallySortedData : public GenericData<std::vector<T>>
{
public:
    typedef GenericData<std::vector<T>> base_type;

    explicit PartiallySortedData(std::size_t n, std::size_t shuffles, T min = 0)
        : base_type("sorted data " + range_str(min, min+n), n)
        , m_rd()
        , m_gen(m_rd())
        , m_min(min)
        , m_max(min+n)
        , m_shuffles(shuffles)
    {
        resize_and_shuffle();
        D() << "created PartiallySortedData" << base_type::m_data;
    }

    PartiallySortedData (const PartiallySortedData & o)
        : base_type(o.get_name(), o.get_n())
        , m_rd()
        , m_gen(o.m_gen)
        , m_min(o.m_min)
        , m_max(o.m_max)
        , m_shuffles(o.m_shuffles)
    {
        resize_and_shuffle();
        D() << "created copy PartiallySortedData" << base_type::m_data;
    }

    virtual std::shared_ptr<TaskData> clone_copy() const override
    {
        return std::make_shared<PartiallySortedData<T>>(*this);
    }

protected:
    std::string range_str(T min, T max) const
    {
        return "[" + std::to_string(min) + ", " + std::to_string(max) + "]";
    }

private:
    void resize_and_shuffle()
    {
        base_type::m_data.resize(base_type::get_n());

        std::size_t min = m_min;
        std::generate(base_type::m_data.begin(), base_type::m_data.end(), [&]{ return min++; });

        std::uniform_int_distribution<std::size_t> dis(0, base_type::get_n());
        std::size_t n_shuffles = m_shuffles;
        while (n_shuffles-- != 0)
        {
            std::swap(base_type::m_data[dis(m_gen)], base_type::m_data[dis(m_gen)]);
        }
    }

    utils::not_so_random_device m_rd;
    std::mt19937 m_gen;
    const T m_min, m_max;
    const double m_shuffles;
};

} // ns common


#endif // SORTEDDATA_HPP
