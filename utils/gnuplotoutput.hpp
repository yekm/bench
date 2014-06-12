#ifndef GNUPLOTOUTPUT_H
#define GNUPLOTOUTPUT_H

#include <fstream>
#include <string>

namespace utils
{

class GnuplotOutput
{
public:
    GnuplotOutput();
    explicit GnuplotOutput(const std::string &);
    void write();
private:
    std::ofstream m_output_file;
    std::ostream m_output;
};

} // utils

#endif // GNUPLOTOUTPUT_H
