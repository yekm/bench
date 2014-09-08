#ifndef POPCNT_ALGS_HPP
#define POPCNT_ALGS_HPP

#include "algorithm.hpp"
#include "popcnt.hpp"
#include "utils/dbg.hpp"

#include <x86intrin.h>

/* Inspired by http://stackoverflow.com/questions/25078285/replacing-a-32-bit-loop-count-variable-with-64-bit-introduces-crazy-performance
 * some docs http://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetParallel
 */

class simple_pcnt : public Algorithm
{
public:
    simple_pcnt()
        : Algorithm("Simple popcnt")
    {}
private:
    virtual void do_run(TaskData & td, std::unique_ptr<AResult> & r)
    {
        const Popcnt::g_type::container_type &d = static_cast<Popcnt::g_type&>(td).get_const();
        size_t pcount = 0;
        for (uint32_t v : d)
        {
            while (v != 0)
            {
                pcount += v & 1;
                v >>= 1;
            }
        }
        std::unique_ptr<PResult> res(new PResult());
        res->count = pcount;
        r->set_custom_result(std::move(res));
        D() << get_name() << pcount;
    }
};

class brianc_pcnt : public Algorithm
{
public:
    brianc_pcnt()
        : Algorithm("Brian Kernighan popcnt")
    {}
private:
    virtual void do_run(TaskData & td, std::unique_ptr<AResult> & r)
    {
        const Popcnt::g_type::container_type &d = static_cast<Popcnt::g_type&>(td).get_const();
        size_t pcount = 0;
        for (uint32_t v : d)
        {
            while (v != 0)
            {
                v &= v - 1;
                pcount++;
            }
        }
        std::unique_ptr<PResult> res(new PResult());
        res->count = pcount;
        r->set_custom_result(std::move(res));
        D() << get_name() << pcount;
    }
};

