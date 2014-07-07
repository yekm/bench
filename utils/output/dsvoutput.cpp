#include "dsvoutput.hpp"

namespace utils
{

DsvOutput::DsvOutput(const std::string &delimiter)
    : m_delimiter(delimiter)
{

}

DsvOutput::DsvOutput(const std::string &filename, const std::string &delimiter)
    : BasicOutput(filename)
    , m_delimiter(delimiter)
{
}

void DsvOutput::write(const AlgorithmStat::timestat_type & ts)
{
    for (const auto & x : ts)
    {
        const utils::Stat & s = x.second;
        m_output << x.first << m_delimiter
                 << s.mean() << m_delimiter
                 << s.stddev() << m_delimiter
                 << s.min() << m_delimiter
                 << s.max() << "\n";
    }
}



} // utils
