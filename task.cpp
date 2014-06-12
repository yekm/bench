#include "task.hpp"

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
    auto &alg = m_algs[a->get_name()];
    alg = std::move(a);
}

bool Task::algorithms_ok()
{
    for (const auto & a : m_algs)
    {
        if (a.second->m_statistics.m_status.ok())
            return true;
    }
    return false;
}
