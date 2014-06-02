#include "timer.h"

namespace utils
{

Timer::Timer()
{
}

Timer::timediff_type Timer::get()
{
    return TimeStamp() - m_ts;
}

Timer::timediff_type Timer::reset()
{
    double d = get();
    m_ts = TimeStamp();
    return d;
}

Timer::timediff_type Timer::since(const Timer & s)
{
    return m_ts - s.m_ts;
}

} // ns utils
