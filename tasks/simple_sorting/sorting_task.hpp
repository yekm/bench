#ifndef SORTING_TASK_H
#define SORTING_TASK_H

#include "task.hpp"
#include "common/quickrandomdata.hpp"
#include "common/partiallysorteddata.hpp"
#include "bexception.hpp"

//#include <boost/align/aligned_allocator.hpp>
//namespace ba = boost::alignment;

class SortingTask : public Task
{
public:
    typedef int item_type;
    //typedef char item_type; // nice for debugging
    /*
    typedef std::vector<item_type,
                        ba::aligned_allocator<item_type,
                                              64>> vector_type;
    */
    typedef GenericData<std::vector<item_type>> g_type;

    SortingTask()
        : Task("sorting algorithms", utils::PlotSettings(utils::PlotSettings::AxisScale::AS_LOGXY))
    {}

    virtual std::shared_ptr<TaskData> prepare_data(std::size_t n) override
    {
        return std::make_shared<common::RandomData<item_type>>(n);
        //return std::make_shared<QuickRandomData<item_type>>(n);
    }

    virtual void validate(const AResult & ares) override
    {
        auto &d = static_cast<const g_type&>(ares.get_taskdata()).get_const();
        if (std::is_sorted(d.cbegin(), d.cend()) != true)
            throw BException("sorting check failed");
    }
protected:
    SortingTask(const std::string & name, utils::PlotSettings s)
        : Task(name, s)
    {}
};

#endif // SORTING_TASK_H
