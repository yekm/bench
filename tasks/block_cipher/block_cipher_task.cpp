#include "block_cipher/block_cipher_task.hpp"
#include "block_cipher/block_cipher_algs.hpp"
#include "taskcollection.hpp"

namespace {

static struct st
{
    st()
    {
        std::unique_ptr<Task> t(new block_cipher());
        t->add_alg(std::unique_ptr<Algorithm>(new Gost89()));
        //t->add_alg(std::unique_ptr<Algorithm>(new KuzSimple())); // awfully slow
        t->add_alg(std::unique_ptr<Algorithm>(new KuzSIMD()));
        t->add_alg(std::unique_ptr<Algorithm>(new AES<32>()));
        t->add_alg(std::unique_ptr<Algorithm>(new AES<24>()));
        t->add_alg(std::unique_ptr<Algorithm>(new AES<16>()));
        t->add_alg(std::unique_ptr<Algorithm>(new KuzTC26()));
        t->add_alg(std::unique_ptr<Algorithm>(new KuzTC26_fast3()));
        t->add_alg(std::unique_ptr<Algorithm>(new PerbyteXOR()));
        t->add_alg(std::unique_ptr<Algorithm>(new SIMDXOR()));
        t->add_alg(std::unique_ptr<Algorithm>(new Salsa20<32>()));
        t->add_alg(std::unique_ptr<Algorithm>(new Salsa20<32, 1024>()));
        TaskCollection::get().add_task(__FILE__, std::move(t));
    }
} block_cipher_;

}
