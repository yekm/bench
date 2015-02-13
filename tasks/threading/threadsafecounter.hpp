#ifndef THREADSAFECOUNTER_HPP
#define THREADSAFECOUNTER_HPP

#include <vector>
#include <atomic>
#include <mutex>

namespace tsc
{

template <typename T, unsigned Align>
struct alignas(Align) AlignedStruct
{
    static_assert(std::is_pod<T>::value, "cant align non-pod");
    AlignedStruct()
        : m_data(0)
    {}
    T m_data;
};

class ThreadSafeCounter
{
public:
    virtual void inc(size_t thread_i) = 0;
    virtual size_t result() = 0;
};


// http://www.drdobbs.com/parallel/eliminate-false-sharing/217500206

template <unsigned Align>
class LockfreeCounter : public ThreadSafeCounter
{
public:
    LockfreeCounter(size_t n)
        : m_results(n)
    {
    }

    void inc(size_t thread_i) override
    {
        ++(m_results[thread_i].m_data);
    }
    size_t result() override
    {
        return std::accumulate(
                    m_results.begin(),
                    m_results.end(),
                    std::size_t(0),
                    [](std::size_t sum, const aligned_type & element){
            return sum+element.m_data;
        });
    }
private:
    typedef AlignedStruct<std::size_t, Align> aligned_type;
    std::vector<aligned_type> m_results;
};

typedef LockfreeCounter<alignof(std::size_t)> FalseSharingCounter;
// most common cache line size is 64
typedef LockfreeCounter<64> FixedFalseSharingCounter;


class AtomicCounter : public ThreadSafeCounter
{
public:
    void inc(size_t) override
    {
        ++m_count;
    }
    size_t result() override
    {
        return m_count;
    }
private:
    std::atomic<size_t> m_count;
};

class MutexCounter : public ThreadSafeCounter
{
public:
    void inc(size_t) override
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        ++m_count;
    }
    size_t result() override
    {
        return m_count;
    }
private:
    std::mutex m_mutex;
    size_t m_count;
};

} // ns tsc

#endif // THREADSAFECOUNTER_HPP
