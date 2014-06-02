#include <iostream>

#include "taskcollection.h"
#include "utils/stat.h"
#include "utils/timer.h"
#include "utils/dbg.h"
#include "utils/gnuplotoutput.h"
#include "algorithmstatdecorator.h"
#include <chrono>

#include <getopt.h>

int main(int argc, char * argv[])
{
    int opt;
    int runs_per_n = 3;
    int max_round_time = 120; // seconds
    std::size_t owerride_max_n = 0;
    while ((opt = getopt(argc, argv, "r:dn:t:")) != -1) {
        switch (opt) {
        case 'r':
            runs_per_n = atoi(optarg);
            break;
        case 'n':
            owerride_max_n = atof(optarg); // for e-notation
            break;
        case 'd':
            utils::Debugging::get().set(true);
            break;
        case 't':
            max_round_time = atoi(optarg);
            break;
        default:
            std::cout << "Usage: bench [-r int] [-d] [-n size_t] [-t int]\n"
                      << "\t-r\talgorithms run per each n. ex -r 1. (default: 3)\n"
                      << "\t-d\tenable debug output. ex -d. (default: false)\n"
                      << "\t-n\tmaximum n. ex -n 1e10 (default: algorithm dependent)\n"
                      << "\t-t\tmaximum algorithm run time. ex -t 300. (default: 120 seconds). broken for now.\n"
                      << "\nexample: ../bench -r 1 -n 1e6 | tee out.gnuplot | gnuplot && $BROWSER index.html\n"
                         ;
            return -1;
            break;
        }
    }

    //std::cout << TaskCollection::get().size() << std::endl;
    for (auto & x : TaskCollection::get())
    {
        Task & t = *x.second.get();
        //std::cout << x.first << " :: " << t.get_name() << std::endl;
        auto ns = t.get_n();
        std::size_t n = ns.first
                , c = 0
                , m = 1
                , max_n = owerride_max_n != 0 ? owerride_max_n : ns.second;
        while(n < max_n)
        {
            std::shared_ptr<TaskData> td = t.prepare_data(n);
            for (auto & a : t.get_algorithms())
            {
                Algorithm & alg = *a.second.get();
                if (static_cast<AlgorithmStatDecorator*>(a.second.get())->get_status() != AlgorithmStatDecorator::AS_OK)
                    continue;
                alg.prepare(*td.get());
                //std::cout << a.second->get_name() << std::endl;
                for (int i=0; i<runs_per_n; i++)
                {
                    std::shared_ptr<TaskData> td_clone(td->clone());
                    alg.run(*td_clone.get());
                    alg.check(*td_clone.get());
                }
            }
            c++;
            m *= 2;
            n = ns.first * m;
        }
    }
    std::cout << std::endl;
    utils::GnuplotOutput go;
    go.write();
    return 0;
}
