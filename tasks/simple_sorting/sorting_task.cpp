#include "simple_sorting/sorting_task.hpp"
#include "simple_sorting/psorting_task.hpp"
#include "simple_sorting/sorting_algs.hpp"
#include "taskcollection.hpp"

namespace {

static struct st
{
    st()
    {
        std::unique_ptr<Task> t(new SortingTask());
        t->add_alg(std::unique_ptr<Algorithm>(new std_sort()));
        t->add_alg(std::unique_ptr<Algorithm>(new merge_sort()));
        t->add_alg(std::unique_ptr<Algorithm>(new insertion_sort()));
        t->add_alg(std::unique_ptr<Algorithm>(new shell_sort()));
        t->add_alg(std::unique_ptr<Algorithm>(new selection_sort()));
        t->add_alg(std::unique_ptr<Algorithm>(new swenson_quiksort()));
        t->add_alg(std::unique_ptr<Algorithm>(new swenson_timsort()));
        t->add_alg(std::unique_ptr<Algorithm>(new swenson_heapsort()));
        t->add_alg(std::unique_ptr<Algorithm>(new swenson_bi_sort()));
        t->add_alg(std::unique_ptr<Algorithm>(new swenson_selectionsort()));
        t->add_alg(std::unique_ptr<Algorithm>(new swenson_shellsort()));
        t->add_alg(std::unique_ptr<Algorithm>(new swenson_grailsort()));
        t->add_alg(std::unique_ptr<Algorithm>(new swenson_sqrtsort()));
        t->add_alg(std::unique_ptr<Algorithm>(new swenson_mergesort()));
        //t->add_alg(std::unique_ptr<Algorithm>(new djb_avx2_sort()));
        t->add_alg(std::unique_ptr<Algorithm>(new djb_portable4_sort()));
        t->add_alg(std::unique_ptr<Algorithm>(new pdq_sort()));
#ifdef CUDA_FOUND
        t->add_alg(std::unique_ptr<Algorithm>(new thrust_sort()));
#endif
        TaskCollection::get().add_task(__FILE__, std::move(t));

        std::unique_ptr<Task> pt(new PSortingTask());
        pt->add_alg(std::unique_ptr<Algorithm>(new std_sort()));
        pt->add_alg(std::unique_ptr<Algorithm>(new insertion_sort()));
        pt->add_alg(std::unique_ptr<Algorithm>(new shell_sort()));
        pt->add_alg(std::unique_ptr<Algorithm>(new merge_sort()));
        pt->add_alg(std::unique_ptr<Algorithm>(new swenson_timsort()));
        pt->add_alg(std::unique_ptr<Algorithm>(new djb_portable4_sort()));
        pt->add_alg(std::unique_ptr<Algorithm>(new pdq_sort()));
#ifdef CUDA_FOUND
        pt->add_alg(std::unique_ptr<Algorithm>(new thrust_sort()));
#endif
        TaskCollection::get().add_task(__FILE__ "2", std::move(pt));
    }
} sorting_task_;

}
