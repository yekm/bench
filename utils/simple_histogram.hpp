#ifndef SIMPLE_HISTOGRAM_HPP
#define SIMPLE_HISTOGRAM_HPP

#include <map>
#include <vector>
#include <limits>

#include "utils/dbg.hpp"

namespace utils
{

template <typename T>
std::map<T, size_t> simple_histogram(
        const std::vector<T> & v,
        int bins,
        T min = std::numeric_limits<T>::lowest(),
        T max = std::numeric_limits<T>::max()
        )
{
    auto bin_step = std::abs(max/bins-min/bins);
    D() << "bin_step" << bin_step << "len" << min-max << "min" << min << "max" << max;

    std::map<T, size_t> histogram;

    T bin = min;
    for (int i=0; i<bins-1; ++i)
    {
        bin += bin_step;
        histogram[bin] = 0;
    }
    histogram[max] = 0;

    for (const auto &x : v)
    {
        typedef typename std::map<T, size_t>::value_type vtype;
        auto lb = std::lower_bound(histogram.begin(), histogram.end(), x,
                                   [](const vtype &e1, const T &e2)
                                   { return e1.first < e2; }
                                   );
        if (lb == histogram.end())
            E() << "value" << x << "out of histogram bins bounds.";
        lb->second++;
    }

    return histogram;
}

template <typename T>
void draw_histogram(const std::map<T, size_t> & m, size_t width = 100)
{
    typedef typename std::map<T, size_t>::value_type vtype;
    T max = std::max_element(
                m.begin(),
                m.end(),
                [](const vtype &e1, const vtype &e2){ return e1.second < e2.second; }
    )->second;

    std::cout << std::endl;

    for (const auto &e : m)
    {
        auto sample = e.second;
        auto w = std::cout.width();
        std::cout.width(12);
        std::cout << std::left << e.first;
        std::cout.width(w);
        std::cout << ":";
        std::cout.width(12);
        std::cout << sample;
        std::cout.width(w);
        for (int i=0; i<(float)sample/max*width; ++i)
            std::cout << "*";
        std::cout << std::endl;
    }

    std::cout << std::endl;
}

} // ns utils

#endif // SIMPLE_HISTOGRAM_HPP
