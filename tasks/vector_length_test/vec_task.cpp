#include "vector_length_test/vec_task.hpp"
#include "vector_length_test/vec_algs.hpp"
#include "taskcollection.hpp"

namespace {

static struct st
{
    st()
    {
        std::unique_ptr<Task> t(new VecTask());
        t->add_alg(std::unique_ptr<Algorithm>(new vec_template()));
        t->add_alg(std::unique_ptr<Algorithm>(new vec_loop()));
        t->add_alg(std::unique_ptr<Algorithm>(new vec_handmade()));
        TaskCollection::get().add_task(__FILE__, std::move(t));
    }
} sorting_task_;

}
