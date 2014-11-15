#ifndef SIMPLE_HISTOGRAM_HPP
#define SIMPLE_HISTOGRAM_HPP

#include <map>
#include <vector>
#include <limits>

#include "utils/dbg.hpp"

namespace utils
{

template <typename T>
std::map<T, size_t> simple_histogram(const std::vector<T> & v, size_t bins)
{
    long int min = std::numeric_limits<T>::min();
    long int max = std::numeric_limits<T>::max();
    long int len = max-min;
    size_t bin_step = len/bins;
    D() << "bin_step" << bin_step << "len" << len << "min" << min << "max" << max;

    std::map<T, size_t> histogram;

    long int bin = min;
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
        histogram[lb->first] += 1;
    }

    return histogram;
}

template <typename T>
void draw_histogram(const std::map<T, size_t> & m, size_t width = 100)
{
    typedef typename std::map<T, size_t>::value_type vtype;
    size_t max = std::max_element(
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
        std::cout << e.first;
        std::cout.width(w);
        std::cout << ":";
        std::cout.width(12);
        std::cout << std::left << sample;
        std::cout.width(w);
        for (int i=0; i<(float)sample/max*width; ++i)
            std::cout << "*";
        std::cout << std::endl;
    }

    std::cout << std::endl;
}

} // ns utils

#endif // SIMPLE_HISTOGRAM_HPP
