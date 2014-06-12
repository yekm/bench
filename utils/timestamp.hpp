#ifndef TIMESTAMP_H
#define TIMESTAMP_H

#include <time.h>

namespace utils
{

class TimeStamp
{
public:
    TimeStamp();
    double operator-(const TimeStamp &);
private:
    timespec m_ts;
};

} // ns utils

#endif // TIMESTAMP_H
