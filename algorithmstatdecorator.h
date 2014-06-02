#ifndef ALGORITHMSTATDECORATOR_H
#define ALGORITHMSTATDECORATOR_H

#include "algorithm.h"
#include "utils/stat.h"

#include <map>

#include "utils/gnuplotoutput.h"

class AlgorithmStatDecorator : public Algorithm
{
public:
    enum AlgorithmStatus
    {
        AS_OK,
        AS_ERROR,
        AS_EXEPTION,
        AS_OOM,
        AS_TIMEOUT,
    };

    AlgorithmStatDecorator(std::unique_ptr<Algorithm> &&);
    virtual void prepare(const TaskData &);
    virtual void run(TaskData &);
    virtual void check(const TaskData &);
    AlgorithmStatus get_status();
private:
    std::unique_ptr<Algorithm> m_alg;
    std::map<std::size_t, utils::Stat> m_stat_run, m_stat_prepare;
    AlgorithmStatus m_status;
    std::string m_error_sring;

    friend class utils::GnuplotOutput;
};

// TODO: sober up and rewrite

#endif // ALGORITHMSTATDECORATOR_H
