#ifndef DSVOUTPUT_HPP
#define DSVOUTPUT_HPP

#include "basicoutput.hpp"
#include "algorithmstat.hpp"

namespace utils
{

class DsvOutput : public BasicOutput
{
public:
    DsvOutput(const std::string & delimiter);
    explicit DsvOutput(const std::string & filename, const std::string & delimiter);
    void write(const AlgorithmStat::timestat_type &);
private:
    const std::string m_delimiter;
};

} // utils

#endif // DSVOUTPUT_HPP
