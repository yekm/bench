#include "randomdata.h"
#include <random>
#include <algorithm>

namespace common
{

template <>
void RandomData<int>::generate(int min, int max)
{
    std::uniform_int_distribution<int> dis(min, max);
    std::generate_n(m_data.begin(), m_data.size(), std::bind(dis, m_gen));
}

template <>
void RandomData<char>::generate(char min, char max)
{
    std::uniform_int_distribution<char> dis(min, max);
    std::generate_n(m_data.begin(), m_data.size(), std::bind(dis, m_gen));
}

} // ns common
