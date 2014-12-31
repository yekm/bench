#include "threading/threading_task.hpp"
#include "threading/threading_algs.hpp"
#include "taskcollection.hpp"

namespace {

static struct st
{
    st()
    {
        std::unique_ptr<Task> t(new FalseSharingTask());
        t->add_alg(std::unique_ptr<Algorithm>(new FalseSharing_bad()));
        t->add_alg(std::unique_ptr<Algorithm>(new FalseSharing_fix()));
        TaskCollection::get().add_task(__FILE__, std::move(t));

    }
} threading_;

}