class swar32_pcnt : public Algorithm
{
public:
    swar32_pcnt()
        : Algorithm("32bit SWAR popcnt")
    {}
private:
    virtual void do_run(TaskData & td, std::unique_ptr<AResult> & r)
    {
        const Popcnt::g_type::container_type &d = static_cast<Popcnt::g_type&>(td).get_const();
        size_t pcount = 0;
        for (uint32_t i : d)
        {
            i = i - ((i >> 1) & 0x55555555);
            i = (i & 0x33333333) + ((i >> 2) & 0x33333333);
            pcount += (((i + (i >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
        }
        std::unique_ptr<PResult> res(new PResult());
        res->count = pcount;
        r->set_custom_result(std::move(res));
        D() << get_name() << pcount;
    }
};

template <typename T>
inline T generalized(T v)
{
    v = v - ((v >> 1) & (T)~(T)0/3);
    v = (v & (T)~(T)0/15*3) + ((v >> 2) & (T)~(T)0/15*3);
    v = (v + (v >> 4)) & (T)~(T)0/255*15;
    return (T)(v * ((T)~(T)0/255)) >> (sizeof(T) - 1) * 8;
}

class swargen_pcnt : public Algorithm
{
public:
    swargen_pcnt()
        : Algorithm("generalized 32 bit SWAR popcnt")
    {}
private:
    virtual void do_run(TaskData & td, std::unique_ptr<AResult> & r)
    {
        const Popcnt::g_type::container_type &d = static_cast<Popcnt::g_type&>(td).get_const();
        size_t pcount = 0;
        for (uint32_t i : d)
        {
            pcount += generalized(i);
        }
        std::unique_ptr<PResult> res(new PResult());
        res->count = pcount;
        r->set_custom_result(std::move(res));
        D() << get_name() << pcount;
    }
};


class swargen64_pcnt : public Algorithm
{
public:
    swargen64_pcnt()
        : Algorithm("generalized 64 bit SWAR popcnt")
    {}
private:
    virtual void do_run(TaskData & td, std::unique_ptr<AResult> & r)
    {
        const Popcnt::g_type::container_type &d = static_cast<Popcnt::g_type&>(td).get_const();
        const u_int64_t * d64 = reinterpret_cast<const u_int64_t*>(&d[0]);
        const size_t size = d.size() / ( sizeof(u_int64_t) / sizeof(Popcnt::item_type) );
        size_t pcount = 0;
        size_t c = 0;
        while (c < size)
        {
            u_int64_t i = d64[c];
            pcount += generalized(i);
            c++;
        }
        std::unique_ptr<PResult> res(new PResult());
        res->count = pcount;
        r->set_custom_result(std::move(res));
        D() << get_name() << pcount;
    }
};

class table_pcnt : public Algorithm
{
public:
    table_pcnt()
        : Algorithm("table lookup popcnt")
    {
        m_table[0] = 0;
        for (int i = 0; i < 256; i++)
        {
          m_table[i] = (i & 1) + m_table[i / 2];
        }
    }
private:
    virtual void do_run(TaskData & td, std::unique_ptr<AResult> & r)
    {
        const Popcnt::g_type::container_type &d = static_cast<Popcnt::g_type&>(td).get_const();
        size_t pcount = 0;
        for (uint32_t i : d)
        {
            unsigned char * p = reinterpret_cast<unsigned char *>(&i);
            pcount += m_table[p[0]] + m_table[p[1]] + m_table[p[2]] + m_table[p[3]];
        }
        std::unique_ptr<PResult> res(new PResult());
        res->count = pcount;
        r->set_custom_result(std::move(res));
        D() << get_name() << pcount;
    }
    unsigned char m_table[256];
};

#ifdef __POPCNT__

class intrin64_pcnt : public Algorithm
{
public:
    intrin64_pcnt()
        : Algorithm("intrinsics _mm_popcnt_u64 popcnt")
    {}
private:
    virtual void do_run(TaskData & td, std::unique_ptr<AResult> & r)
    {
        const Popcnt::g_type::container_type &d = static_cast<Popcnt::g_type&>(td).get_const();
        const u_int64_t * d64 = reinterpret_cast<const u_int64_t*>(&d[0]);
        const size_t size = d.size() / ( sizeof(u_int64_t) / sizeof(Popcnt::item_type) );
        size_t pcount = 0;
        size_t c = 0;
        while (c < size)
        {
            pcount += _mm_popcnt_u64(d64[c]);
            c++;
        }
        std::unique_ptr<PResult> res(new PResult());
        res->count = pcount;
        r->set_custom_result(std::move(res));
        D() << get_name() << pcount;
    }
};

class unr_intrin64_pcnt : public Algorithm
{
public:
    unr_intrin64_pcnt()
        : Algorithm("intrinsics _mm_popcnt_u64 unrolled popcnt")
    {}
private:
    virtual void do_run(TaskData & td, std::unique_ptr<AResult> & r)
    {
        const Popcnt::g_type::container_type &d = static_cast<Popcnt::g_type&>(td).get_const();
        const u_int64_t * d64 = reinterpret_cast<const u_int64_t*>(&d[0]);
        const size_t size = d.size() / ( sizeof(u_int64_t) / sizeof(Popcnt::item_type) );
        size_t pcount = 0;
        size_t c = 0;
        while (c < size)
        {
            pcount += _mm_popcnt_u64(d64[c]);
            pcount += _mm_popcnt_u64(d64[c+1]);
            pcount += _mm_popcnt_u64(d64[c+2]);
            pcount += _mm_popcnt_u64(d64[c+3]);
            c+=4;
        }
        std::unique_ptr<PResult> res(new PResult());
        res->count = pcount;
        r->set_custom_result(std::move(res));
        D() << get_name() << pcount;
    }
};


class asm_intrin64_pcnt : public Algorithm
{
public:
    asm_intrin64_pcnt()
        : Algorithm("intrinsics _mm_popcnt_u64 manual asm popcnt")
    {}
private:
    virtual void do_run(TaskData & td, std::unique_ptr<AResult> & r)
    {
        const Popcnt::g_type::container_type &d = static_cast<Popcnt::g_type&>(td).get_const();
        const u_int64_t * d64 = reinterpret_cast<const u_int64_t*>(&d[0]);
        const size_t size = d.size() / ( sizeof(u_int64_t) / sizeof(Popcnt::item_type) );
        size_t pcount = 0;
        size_t c = 0;
        u_int64_t c0 = 0;
        u_int64_t c1 = 0;
        u_int64_t c2 = 0;
        u_int64_t c3 = 0;
        while (c < size)
        {
            u_int64_t r0 = d64[c + 0];
            u_int64_t r1 = d64[c + 1];
            u_int64_t r2 = d64[c + 2];
            u_int64_t r3 = d64[c + 3];
            __asm__(
                "popcnt %4, %4  \n\t"
                "add %4, %0     \n\t"
                "popcnt %5, %5  \n\t"
                "add %5, %1     \n\t"
                "popcnt %6, %6  \n\t"
                "add %6, %2     \n\t"
                "popcnt %7, %7  \n\t"
                "add %7, %3     \n\t"
                : "+r" (c0), "+r" (c1), "+r" (c2), "+r" (c3)
                : "r"  (r0), "r"  (r1), "r"  (r2), "r"  (r3)
            );
            c+=4;
        }
        pcount = c0 + c1 + c2 + c3;
        std::unique_ptr<PResult> res(new PResult());
        res->count = pcount;
        r->set_custom_result(std::move(res));
        D() << get_name() << pcount;
    }
};

#endif // __POPCNT__

#endif // POPCNT_ALGS_HPP
