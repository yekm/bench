#ifndef DBG_H
#define DBG_H

#include <string>
#include <vector>
#include <set>
#include <map>
#include <iostream>
#include <iterator>
#include <typeinfo>

#include <algorithm>

namespace utils
{

struct Dbg
{
    Dbg()
        : m_flags(std::cout.flags())
    {
        std::cout << "[";
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
        std::cout << " vector<" << typeid(T).name() << ">(";
        std::copy(v.begin(), v.end(), std::ostream_iterator<T>(std::cout, ", "));
        std::cout << ")";
        return *this;
    }

    Dbg& operator<< (std::vector<char> v)
    {
        std::cout << " vector<char>(";
        std::copy(v.begin(), v.end(), std::ostream_iterator<int>(std::cout, ", "));
        std::cout << ")";
        return *this;
    }

    template <typename T>
    Dbg& operator<< (std::set<T> v)
    {
        std::cout << " set<" << typeid(T).name() << ">(";
        std::copy(v.begin(), v.end(), std::ostream_iterator<T>(std::cout, ", "));
        std::cout << ")";
        return *this;
    }

    template <typename K, typename V>
    Dbg& operator<< (std::map<K, V> m)
    {
        std::cout << " map<" << typeid(K).name() << ", " << typeid(V).name() << ">(";
        for (const auto &t : m)
        {
            (*this) << t.first << ":" << t.second << ", ";
        }
        std::cout << ")";
        return *this;
    }

    Dbg& operator<< (char d)
    {
        return (*this) << (int)d;
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
    enum Verbosity
    {
        V_ERROR = 0,
        V_WARNING,
        V_INFO,
        V_DEBUG,
    };

    static Debugging& get()
    {
        static Debugging d;
        return d;
    }

    void set_verbosity(Verbosity v)
    {
        m_verbosity = v;
    }

    Verbosity get_verbosity()
    {
        return m_verbosity;
    }

private:
    Debugging()
        : m_verbosity(V_ERROR)
    {
    }

    Verbosity m_verbosity;
};

} // ns utils

namespace
{

// http://stackoverflow.com/a/8520871/864782

struct MatchPathSeparator
{
    bool operator()( char ch ) const
    {
        return ch == '/';
    }
};

#if 0 // nope nope nope
struct MatchPathSeparator
{
    bool operator()( char ch ) const
    {
        return ch == '\\' || ch == '/';
    }
};
#endif

std::string ___basename(std::string const& pathname)
{
    return std::string(
                std::find_if(
                    pathname.rbegin(), pathname.rend(), MatchPathSeparator()
                    ).base(),
                pathname.end()
                );
}

}


#ifdef DEBUG
    #define ___DBG(x) if (x <= utils::Debugging::get().get_verbosity()) utils::Dbg()
#else
    #define ___DBG(x) if (false) utils::Dbg()
#endif

#define ___FN ___basename(__FILE__) + ":" + std::to_string(__LINE__)
#define E(x) ___DBG(utils::Debugging::V_ERROR) << "E" << ___FN
#define W(x) ___DBG(utils::Debugging::V_WARNING) << "W" << ___FN
#define I(x) ___DBG(utils::Debugging::V_INFO) << "I" << ___FN
#define D(x) ___DBG(utils::Debugging::V_DEBUG) << "D" << ___FN

#endif // DBG_H
