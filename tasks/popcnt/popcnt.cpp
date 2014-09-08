#include "popcnt.hpp"
#include "popcnt_algs.hpp"

#include "taskcollection.hpp"

namespace {

static struct st
{
    st()
    {
        std::unique_ptr<Task> t(new Popcnt());
        t->add_alg(std::unique_ptr<Algorithm>(new simple_pcnt()));
        t->add_alg(std::unique_ptr<Algorithm>(new brianc_pcnt()));
        t->add_alg(std::unique_ptr<Algorithm>(new swar32_pcnt()));
        t->add_alg(std::unique_ptr<Algorithm>(new swargen_pcnt()));
        t->add_alg(std::unique_ptr<Algorithm>(new swargen64_pcnt()));
        t->add_alg(std::unique_ptr<Algorithm>(new table_pcnt()));
#ifdef __POPCNT__
        t->add_alg(std::unique_ptr<Algorithm>(new intrin64_pcnt()));
        t->add_alg(std::unique_ptr<Algorithm>(new unr_intrin64_pcnt()));
        t->add_alg(std::unique_ptr<Algorithm>(new asm_intrin64_pcnt()));
#endif // __POPCNT__
        TaskCollection::get().add_task(__FILE__, std::move(t));
    }
} p_task_;

}
