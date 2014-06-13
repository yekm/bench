#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <string>
#include "taskdata.hpp"
#include "algorithmstat.hpp"
#include "aresult.hpp"

#include "utils/stat.hpp"

class Algorithm
{
public:
    explicit Algorithm(const std::string &);
    Algorithm(Algorithm &&) = default;
    Algorithm(const Algorithm &) = delete;
    virtual ~Algorithm() = 0;
    void prepare(const TaskData &);
    std::unique_ptr<AResult> run(std::shared_ptr<TaskData>);
    std::string complexity();
    std::string get_name();
    AlgorithmStat m_statistics;
private:
    virtual void do_prepare(const TaskData &);
    virtual void do_run(TaskData &, std::unique_ptr<AResult> &) = 0;
    virtual std::string do_complexity();
    const std::string m_name;
};

#endif // ALGORITHM_H
