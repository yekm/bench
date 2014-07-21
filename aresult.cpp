#include "aresult.hpp"

AResult::AResult()
{
}

AResult::AResult(std::shared_ptr<TaskData> td)
    : m_td(td)
{
}

AResult::AResult(const AResult & a)
    : m_td(a.m_td)
{
}

const TaskData & AResult::get_taskdata() const
{
    return *m_td;
}

std::shared_ptr<TaskData> AResult::get_ptr() const
{
    return m_td;
}

AResult::~AResult()
{
}
