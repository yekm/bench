#ifndef THREADING_ALGS_H
#define THREADING_ALGS_H

#include "algorithm.hpp"
#include "threading_task.hpp"
#include "threadsafecounter.hpp"
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


class MTCounting : public MTalg
{
public:
    MTCounting(const std::string & name)
        : MTalg(name)
    {}
protected:
    virtual std::unique_ptr<tsc::ThreadSafeCounter> get_counter(std::size_t thread_count) = 0;
private:
    virtual void init(std::size_t thread_count) override
    {
        m_thread_count = thread_count;
        m_counter = get_counter(thread_count);
    }

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
                m_counter->inc(t_id); // carefully with optimizations!
            ++start;
        }
    }

    virtual void gather_results(Threading::result_type & result) override
    {
        result.m_simple_result = m_counter->result();
    }

    std::unique_ptr<tsc::ThreadSafeCounter> m_counter;
    std::size_t m_thread_count;
};

class FalseSharing_bad : public MTCounting
{
public:
    FalseSharing_bad()
        : MTCounting("false sharing, default alignment")
    {}
protected:
    std::unique_ptr<tsc::ThreadSafeCounter> get_counter(std::size_t thread_count) override
    {
        return std::unique_ptr<tsc::ThreadSafeCounter>(new tsc::FalseSharingCounter(thread_count));
    }
};

class FalseSharing_fix : public MTCounting
{
public:
    FalseSharing_fix()
        : MTCounting("fixed false sharing, 64 bytes alignment")
    {}
protected:
    std::unique_ptr<tsc::ThreadSafeCounter> get_counter(std::size_t thread_count) override
    {
        return std::unique_ptr<tsc::ThreadSafeCounter>(new tsc::FixedFalseSharingCounter(thread_count));
    }
};

class AtomicSync : public MTCounting
{
public:
    AtomicSync()
        : MTCounting("atomic")
    {}
protected:
    std::unique_ptr<tsc::ThreadSafeCounter> get_counter(std::size_t) override
    {
        return std::unique_ptr<tsc::ThreadSafeCounter>(new tsc::AtomicCounter());
    }
};

class MutexSync : public MTCounting
{
public:
    MutexSync()
        : MTCounting("mutex")
    {}
protected:
    std::unique_ptr<tsc::ThreadSafeCounter> get_counter(std::size_t) override
    {
        return std::unique_ptr<tsc::ThreadSafeCounter>(new tsc::MutexCounter());
    }
};

#endif // THREADING_ALGS_H
