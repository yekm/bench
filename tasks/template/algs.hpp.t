#ifndef %UTN%_ALGS_H
#define %UTN%_ALGS_H

#include "algorithm.hpp"
#include "%LTN%_task.hpp"
//#include "utils/dbg.hpp"


class alg : public Algorithm
{
public:
    alg()
        : Algorithm("Template Alg")
    {}
private:
    virtual std::string do_complexity() override
    {
        return "";
    }
    virtual void do_run(TaskData & td, std::unique_ptr<AResult> &) override
    {
        %TN%::g_type::container_type &d = static_cast<%TN%::g_type&>(td).get_mutable();
        D() << "before " << d;
        D() << "after " << d;
    }
};

#endif // %UTN%_ALGS_H
