#include "aresult.hpp"
#include "utils/dbg.hpp"
#include <stdexcept>

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

const CustomResult &AResult::get_custom_result() const
{
    if (m_result.get() == nullptr)
    {
        E() << "trying to get uninitialisd custom result";
        throw std::runtime_error("trying to get uninitialisd custom result");
    }
    return *m_result;
}

void AResult::set_custom_result(std::unique_ptr<CustomResult> && res)
{
    m_result = std::move(res);
}

AResult::~AResult()
{
}


CustomResult::~CustomResult()
{
}
