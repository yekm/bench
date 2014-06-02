#include "timestamp.h"

namespace utils
{

TimeStamp::TimeStamp()
{
    clock_gettime(CLOCK_MONOTONIC_RAW, &m_ts);
}

double TimeStamp::operator - (const TimeStamp & o)
{
    return m_ts.tv_sec - o.m_ts.tv_sec + (m_ts.tv_nsec - o.m_ts.tv_nsec) / 1e9;
}

} // ns utils
