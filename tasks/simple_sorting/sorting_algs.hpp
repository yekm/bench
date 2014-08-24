#ifndef SORTING_ALGS_H
#define SORTING_ALGS_H

#include "algorithm.hpp"
#include "sorting_task.hpp"
#include "simple_sorting/thrust_sort.hpp"
#include "utils/dbg.hpp"

#include "swenson-sort.h"

#include <algorithm>


class std_sort : public Algorithm
{
public:
    std_sort()
        : Algorithm("Introsort std::sort n*log(n)")
    {}
private:
    virtual std::string do_complexity()
    {
        return "a*x*log(x)+b";
    }
    virtual void do_run(TaskData & td, std::unique_ptr<AResult> &)
    {
        SortingTask::g_type::container_type &d = static_cast<SortingTask::g_type&>(td).get_mutable();
        D() << "before " << d;
        std::sort(d.begin(), d.end());
        D() << "after " << d;
    }
};

#ifdef CUDA_FOUND
class thrust_sort : public Algorithm
{
public:
    thrust_sort()
        : Algorithm("thrust::sort")
    {}
private:
/*    virtual std::string do_complexity()
    {
        return "a*x*log(x)+b";
    }*/
    virtual void do_run(TaskData & td, std::unique_ptr<AResult> &)
    {
        SortingTask::g_type::container_type &d = static_cast<SortingTask::g_type&>(td).get_mutable();
        D() << "before " << d;
        thrust_sort_ex<SortingTask::item_type>(d);
        D() << "after " << d;
    }
};
#endif

