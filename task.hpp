#ifndef TASK_H
#define TASK_H

#include <string>
#include <map>
#include "taskdata.hpp"
#include "algorithm.hpp"

#include "utils/status.hpp"

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

    virtual bool get_n(std::size_t &) = 0;

    utils::Status m_status;
    bool algorithms_ok();

protected:
    algs_type m_algs;

private:
    const std::string m_name;
};

#endif // TASK_H
