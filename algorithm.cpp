#include "algorithm.hpp"

Algorithm::Algorithm(const std::string & name)
    : m_name(name)
{
}

Algorithm::~Algorithm()
{
}

void Algorithm::prepare(const TaskData & td)
{
    do_prepare(td);
}

void Algorithm::do_prepare(const TaskData &)
{
}

std::unique_ptr<AResult> Algorithm::run(std::shared_ptr<TaskData> td)
{
    std::unique_ptr<AResult> res(new AResult(td));
    do_run(*td, res);
    return res;
}

std::string Algorithm::complexity()
{
    return do_complexity();
}

std::string Algorithm::do_complexity()
{
    return std::string();
}

std::string Algorithm::get_name()
{
    return m_name;
}
