#ifndef POPCNT_HPP
#define POPCNT_HPP

#include "task.hpp"
#include "common/randomdata.hpp"
#include "bexception.hpp"

struct PResult : CustomResult
{
    size_t count;
};

class Popcnt : public Task
{
public:
    typedef int item_type;
    typedef GenericData<std::vector<item_type>> g_type;

    Popcnt()
        : Task("popcnt perfomance", utils::PlotSettings(utils::PlotSettings::AS_LOGXY))
    {}

    virtual std::shared_ptr<TaskData> prepare_data(std::size_t n)
    {
        return std::make_shared<common::RandomData<item_type>>(n);
    }

    virtual bool get_n(std::size_t & n)
    {
        if (n < 2)
        {
            n = 1024;
            return true;
        }

        if (n > 6e8)
        {
            E() << "ololo" << n;
            return false;
        }

        n *= 2;
        return true;
    }

    virtual void validate(const AResult & ares)
    {
        const size_t pcount = static_cast<const PResult&>(ares.get_custom_result()).count;
        auto &d = static_cast<const g_type&>(ares.get_taskdata()).get_const();
        size_t count = 0;
        for (uint32_t i : d)
        {
            i = i - ((i >> 1) & 0x55555555);
            i = (i & 0x33333333) + ((i >> 2) & 0x33333333);
            count += (((i + (i >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
        }
        if (count != pcount)
            throw BException("popcount check failed");
    }
};

#endif // POPCNT_HPP
