#ifndef ALGORITHMSTAT_H
#define ALGORITHMSTAT_H

#include <map>

#include "utils/stat.hpp"
#include "utils/status.hpp"

struct AlgorithmStat
{
public:
    typedef std::map<std::size_t, utils::Stat> timestat_type;
    timestat_type m_stat_run, m_stat_prepare;
    utils::Status m_status;
};

#endif // ALGORITHMSTAT_H
