#include "not_so_random_device.hpp"

#include <limits>

#include "dbg.hpp"

namespace utils
{

class StaticRandomDevice
{
public:
    typedef std::random_device::result_type r_type;

    static StaticRandomDevice & get()
    {
        static StaticRandomDevice rd;
        return rd;
    }

    r_type generate()
    {
        return m_dis(m_gen);
    }

private:
    StaticRandomDevice()
        : m_rd()
        , m_gen(m_rd())
        , m_dis(std::numeric_limits<r_type>::min(), std::numeric_limits<r_type>::max())
    {
        I() << "StaticRandomDevice init";
    }

    std::random_device m_rd;
    std::mt19937 m_gen;
    std::uniform_int_distribution<r_type> m_dis;
};

not_so_random_device::not_so_random_device()
{
}

std::random_device::result_type not_so_random_device::operator()()
{
    return StaticRandomDevice::get().generate();
}



} // ns utils
