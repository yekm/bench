#ifndef JSONOUTPUT_HPP
#define JSONOUTPUT_HPP

#include "basicoutput.hpp"

namespace utils
{

// used by d3

class JsonOutput : BasicOutput
{
public:
    JsonOutput();
    explicit JsonOutput(const std::string &);
    void write();
};

#endif // JSONOUTPUT_HPP

} // utils
