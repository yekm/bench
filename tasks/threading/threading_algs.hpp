#ifndef THREADING_ALGS_H
#define THREADING_ALGS_H

#include "algorithm.hpp"
#include "threading_task.hpp"
#include "utils/dbg.hpp"

#include <vector>
#include <thread>

class MTalg : public Algorithm
{
public:
    MTalg(const std::string & name)
        : Algorithm(name)
    {}
private:
    virtual void do_run(TaskData & td, std::unique_ptr<AResult> & ares) override
    {
        Threading::CustomRandomData & d =
                    static_cast<Threading::CustomRandomData&>(td);
        D() << "initial data" << d.get_const();
        std::size_t threads_count = d.get_num_threads();
        init(threads_count);
        std::vector<std::thread> threads;
        threads.reserve(threads_count);

        for(std::size_t i=0; i<threads_count; ++i)
        {
            /* Cant simply use [&](){do_thread()} here.
             * i will be captured by reference and may be incremented before
             * passing to do_thread. We need at least [this,i,&d](){}
             * */
            threads.push_back(std::thread([&,i](){do_thread(d, i);}));
        }
        for (auto & t : threads)
            t.join();

        std::unique_ptr<Threading::result_type> result(new Threading::result_type(0));
        gather_results(*result);
        ares->set_custom_result(std::move(result));
    }

    virtual void init(std::size_t thread_count) = 0;
    virtual void do_thread(Threading::CustomRandomData & d,
                           std::size_t thread_id) = 0;
    virtual void gather_results(Threading::result_type &) = 0;
};

template <typename T, unsigned Align>
struct alignas(Align) AlignedStruct
{
    static_assert(std::is_pod<T>::value, "cant align non-pod");
    AlignedStruct()
        : m_data(0)
    {}
    T m_data;
};

// http://www.drdobbs.com/parallel/eliminate-false-sharing/217500206
template <unsigned Align>
class FalseSharing : public MTalg
{
public:
    FalseSharing(const std::string & name)
        : MTalg(name)
    {}
private:
    typedef AlignedStruct<std::size_t, Align> aligned_type;
    virtual void init(std::size_t thread_count) override
    {
        m_thread_count = thread_count;
        m_results.resize(0); // FIXME: persistent data in Algorithms is bad, mkay?
        m_results.resize(m_thread_count, aligned_type());
    }

#pragma GCC push_options
#pragma GCC optimize(0)
/* Apparently gcc with optimizations uses separate counter and assigns it to
 * m_results[t_id].m_data at the end of the loop, thus eliminating fasle sharing.
 * Since we testing data alignment, we can disable optimizations here.
 * */
    virtual void do_thread(Threading::CustomRandomData & d,
                           std::size_t t_id) override
    {
        const auto & data = d.get_const();
        std::size_t chunk_size = data.size() / m_thread_count;
        auto start = data.begin() + chunk_size * t_id;
        auto end = t_id+1==m_thread_count ?
                    data.end() :
                    start + chunk_size;

        while (start != end)
        {
            if (*start % 2 == 0)
                ++(m_results[t_id].m_data);
            ++start;
        }
    }
#pragma GCC pop_options

    virtual void gather_results(Threading::result_type & result) override
    {
        result.m_simple_result = std::accumulate(
                    m_results.begin(),
                    m_results.end(),
                    std::size_t(0),
                    [](std::size_t sum, const aligned_type & element){
                        return sum+element.m_data;
                    });
    }

    std::vector<aligned_type> m_results;
    std::size_t m_thread_count;
};

class FalseSharing_bad : public FalseSharing<alignof(std::size_t)>
{
public:
    FalseSharing_bad()
        : FalseSharing("false sharing, default alignment")
    {}
};

// most common cache line size is 64
class FalseSharing_fix : public FalseSharing<64>
{
public:
    FalseSharing_fix()
        : FalseSharing("fixed false sharing, 64 bytes alignment")
    {}
};

#endif // THREADING_ALGS_H
