#include "basicoutput.hpp"

#include <iostream>

namespace utils
{

BasicOutput::BasicOutput()
    : m_output(std::cout.rdbuf())
{

}

BasicOutput::BasicOutput(const std::string & filename)
    : m_output_file(filename.c_str())
    , m_output(m_output_file.rdbuf())
{
}

} // ns utils
