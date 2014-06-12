#include "taskdata.hpp"

TaskData::TaskData(const std::string & name, std::size_t n)
    : m_name(name)
    , m_n(n)
{
}

TaskData::~TaskData()
{
}

std::string TaskData::get_name() const
{
    return m_name;
}

std::size_t TaskData::get_n() const
{
    return m_n;
}
