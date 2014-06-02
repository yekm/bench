#include "task.h"

#include "algorithmstatdecorator.h"

Task::Task(const std::string & name)
    : m_name(name)
{
}

Task::~Task()
{
}

std::string Task::get_name() const
{
    return m_name;
}

Task::algs_type & Task::get_algorithms()
{
    return m_algs;
}

void Task::add_alg(algs_type::mapped_type a)
{
//    m_algs[a->get_name()] = std::move(a);
    auto &alg = m_algs[a->get_name()];
    alg = std::unique_ptr<Algorithm>(
                new AlgorithmStatDecorator(std::move(a))
                );
}
