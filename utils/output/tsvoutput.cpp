#include "tsvoutput.hpp"

namespace utils
{

TsvOutput::TsvOutput()
    : DsvOutput("\t")
{
}

TsvOutput::TsvOutput(const std::string & filename)
    : DsvOutput(filename, "\t")
{
}

void TsvOutput::write(const AlgorithmStat::timestat_type & ts)
{
    m_output << "n\tmean\tstddev\tmin\tmax\n";
    DsvOutput::write(ts);
}

} // utils
