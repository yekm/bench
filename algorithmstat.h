#ifndef ALGORITHMSTAT_H
#define ALGORITHMSTAT_H

#include <map>

#include "utils/stat.h"
#include "utils/status.h"

struct AlgorithmStat
{
public:
    std::map<std::size_t, utils::Stat> m_stat_run, m_stat_prepare;
    utils::Status m_status;
};

#endif // ALGORITHMSTAT_H
