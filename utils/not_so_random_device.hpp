#ifndef NOT_SO_RANDOM_DEVICE_HPP
#define NOT_SO_RANDOM_DEVICE_HPP

#include <random>

namespace utils
{

class not_so_random_device
{
public:
    not_so_random_device();
    std::random_device::result_type operator()();
};

} // ns utils

#endif // NOT_SO_RANDOM_DEVICE_HPP
