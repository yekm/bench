#ifndef TASK_H
#define TASK_H

#include <string>
#include <map>
#include "taskdata.h"
#include "algorithm.h"

#include "utils/status.h"

class Task
{
public:
    typedef std::map<std::string, std::unique_ptr<Algorithm>> algs_type;

    explicit Task(const std::string &);
    Task(Task &&) = default;
    virtual ~Task() = 0;

    virtual std::shared_ptr<TaskData> prepare_data(std::size_t n) = 0;
    algs_type & get_algorithms();
    void add_alg(algs_type::mapped_type);

    std::string get_name() const;

    virtual std::pair<std::size_t, std::size_t> get_n() = 0;

    utils::Status m_status;

protected:
    algs_type m_algs;

private:
    const std::string m_name;
};

#endif // TASK_H
