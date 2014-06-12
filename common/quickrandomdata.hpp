#ifndef QUICKRANDOMDATA_H
#define QUICKRANDOMDATA_H

#include "randomdata.hpp"

// works 2 times faster

namespace common
{

template <typename T>
class QuickRandomData : public RandomData<T>
{
public:
    explicit QuickRandomData(std::size_t n)
        : RandomData<T>(n)
    {
        m_data2 = this->m_data;
    }

    QuickRandomData(const QuickRandomData & r)
        : RandomData<T>(0)
    {
        if (r.m_data.size() > r.m_data2.size())
            this->m_data = r.m_data;
        else
            this->m_data = r.m_data2;
    }

    virtual std::shared_ptr<TaskData> clone_copy() const
    {
        return std::make_shared<QuickRandomData<T>>(*this);
    }

private:
    std::vector<T> m_data2;
};

} // ns common

#endif // QUICKRANDOMDATA_H
