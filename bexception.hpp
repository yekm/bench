#ifndef BEXCEPTION_H
#define BEXCEPTION_H

#include <stdexcept>

class BException : public std::runtime_error {
public:
    BException(const std::string & wut)
        : std::runtime_error(wut)
    {}
};

#endif // BEXCEPTION_H
