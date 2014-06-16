#ifndef QUICKRANDOMDATA_H
#define QUICKRANDOMDATA_H

#include "randomdata.hpp"
#include <cassert>

// works 2 times faster

namespace common
{

template <typename T>
class QuickRandomData : public RandomData<T>
{
public:
    typedef typename RandomData<T>::base_type base_type;

    explicit QuickRandomData(std::size_t n,
                             T min = std::numeric_limits<T>::min(),
                             T max = std::numeric_limits<T>::max())
        : RandomData<T>("quick random data " + range_str(min, max), n, min, max)
        , m_data_original(base_type::m_data)
    {
    }

    QuickRandomData(const QuickRandomData & o)
        : RandomData<T>("copy of " + o.get_name(), 0, o.min(), o.max())
    {
        /* Instead of generating random data again we copying it from m_data2
         * which contains unchanged data
         */
        base_type::m_data = o.m_data_original;
        /* We are copying from *one* parent TaskData, so where there no
         * copy of copy of random data. Thus we can leave just like this.
         * But actually m_data2 should be copied too
         */
        //base_type::m_data2 = o.m_data2;
        assert(o.m_data_original.size());
    }

    virtual std::shared_ptr<TaskData> clone_copy() const
    {
        return std::make_shared<QuickRandomData<T>>(*this);
    }

private:
    const std::vector<T> m_data_original;
};

} // ns common

#endif // QUICKRANDOMDATA_H
