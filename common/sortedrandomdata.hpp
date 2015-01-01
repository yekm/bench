#ifndef SORTEDRANDOMDATA_HPP
#define SORTEDRANDOMDATA_HPP

#include "randomdata.hpp"

namespace common
{

template <typename T>
class SortedRandomData : public RandomData<T>
{
public:
    typedef typename RandomData<T>::base_type base_type;

    explicit SortedRandomData(std::size_t n,
                              T min = std::numeric_limits<T>::min(),
                              T max = std::numeric_limits<T>::max())
        : RandomData<T>("sorted random data " + range_str(min, max), n, min, max)
    {
        std::sort(base_type::m_data.begin(), base_type::m_data.end());
    }

    SortedRandomData (const SortedRandomData & o)
        : RandomData<T>(o)
    {
        std::sort(base_type::m_data.begin(), base_type::m_data.end());
    }

    virtual std::shared_ptr<TaskData> clone_copy() const override
    {
        return std::make_shared<SortedRandomData<T>>(*this);
    }
};

} // ns common

#endif // SORTEDRANDOMDATA_HPP
