#ifndef SORTING_ALGS_H
#define SORTING_ALGS_H

#include "algorithm.hpp"
#include "sorting_task.hpp"
#include "utils/dbg.hpp"
#include "bexeption.hpp"

#include <algorithm>

class SortingCheck : public Algorithm
{
public:
    SortingCheck(const std::string & name)
        : Algorithm(name)
    {}
    virtual void check(const TaskData & td)
    {
        D() << "check";
        auto &d = static_cast<const SortingTask::g_type&>(td).get_const();
        if (std::is_sorted(d.cbegin(), d.cend()) != true)
            throw BExeption("sorting check failed");
    }
};

class std_sort : public SortingCheck
{
public:
    std_sort()
        : SortingCheck("Introsort std::sort n*log(n)")
    {}
    virtual std::string complexity()
    {
        return "a*x*log(x)+b";
    }
    virtual void run(TaskData & td)
    {
        SortingTask::g_type::container_type &d = static_cast<SortingTask::g_type&>(td).get_mutable();
        D() << "before " << d;
        std::sort(d.begin(), d.end());
        D() << "after " << d;
    }
};

class merge_sort : public SortingCheck
{
public:
    merge_sort()
        : SortingCheck("Merge sort n*log(n)")
    {}
    virtual std::string complexity()
    {
        return "a*x*log(x)+b";
    }
    virtual void run(TaskData & td)
    {
        SortingTask::g_type::container_type &d = static_cast<SortingTask::g_type&>(td).get_mutable();
        D() << "before " << d;
        int N = d.size();
        SortingTask::g_type::container_type aux(d.size());
        for (int n = 1; n < N; n = n+n)
        {
            for (int i = 0; i < N-n; i += n+n)
            {
                int lo = i;
                int m  = i+n-1;
                int hi = std::min(i+n+n-1, N-1);
                for (int k = lo; k <= hi; k++)
                    aux[k] = d[k];
                int im = lo, jm = m+1;
                for (int k = lo; k <= hi; k++)
                {
                    if      (im > m)              d[k] = aux[jm++];  // this copying is unneccessary
                    else if (jm > hi)             d[k] = aux[im++];
                    else if (aux[jm] < aux[im])   d[k] = aux[jm++];
                    else                          d[k] = aux[im++];
                }

            }
        }
        D() << "after " << d;
    }
};

class insertion_sort : public SortingCheck
{
public:
    insertion_sort()
        : SortingCheck("Insertion sort n^2")
    {}
    virtual std::string complexity()
    {
        return "a*x**2+b";
    }
    virtual void run(TaskData & td)
    {
        SortingTask::g_type::container_type &d = static_cast<SortingTask::g_type&>(td).get_mutable();
        D() << "before " << d;
        typedef SortingTask::g_type::container_type::iterator itr_type;
        for ( itr_type i = d.begin(); i != d.end(); ++i)
        {
            for (itr_type j = i; j != d.begin() && *j < *(j-1); --j)
                std::swap(*j, *(j-1));
        }
        D() << "after " << d;
    }
};

class selection_sort : public SortingCheck
{
public:
    selection_sort()
        : SortingCheck("Selection sort n^2")
    {}
    virtual std::string complexity()
    {
        return "a*x**2+b";
    }
    virtual void run(TaskData & td)
    {
        SortingTask::g_type::container_type &d = static_cast<SortingTask::g_type&>(td).get_mutable();
        D() << "before " << d;
        typedef SortingTask::g_type::container_type::iterator itr_type;
        for ( itr_type i = d.begin(); i != d.end(); ++i)
        {
            itr_type min = i;
            for (itr_type j = i+1; j != d.end(); ++j)
                if (*j < *min) min = j;
            std::swap(*i, *min);
        }
        D() << "after " << d;
    }
};

class shell_sort : public SortingCheck
{
public:
    shell_sort()
        : SortingCheck("Shell sort n*log^2(n)")
    {}
    virtual std::string complexity()
    {
        return "a*x*(log(x))**2+b";
    }
    virtual void run(TaskData & td)
    {
        SortingTask::g_type::container_type &d = static_cast<SortingTask::g_type&>(td).get_mutable();
        D() << "before " << d;

        int N = d.size();
        // 3x+1 increment sequence:  1, 4, 13, 40, 121, 364, 1093, ...
        int h = 1;
        while (h < N/3) h = 3*h + 1;

        while (h >= 1) {
            // h-sort the array
            for (int i = h; i < N; i++) {
                for (int j = i; j >= h && d[j] < d[j-h]; j -= h) {
                    std::swap(d[j], d[j-h]);
                }
            }
            h /= 3;
        }

        D() << "after " << d;
    }
};


#endif // SORTING_ALGS_H
