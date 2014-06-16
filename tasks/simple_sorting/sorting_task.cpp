#include "simple_sorting/sorting_task.hpp"
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
        TaskCollection::get().add_task(__FILE__, std::move(t));
    }
} sorting_task_;

}
