#ifndef BLOCK_CIPHER_H
#define BLOCK_CIPHER_H

#include "task.hpp"
#include "common/randomdata.hpp"
#include "bexception.hpp"

class block_cipher : public Task
{
public:
    typedef unsigned char item_type;
    typedef GenericData<std::vector<item_type>> g_type;

    block_cipher()
        : Task("Block ciphers, 1MB of random data.",
               utils::PlotSettings(utils::PlotSettings::AxisScale::AS_LINEAR))
    {
        get_plotsettings().set_plot_type(utils::PlotSettings::PlotType::PT_BARS);
    }

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

    virtual bool get_n(std::size_t & n) override
    {
        if (n<8)
        {
            n = 1*1024*1024; // 1 MB
            return true;
        }
        return false;
    }

};

#endif // BLOCK_CIPHER_H
