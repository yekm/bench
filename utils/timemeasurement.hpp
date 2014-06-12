#ifndef TIMEMEASUREMENT_H
#define TIMEMEASUREMENT_H

#include <string>
#include <functional>

#include "timer.hpp"
#include "status.hpp"

namespace utils
{

class TimeMeasurement
{
public:
    TimeMeasurement(std::function<void()> f);
    const TimeMeasurement & get_time(Timer::timediff_type & d) const;
    const TimeMeasurement & get_status(Status & s) const;
    const TimeMeasurement & set_timeout(Timer::timediff_type) const;

private:
    Timer::timediff_type m_d;
    mutable Status m_s;
};

} // ns utils

#endif // TIMEMEASUREMENT_H
