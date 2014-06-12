#ifndef TIMEMEASUREMENT_H
#define TIMEMEASUREMENT_H

#include <string>
#include <functional>

#include "timer.h"
#include "bexeption.h"
#include "status.h"

namespace utils
{

template <typename R>
class TimeMeasurement
{
public:
    typedef std::function<R()> functor_type;
    TimeMeasurement(functor_type f)
        : m_f(f)
    {}

    R measure(Timer::timediff_type & d, Status & s)
    {
        R r;
        try
        {
            utils::Timer timer;
            r = m_f();
            d = timer.get();
        }
        catch (const BExeption & e)
        {
            s.set_status(Status::SE_ERROR, e.what());
        }
        catch (const std::bad_alloc & e)
        {
            s.set_status(Status::SE_OOM);
        }
        catch (...)
        {
            s.set_status(Status::SE_UNKNOWN_EXCEPTION);
        }
        return r;
    }

private:
    functor_type m_f;
};

template<>
void TimeMeasurement<void>::measure(Timer::timediff_type & d, Status & s)
{
    try
    {
        utils::Timer timer;
        m_f();
        d = timer.get();
    }
    catch (const BExeption & e)
    {
        s.set_status(Status::SE_ERROR, e.what());
    }
    catch (const std::bad_alloc & e)
    {
        s.set_status(Status::SE_OOM);
    }
    catch (...)
    {
        s.set_status(Status::SE_UNKNOWN_EXCEPTION);
    }
}

} // ns utils

#endif // TIMEMEASUREMENT_H
