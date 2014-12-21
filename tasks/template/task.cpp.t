#include "%LTN%/%LTN%_task.hpp"
#include "%LTN%/%LTN%_algs.hpp"
#include "taskcollection.hpp"

namespace {

static struct st
{
    st()
    {
        std::unique_ptr<Task> t(new %TN%());
        t->add_alg(std::unique_ptr<Algorithm>(new alg()));
        TaskCollection::get().add_task(__FILE__, std::move(t));

    }
} %LTN%_;

}
