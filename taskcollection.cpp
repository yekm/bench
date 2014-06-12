#include "taskcollection.hpp"

void TaskCollection::add_task(const std::string & desc, tcollection::mapped_type t)
{
    m_tasks[desc] = std::move(t);
}
