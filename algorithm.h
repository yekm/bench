#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <string>
#include <stdexcept>
#include "taskdata.h"

#include "utils/stat.h"

class Algorithm
{
public:
    class AlgExeption : public std::runtime_error {
    public:
        AlgExeption(const std::string & wut)
            : std::runtime_error(wut)
        {}
    };

    explicit Algorithm(const std::string &);
    Algorithm(Algorithm &&) = default;
    Algorithm(const Algorithm &) = delete;
    virtual ~Algorithm() = 0;
    virtual void prepare(const TaskData &);
    virtual void run(TaskData &) = 0;
    virtual void check(const TaskData &);
    virtual std::string complexity();
    std::string get_name();
private:
    const std::string m_name;
};

#endif // ALGORITHM_H
