#ifndef TASKCOLLECTION_H
#define TASKCOLLECTION_H

#include <map>
#include <memory>
#include "task.h"

class TaskCollection
{
public:
    typedef std::map<std::string, std::unique_ptr<Task>> tcollection;
    static TaskCollection & get()
    {
        static TaskCollection s;
        return s;
    }

    void add_task(const std::string &, tcollection::mapped_type);
    tcollection::const_iterator begin()
    {
        return m_tasks.cbegin();
    }
    tcollection::const_iterator end()
    {
        return m_tasks.cend();
    }
    std::size_t size()
    {
        return m_tasks.size();
    }

    TaskCollection(const TaskCollection&) = delete;
    TaskCollection(TaskCollection&&) = delete;
    TaskCollection& operator=(TaskCollection&) = delete;
private:
    TaskCollection()
    {
    }

    tcollection m_tasks;
};

#endif // TASKCOLLECTION_H
