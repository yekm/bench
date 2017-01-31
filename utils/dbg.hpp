#ifndef DBG_H
#define DBG_H

#include <string>
#include <vector>
#include <set>
#include <map>
#include <iostream>
#include <iomanip>
#include <iterator>
#include <typeinfo>
#include <thread>
#include <type_traits>
#include <algorithm>

namespace utils
{

class Debugging
{
public:
    enum class Verbosity
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
#ifdef DEBUG
        : m_verbosity(Verbosity::V_DEBUG)
#else
        : m_verbosity(Verbosity::V_ERROR)
#endif
    {
    }

    Verbosity m_verbosity;
};

struct Dbg
{
    Dbg(Debugging::Verbosity v)
        : m_flags(std::cout.flags())
    {
        std::cout << "[";
        switch (v)
        {
        case Debugging::Verbosity::V_ERROR:   std::cout << "E "; break;
        case Debugging::Verbosity::V_WARNING: std::cout << "W "; break;
        case Debugging::Verbosity::V_INFO:    std::cout << "I "; break;
        case Debugging::Verbosity::V_DEBUG:   std::cout << "D "; break;
        default:                              std::cout << "? "; break;
        }
        std::cout << std::hex << std::this_thread::get_id() << " ";
        std::cout.flags(m_flags);
    }

    ~Dbg()
    {
        std::cout << "]\n" << std::flush;
        std::cout.flags(m_flags);
    }

    template<typename T>
    Dbg& p(T & d)
    {
        std::cout << d;
        if (!nos)
            std::cout << " ";
        std::cout.flags(m_flags);
        return *this;
    }

    template <typename U, typename Enable = void>
    struct sOP {
        static void setup(U d) {}
    };

    template<typename U>
    struct sOP<U, typename std::enable_if<std::is_integral<U>::value>::type> {
        static void setup(U d)
        {
            std::cout << "0x" << std::hex << std::setw(2) << std::setfill('0');
        }
    };

    template<typename U>
    struct sOP<U, typename std::enable_if<std::is_floating_point<U>::value>::type> {
        static void setup(U d)
        {
            std::cout << std::setprecision(4);
        }
    };


    template<typename T>
    struct OP {
        OP(Dbg & d)
            : m_dbg(d) {}
        Dbg& op(T d)
        {
            sOP<T>::setup(d);
            return m_dbg.p(d);
        }
    private:
        Dbg& p(T d)
        {
            return m_dbg.p(d);
        }
        Dbg& m_dbg;
    };

    template<typename T>
    Dbg& operator<< (T d)
    {
        return OP<T>(*this).op(d);
    }

    void pindent()
    {
        for (int i=0; i<indent; i++)
            std::cout << "  ";
    }

    template <typename T>
    Dbg& operator<< (std::vector<T> v)
    {
        nos = true;
        (*this) << "vector<" << typeid(T).name() << ">(\n";
        indent++;
        pindent();
        int line = 0;
        for (const auto & e : v)
        {
            (*this) << e;
            line++;
            if (line % 16 == 0)
                std::cout << "\n", pindent();
            else
                std::cout << " ";
        }
        indent--;
        if (line%16)
            std::cout << "\n";
        pindent();
        (*this) << ")";
        nos = false;
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
    bool nos = false;
    int indent= 0;
};

} // ns utils


namespace
{

std::string ___basename(std::string const& pathname)
{
    auto slash = pathname.find_last_of("/");
    if (slash == std::string::npos)
        return pathname;
    else
        return pathname.substr(slash+1);
}

} // ns


#ifdef DEBUG
    #define ___DBG(x) if (x <= utils::Debugging::get().get_verbosity()) utils::Dbg(x)
#else
    #define ___DBG(x) if (false) utils::Dbg(x)
#endif

#define ___FN ___basename(__FILE__) + ":" + std::to_string(__LINE__)
#define E(x) ___DBG(utils::Debugging::Verbosity::V_ERROR)   << ___FN
#define W(x) ___DBG(utils::Debugging::Verbosity::V_WARNING) << ___FN
#define I(x) ___DBG(utils::Debugging::Verbosity::V_INFO)    << ___FN
#define D(x) ___DBG(utils::Debugging::Verbosity::V_DEBUG)   << ___FN

#endif // DBG_H
