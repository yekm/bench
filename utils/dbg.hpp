#ifndef DBG_H
#define DBG_H

#include <vector>
#include <iostream>
#include <iterator>

namespace utils
{

struct Dbg
{
    Dbg()
        : m_flags(std::cout.flags())
    {
        std::cout << "[ ";
    }

    ~Dbg()
    {
        std::cout << "]\n" << std::flush;
        std::cout.flags(m_flags);
    }

    template<typename T>
    Dbg& operator<< (T d)
    {
        std::cout << d << " ";
        return *this;
    }

    template <typename T>
    Dbg& operator<< (std::vector<T> v)
    {
        std::copy(v.begin(), v.end(), std::ostream_iterator<int>(std::cout, " "));
        return *this;
    }

    Dbg& operator<< (char* d)
    {
        return (*this) << (void*)d;
    }

    Dbg& operator<< (const char *&d)
    {
        return (*this) << (void*)d;
    }
private:
    std::ios::fmtflags m_flags;
};

class Debugging
{
public:
    static Debugging& get()
    {
        static Debugging d;
        return d;
    }

    void set(bool enabled)
    {
        m_enabled = enabled;
    }

    bool enabled()
    {
        return m_enabled;
    }

private:
    Debugging()
        : m_enabled(false)
    {
    }

    bool m_enabled;
};

} // ns utils

#ifdef DEBUG
#define D(x) if (utils::Debugging::get().enabled()) utils::Dbg()
#else
    #define D(x) if (false) utils::Dbg()
#endif

#endif // DBG_H
