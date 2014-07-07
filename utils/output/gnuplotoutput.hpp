#ifndef GNUPLOTOUTPUT_H
#define GNUPLOTOUTPUT_H

#include "basicoutput.hpp"

namespace utils
{

class GnuplotOutput : BasicOutput
{
public:
    GnuplotOutput();
    explicit GnuplotOutput(const std::string &);
    void write();
};

} // utils

#endif // GNUPLOTOUTPUT_H
