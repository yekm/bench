#ifndef TSVOUTPUT_HPP
#define TSVOUTPUT_HPP

#include "dsvoutput.hpp"

namespace utils
{

class TsvOutput : DsvOutput
{
public:
    TsvOutput();
    explicit TsvOutput(const std::string &);
    void write(const AlgorithmStat::timestat_type &);
};

} // utils

#endif // TSVOUTPUT_HPP
