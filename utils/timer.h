#ifndef TIMER_H
#define TIMER_H

#include "timestamp.h"

namespace utils
{

class Timer
{
public:
    typedef double timediff_type;
    Timer();
    timediff_type get();
    timediff_type reset();
    timediff_type since(const Timer &);
private:
    TimeStamp m_ts;
};

} // ns utilss

#endif // TIMER_H
