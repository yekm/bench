#ifndef VECRESULT_HPP
#define VECRESULT_HPP

#include "aresult.hpp"
#include <vector>

class VecResult : public AResult
{
public:
    VecResult(AResult & r, std::size_t _n)
        : AResult(r)
        , n(_n)
    {}

    std::size_t n;
    std::vector<float> lengths;
};

#endif // VECRESULT_HPP
