#ifndef DESPACE_H
#define DESPACE_H

#include "bexception.hpp"
#include "common/randomdata.hpp"
#include "task.hpp"

// https://www.reddit.com/r/cpp/comments/5pb5h3/how_quickly_can_you_remove_spaces_from_a_string/

class despace : public Task
{
public:
    typedef char item_type;
    typedef GenericData<std::vector<item_type>> g_type;

    despace()
        : Task("despace",
              utils::PlotSettings(utils::PlotSettings::AxisScale::AS_LOGXY,
                                  "string length"))
    {
    }

    virtual std::shared_ptr<TaskData> prepare_data(std::size_t n) override
    {
        auto data = std::make_shared<common::RandomData<item_type>>(n, '0', 'z');
        auto space_idx = common::RandomData<int>(n/10, 0, n);
        auto d = data->get_mutable();
        for (auto i : space_idx.get_const())
            d.at(i) = ' ';
        d.at(d.size()-1) = '0';
        I() << std::string(d.begin(), d.end());
        return data;
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
        if (n < 2)
            n = 2;
        if (n > 4098)
            return false;
        n *= 2;
        return true;
    }

};

#endif // DESPACE_H
