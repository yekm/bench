#include "task.hpp"

Task::Task(const std::string &name, utils::PlotSettings s)
    : m_name(name)
    , m_plotsettings(s)
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

/// TODO: wtf is this?
bool Task::algorithms_ok()
{
    for (const auto & a : m_algs)
    {
        if (a.second->m_statistics.m_status.ok())
            return true;
    }
    return false;
}

utils::PlotSettings & Task::get_plotsettings()
{
    return m_plotsettings;
}

void Task::validate(const AResult &)
{
}

bool Task::get_n(std::size_t & n)
{
    n < 2 ? n=2 : n*=2;
    return true;
}
