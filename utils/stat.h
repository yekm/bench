#ifndef STAT_H
#define STAT_H

#include <vector>
#include <algorithm>

namespace utils
{

template <typename T>
class StatT
{
public:
    StatT()
        : m_max(std::numeric_limits<T>::min())
        , m_min(std::numeric_limits<T>::max())
    {}

    void add(T t)
    {
        m_data.push_back(t);
        if (m_max < t)
            m_max = t;
        if (m_min > t)
            m_min = t;
    }

    T min() const
    {
        return m_min;
    }

    T max() const
    {
        return m_max;
    }

    T sum() const
    {
        return std::accumulate(m_data.begin(), m_data.end(), .0);
    }

    T mean() const
    {
        return sum() / m_data.size();
    }

    T variance() const
    {
        T m = mean();
        T v = 0;
        for (const T & t : m_data)
        {
            v += (t-m)*(t-m);
        }
        return v / m_data.size();
    }

    T stddev() const
    {
        return sqrt(variance());
    }

private:
    std::vector<T> m_data;
    T m_max, m_min;
};


// actually Timer::timediff_type. include timer.h?
// or move into timemeasurement.h
typedef StatT<double> Stat;

} // ns utils

#endif // STAT_H
