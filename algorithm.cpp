#include "algorithm.h"

Algorithm::Algorithm(const std::string & name)
    : m_name(name)
{
}

Algorithm::~Algorithm()
{
}

void Algorithm::prepare(const TaskData &)
{
}

void Algorithm::check(const TaskData &)
{
}

std::string Algorithm::get_name()
{
    return m_name;
}
