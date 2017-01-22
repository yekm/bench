#include "despace/despace_task.hpp"
#include "despace/despace_algs.hpp"
#include "taskcollection.hpp"

namespace
{

static struct st
{
    st()
    {
        std::unique_ptr<Task> t(new despace());
        t->add_alg(std::unique_ptr<Algorithm>(new a_memcpy()));
        t->add_alg(std::unique_ptr<Algorithm>(new a_remove()));
        t->add_alg(std::unique_ptr<Algorithm>(new a_despace()));
        t->add_alg(std::unique_ptr<Algorithm>(new a_faster_despace()));
        t->add_alg(std::unique_ptr<Algorithm>(new a_despace64()));
        t->add_alg(std::unique_ptr<Algorithm>(new a_avx2_despace()));
        t->add_alg(std::unique_ptr<Algorithm>(new a_avx2_countspaces()));
        t->add_alg(std::unique_ptr<Algorithm>(new a_sse4_despace()));
        t->add_alg(std::unique_ptr<Algorithm>(new a_sse4_despace_branchless()));
        t->add_alg(std::unique_ptr<Algorithm>(new a_sse4_despace_trail()));
        t->add_alg(std::unique_ptr<Algorithm>(new a_sse42_despace()));
        t->add_alg(std::unique_ptr<Algorithm>(new a_sse42_despace_branchless()));
        t->add_alg(std::unique_ptr<Algorithm>(new a_sse42_despace_branchless_lookup()));
        TaskCollection::get().add_task(__FILE__, std::move(t));
    }
} despace_;
}
