#ifndef BEXEPTION_H
#define BEXEPTION_H

#include <stdexcept>

class BExeption : public std::runtime_error {
public:
    BExeption(const std::string & wut)
        : std::runtime_error(wut)
    {}
};

#endif // BEXEPTION_H
