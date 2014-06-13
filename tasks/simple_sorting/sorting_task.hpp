#ifndef SORTING_TASK_H
#define SORTING_TASK_H

#include "task.hpp"
#include "common/quickrandomdata.hpp"
#include "bexeption.hpp"

class SortingTask : public Task
{
public:
    typedef int item_type;
    //typedef char item_type; // nice for debugging
    typedef GenericData<std::vector<item_type>> g_type;

    SortingTask()
        : Task("sorting algorithms")
    {}

    virtual std::shared_ptr<TaskData> prepare_data(std::size_t n)
    {
        return std::make_shared<common::RandomData<item_type>>(n);
        //return std::make_shared<QuickRandomData<item_type>>(n);
    }

    virtual bool get_n(std::size_t & n)
    {
        n *= 2;
        return true;
    }

    virtual void check(const TaskData & td)
    {
        D() << "check";
        auto &d = static_cast<const g_type&>(td).get_const();
        if (std::is_sorted(d.cbegin(), d.cend()) != true)
            throw BExeption("sorting check failed");
    }
};

#endif // SORTING_TASK_H
