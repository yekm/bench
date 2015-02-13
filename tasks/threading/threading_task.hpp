#ifndef THREADING_H
#define THREADING_H

#include "task.hpp"
#include "common/randomdata.hpp"
#include "bexception.hpp"

class Threading : public Task
{
public:
    typedef int item_type;
    //typedef char item_type; // nice for debugging
    typedef GenericData<std::vector<item_type>> g_type;
    typedef SimpleResult<std::size_t> result_type;

    class CustomRandomData : public common::RandomData<item_type>
    {
    public:
        typedef common::RandomData<item_type> base_type;
        CustomRandomData(std::size_t n)
            : base_type(1e7)
            , m_num_threads(n)
        {}
        std::size_t get_num_threads()
        {
            return m_num_threads;
        }
        virtual std::shared_ptr<TaskData> clone_copy() const override
        {
            return std::make_shared<CustomRandomData>(*this);
        }
    private:
        std::size_t m_num_threads;
    };

    Threading(const std::string & name)
        : Task(name, utils::PlotSettings(utils::PlotSettings::AS_LINEAR))
    {}

    virtual std::shared_ptr<TaskData> prepare_data(std::size_t n) override
    {
        return std::make_shared<CustomRandomData>(n);
    }

    virtual bool get_n(std::size_t & n) override
    {
        ++n;
        if (n > 32)
            return false;
        return true;
    }

};

class ThreadedCountingTask : public Threading
{
public:
    ThreadedCountingTask(const std::string & add)
        : Threading("Threaded counting, 1e7 elements (" + add + ")")
    {}

    virtual void validate(const AResult & ares) override
    {
        const Threading::result_type & result =
                static_cast<const Threading::result_type&>(ares.get_custom_result());
        auto &d = static_cast<const g_type&>(ares.get_taskdata()).get_const();
        std::size_t true_count = std::count_if(d.begin(), d.end(),
                      [](const Threading::item_type & e) { return e%2 == 0; } );
        D() << "true_count" << true_count << ", result" << result.m_simple_result;
        if (true_count != result.m_simple_result)
            throw BException("check failed");
    }
};

#endif // THREADING_H
