#ifndef %UTN%_H
#define %UTN%_H

#include "task.hpp"
#include "common/randomdata.hpp"
#include "bexception.hpp"

class %TN% : public Task
{
public:
    typedef int item_type;
    //typedef char item_type; // nice for debugging
    typedef GenericData<std::vector<item_type>> g_type;

    %TN%()
        : Task("%TN%", utils::PlotSettings(utils::PlotSettings::AS_LOGXY))
    {}

    virtual std::shared_ptr<TaskData> prepare_data(std::size_t n) override
    {
        return std::make_shared<common::RandomData<item_type>>(n);
    }
/*
    virtual void validate(const AResult & ares) override
    {
        auto &d = static_cast<const g_type&>(ares.get_taskdata()).get_const();
        if (false)
            throw BException("sorting check failed");
    }*/
};

#endif // %UTN%_H
