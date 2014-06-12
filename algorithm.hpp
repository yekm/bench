#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <string>
#include "taskdata.hpp"
#include "algorithmstat.hpp"

#include "utils/stat.hpp"

class Algorithm
{
public:
    explicit Algorithm(const std::string &);
    Algorithm(Algorithm &&) = default;
    Algorithm(const Algorithm &) = delete;
    virtual ~Algorithm() = 0;
    virtual void prepare(const TaskData &);
    virtual void run(TaskData &) = 0;
    virtual void check(const TaskData &);
    virtual std::string complexity();
    std::string get_name();
    AlgorithmStat m_statistics;
private:
    const std::string m_name;
};

#endif // ALGORITHM_H