class merge_sort : public Algorithm
{
public:
    merge_sort()
        : Algorithm("Merge sort n*log(n)")
    {}
private:
    virtual std::string do_complexity()
    {
        return "a*x*log(x)+b";
    }
    virtual void do_run(TaskData & td, std::unique_ptr<AResult> &)
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

class insertion_sort : public Algorithm
{
public:
    insertion_sort()
        : Algorithm("Insertion sort n^2")
    {}
private:
    virtual std::string do_complexity()
    {
        return "a*x**2+b";
    }
    virtual void do_run(TaskData & td, std::unique_ptr<AResult> &)
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

class selection_sort : public Algorithm
{
public:
    selection_sort()
        : Algorithm("Selection sort n^2")
    {}
private:
    virtual std::string do_complexity()
    {
        return "a*x**2+b";
    }
    virtual void do_run(TaskData & td, std::unique_ptr<AResult> &)
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

class shell_sort : public Algorithm
{
public:
    shell_sort()
        : Algorithm("Shell sort n*log^2(n)")
    {}
private:
    virtual std::string do_complexity()
    {
        return "a*x*(log(x))**2+b";
    }
    virtual void do_run(TaskData & td, std::unique_ptr<AResult> &)
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

class swenson_quiksort : public Algorithm
{
public:
    swenson_quiksort()
        : Algorithm("swenson quiksort n*log(n)")
    {}
private:
    virtual std::string do_complexity()
    {
        return "a*x*log(x)+b";
    }
    virtual void do_run(TaskData & td, std::unique_ptr<AResult> &)
    {
        SortingTask::g_type::container_type &d = static_cast<SortingTask::g_type&>(td).get_mutable();
        D() << "before " << d;
        swenson_quick_sort(d.data(), d.size());
        D() << "after " << d;
    }
};

class swenson_timsort : public Algorithm
{
public:
    swenson_timsort()
        : Algorithm("swenson timsort n*log(n)")
    {}
private:
    virtual std::string do_complexity()
    {
        return "a*x*log(x)+b";
    }
    virtual void do_run(TaskData & td, std::unique_ptr<AResult> &)
    {
        SortingTask::g_type::container_type &d = static_cast<SortingTask::g_type&>(td).get_mutable();
        D() << "before " << d;
        swenson_tim_sort(d.data(), d.size());
        D() << "after " << d;
    }
};

class swenson_mergesort : public Algorithm
{
public:
    swenson_mergesort()
        : Algorithm("swenson mergesort n*log(n)")
    {}
private:
    virtual std::string do_complexity()
    {
        return "a*x*log(x)+b";
    }
    virtual void do_run(TaskData & td, std::unique_ptr<AResult> &)
    {
        SortingTask::g_type::container_type &d = static_cast<SortingTask::g_type&>(td).get_mutable();
        D() << "before " << d;
        swenson_merge_sort(d.data(), d.size());
        D() << "after " << d;
    }
};

class swenson_heapsort : public Algorithm
{
public:
    swenson_heapsort()
        : Algorithm("swenson heapsort n*log(n)")
    {}
private:
    virtual std::string do_complexity()
    {
        return "a*x*log(x)+b";
    }
    virtual void do_run(TaskData & td, std::unique_ptr<AResult> &)
    {
        SortingTask::g_type::container_type &d = static_cast<SortingTask::g_type&>(td).get_mutable();
        D() << "before " << d;
        swenson_heap_sort(d.data(), d.size());
        D() << "after " << d;
    }
};

class swenson_bi_sort : public Algorithm
{
public:
    swenson_bi_sort()
        : Algorithm("swenson binary insertion sort n^2")
    {}
private:
    virtual std::string do_complexity()
    {
        return "a*x*x+b";
    }
    virtual void do_run(TaskData & td, std::unique_ptr<AResult> &)
    {
        SortingTask::g_type::container_type &d = static_cast<SortingTask::g_type&>(td).get_mutable();
        D() << "before " << d;
        swenson_binary_insertion_sort(d.data(), d.size());
        D() << "after " << d;
    }
};

class swenson_selectionsort : public Algorithm
{
public:
    swenson_selectionsort()
        : Algorithm("swenson selection sort n^2")
    {}
private:
    virtual std::string do_complexity()
    {
        return "a*x*x+b";
    }
    virtual void do_run(TaskData & td, std::unique_ptr<AResult> &)
    {
        SortingTask::g_type::container_type &d = static_cast<SortingTask::g_type&>(td).get_mutable();
        D() << "before " << d;
        swenson_heap_sort(d.data(), d.size());
        D() << "after " << d;
    }
};

class swenson_shellsort : public Algorithm
{
public:
    swenson_shellsort()
        : Algorithm("swenson sell sort n*log^2(n)")
    {}
private:
    virtual std::string do_complexity()
    {
        return "a*x*(log(x))**2+b";
    }
    virtual void do_run(TaskData & td, std::unique_ptr<AResult> &)
    {
        SortingTask::g_type::container_type &d = static_cast<SortingTask::g_type&>(td).get_mutable();
        D() << "before " << d;
        swenson_shell_sort(d.data(), d.size());
        D() << "after " << d;
    }
};

class swenson_grailsort : public Algorithm
{
public:
    swenson_grailsort()
        : Algorithm("swenson grail sort")
    {}
private:
    /*virtual std::string do_complexity()
    {
        return "a*x*(log(x))**2+b";
    }*/
    virtual void do_run(TaskData & td, std::unique_ptr<AResult> &)
    {
        SortingTask::g_type::container_type &d = static_cast<SortingTask::g_type&>(td).get_mutable();
        D() << "before " << d;
        swenson_grail_sort(d.data(), d.size());
        D() << "after " << d;
    }
};

class swenson_sqrtsort : public Algorithm
{
public:
    swenson_sqrtsort()
        : Algorithm("swenson sqrt sort")
    {}
private:
    /*virtual std::string do_complexity()
    {
        return "a*x*(log(x))**2+b";
    }*/
    virtual void do_run(TaskData & td, std::unique_ptr<AResult> &)
    {
        SortingTask::g_type::container_type &d = static_cast<SortingTask::g_type&>(td).get_mutable();
        D() << "before " << d;
        swenson_sqrt_sort(d.data(), d.size());
        D() << "after " << d;
    }
};

#endif // SORTING_ALGS_H
