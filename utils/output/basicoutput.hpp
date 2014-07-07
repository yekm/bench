#ifndef BASICOUTPUT_HPP
#define BASICOUTPUT_HPP

#include <fstream>
#include <string>

// for join
#include <sstream>

namespace utils
{

class BasicOutput
{
public:
    BasicOutput();
    explicit BasicOutput(const std::string &);
private:
    std::ofstream m_output_file;
protected:
    std::ostream m_output;
};


// move to separate header?
template <typename T>
inline std::string join(T & v, std::string sep = ", ")
{
    std::stringstream ss;
    for(std::size_t i = 0; i < v.size(); ++i)
    {
        if(i != 0)
            ss << sep;
        ss << v[i];
    }
    return ss.str();
}

} // ns utils

#endif // BASICOUTPUT_HPP
