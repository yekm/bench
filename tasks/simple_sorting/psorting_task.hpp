#ifndef PSORTING_TASK_HPP
#define PSORTING_TASK_HPP

#include "sorting_task.hpp"
#include "common/partiallysorteddata.hpp"

class PSortingTask : public SortingTask
{
public:
    static constexpr std::size_t N = 1e6;

    PSortingTask()
        : SortingTask("Sorting algorithms, partially sorted data, " + std::to_string(N) + " elements",
                      utils::PlotSettings(utils::PlotSettings::AS_LOGXY,
                                          "number of shuffles"))
    {
    }

    virtual std::shared_ptr<TaskData> prepare_data(std::size_t n)
    {
        return std::make_shared<common::PartiallySortedData<SortingTask::item_type>>(N, n);
    }

    virtual bool get_n(std::size_t & n)
    {
        n *= 2;

        if (n > N*8)
            return false;

        return true;
    }
};

#endif // PSORTING_TASK_HPP
