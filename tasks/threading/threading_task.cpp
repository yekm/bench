#include "threading/threading_task.hpp"
#include "threading/threading_algs.hpp"
#include "taskcollection.hpp"

namespace {

static struct st
{
    st()
    {
        std::unique_ptr<Task> t(new ThreadedCountingTask("false sharing"));
        t->add_alg(std::unique_ptr<Algorithm>(new FalseSharing_bad()));
        t->add_alg(std::unique_ptr<Algorithm>(new FalseSharing_fix()));
        TaskCollection::get().add_task(__FILE__, std::move(t));

        std::unique_ptr<Task> t2(new ThreadedCountingTask("atomic vs mutex"));
        t2->add_alg(std::unique_ptr<Algorithm>(new AtomicSync()));
        t2->add_alg(std::unique_ptr<Algorithm>(new MutexSync()));
        TaskCollection::get().add_task(__FILE__ "_b", std::move(t2));

    }
} threading_;

}
