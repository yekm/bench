#include "timemeasurement.hpp"

#include "bexeption.hpp"

namespace utils
{

TimeMeasurement::TimeMeasurement(std::function<void()> f)
{
    try
    {
        utils::Timer timer;
        f();
        m_d = timer.get();
    }
    catch (const BExeption & e)
    {
        m_s.set_status(Status::SE_ERROR, e.what());
    }
    catch (const std::bad_alloc & e)
    {
        m_s.set_status(Status::SE_OOM);
    }
    catch (...)
    {
        m_s.set_status(Status::SE_UNKNOWN_EXCEPTION);
    }
}

const TimeMeasurement & TimeMeasurement::get_time(Timer::timediff_type & d) const
{
    d = m_d;
    return *this;
}

const TimeMeasurement & TimeMeasurement::get_status(Status & s) const
{
    s = m_s;
    return *this;
}

const TimeMeasurement & TimeMeasurement::set_timeout(Timer::timediff_type t) const
{
    if (m_s.get_status() == Status::SE_OK && m_d > t)
        m_s.set_status(Status::SE_TIMEOUT);

    return *this;
}

} // ns utils
