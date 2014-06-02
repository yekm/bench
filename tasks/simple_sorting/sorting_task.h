#ifndef SORTING_TASK_H
#define SORTING_TASK_H

#include "task.h"
#include "common/quickrandomdata.h"

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

    virtual std::pair<std::size_t, std::size_t> get_n()
    {
        return std::make_pair<std::size_t, std::size_t>(2, 5e8);
    }
};

#endif // SORTING_TASK_H
