#ifndef DESPACE_ALGS_H
#define DESPACE_ALGS_H

#include "algorithm.hpp"
#include "despace_task.hpp"
//#include "utils/dbg.hpp"
namespace despacer
{
#include "string.h"
#include "despacer/include/despacer.h"
}

class a_memcpy : public Algorithm
{
public:
    a_memcpy()
        : Algorithm("memcpy") {}

private:
    virtual void do_run(TaskData & td, std::unique_ptr<AResult> &) override
    {
        despace::g_type::container_type & d
            = static_cast<despace::g_type &>(td).get_mutable();
        D() << "before " << d;
        char tmp[d.size()];
        despacer::memcpy(tmp, d.data(), d.size());
        D() << "after " << d;
    }
};

class a_remove : public Algorithm
{
public:
    a_remove()
        : Algorithm("std::remove") {}

private:
    virtual void do_run(TaskData & td, std::unique_ptr<AResult> &) override
    {
        despace::g_type::container_type & d
            = static_cast<despace::g_type &>(td).get_mutable();
        D() << "before " << d;
        std::remove_if(d.begin(), d.end(), [](char x){
            return x == ' ' ||
                    x == '\n' ||
                    x == '\r';
        });
        D() << "after " << d;
    }
};

class a_despace : public Algorithm
{
public:
    a_despace()
        : Algorithm("despacer::despace") {}

private:
    virtual void do_run(TaskData & td, std::unique_ptr<AResult> &) override
    {
        despace::g_type::container_type & d
            = static_cast<despace::g_type &>(td).get_mutable();
        D() << "before " << d;
        despacer::despace(d.data(), d.size());
        D() << "after " << d;
    }
};

class a_faster_despace : public Algorithm
{
public:
    a_faster_despace()
        : Algorithm("despacer::faster_despace") {}

private:
    virtual void do_run(TaskData & td, std::unique_ptr<AResult> &) override
    {
        despace::g_type::container_type & d
            = static_cast<despace::g_type &>(td).get_mutable();
        D() << "before " << d;
        despacer::faster_despace(d.data(), d.size());
        D() << "after " << d;
    }
};

class a_despace64 : public Algorithm
{
public:
    a_despace64()
        : Algorithm("despacer::despace64") {}

private:
    virtual void do_run(TaskData & td, std::unique_ptr<AResult> &) override
    {
        despace::g_type::container_type & d
            = static_cast<despace::g_type &>(td).get_mutable();
        D() << "before " << d;
        despacer::despace64(d.data(), d.size());
        D() << "after " << d;
    }
};

class a_avx2_despace : public Algorithm
{
public:
    a_avx2_despace()
        : Algorithm("despacer::avx2_despace") {}

private:
    virtual void do_run(TaskData & td, std::unique_ptr<AResult> &) override
    {
        despace::g_type::container_type & d
            = static_cast<despace::g_type &>(td).get_mutable();
        D() << "before " << d;
        despacer::avx2_despace(d.data(), d.size());
        D() << "after " << d;
    }
};

class a_sse4_despace : public Algorithm
{
public:
    a_sse4_despace()
        : Algorithm("despacer::sse4_despace") {}

private:
    virtual void do_run(TaskData & td, std::unique_ptr<AResult> &) override
    {
        despace::g_type::container_type & d
            = static_cast<despace::g_type &>(td).get_mutable();
        D() << "before " << d;
        despacer::sse4_despace(d.data(), d.size());
        D() << "after " << d;
    }
};

class a_sse4_despace_branchless : public Algorithm
{
public:
    a_sse4_despace_branchless()
        : Algorithm("despacer::sse4_despace_branchless") {}

private:
    virtual void do_run(TaskData & td, std::unique_ptr<AResult> &) override
    {
        despace::g_type::container_type & d
            = static_cast<despace::g_type &>(td).get_mutable();
        D() << "before " << d;
        despacer::sse4_despace_branchless(d.data(), d.size());
        D() << "after " << d;
    }
};

class a_sse4_despace_trail : public Algorithm
{
public:
    a_sse4_despace_trail()
        : Algorithm("despacer::sse4_despace_trail") {}

private:
    virtual void do_run(TaskData & td, std::unique_ptr<AResult> &) override
    {
        despace::g_type::container_type & d
            = static_cast<despace::g_type &>(td).get_mutable();
        D() << "before " << d;
        despacer::sse4_despace_trail(d.data(), d.size());
        D() << "after " << d;
    }
};

class a_sse42_despace : public Algorithm
{
public:
    a_sse42_despace()
        : Algorithm("despacer::sse42_despace") {}

private:
    virtual void do_run(TaskData & td, std::unique_ptr<AResult> &) override
    {
        despace::g_type::container_type & d
            = static_cast<despace::g_type &>(td).get_mutable();
        D() << "before " << d;
        despacer::sse42_despace(d.data(), d.size());
        D() << "after " << d;
    }
};

class a_sse42_despace_branchless : public Algorithm
{
public:
    a_sse42_despace_branchless()
        : Algorithm("despacer::sse42_despace_branchless") {}

private:
    virtual void do_run(TaskData & td, std::unique_ptr<AResult> &) override
    {
        despace::g_type::container_type & d
            = static_cast<despace::g_type &>(td).get_mutable();
        D() << "before " << d;
        despacer::sse42_despace_branchless(d.data(), d.size());
        D() << "after " << d;
    }
};


class a_avx2_countspaces : public Algorithm
{
public:
    a_avx2_countspaces()
        : Algorithm("despacer::avx2_countspaces") {}

private:
    virtual void do_run(TaskData & td, std::unique_ptr<AResult> &) override
    {
        despace::g_type::container_type & d
            = static_cast<despace::g_type &>(td).get_mutable();
        D() << "before " << d;
        despacer::avx2_countspaces(d.data(), d.size());
        D() << "after " << d;
    }
};

class a_sse42_despace_branchless_lookup : public Algorithm
{
public:
    a_sse42_despace_branchless_lookup()
        : Algorithm("despacer::sse42_despace_branchless_lookup") {}

private:
    virtual void do_run(TaskData & td, std::unique_ptr<AResult> &) override
    {
        despace::g_type::container_type & d
            = static_cast<despace::g_type &>(td).get_mutable();
        D() << "before " << d;
        despacer::sse42_despace_branchless_lookup(d.data(), d.size());
        D() << "after " << d;
    }
};



#endif // DESPACE_ALGS_H
