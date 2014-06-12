#include <iostream>

#include "taskcollection.hpp"
#include "utils/stat.hpp"
#include "utils/timer.hpp"
#include "utils/dbg.hpp"
#include "utils/gnuplotoutput.hpp"
#include "utils/timemeasurement.hpp"
#include <chrono>

#include <getopt.h>

int main(int argc, char * argv[])
{
    int opt;
    int runs_per_n = 3;
    utils::Timer::timediff_type max_round_time = 60; // seconds
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
            max_round_time = atof(optarg);
            break;
        default:
            std::cout << "Usage: bench [-r int] [-d] [-n size_t] [-t int]\n"
                      << "\t-r\talgorithms run per each n. ex -r 1. (default: 3)\n"
                      << "\t-d\tenable debug output. ex -d. (default: false)\n"
                      << "\t-n\tmaximum n. ex -n 1e10 (default: algorithm dependent)\n"
                      << "\t-t\tmaximum algorithm run time. ex -t 300. (default: 120 seconds). broken for now.\n"
                      << "\nexample: ../bench -r 5 -n 7e4 && gnuplot o.gnuplot && $BROWSER index.html\n"
                         ;
            return -1;
            break;
        }
    }

    //std::cout << TaskCollection::get().size() << std::endl;
    for (auto & x : TaskCollection::get())
    {
        Task & t = *x.second.get();
        std::cout << "Task: " << t.get_name() << std::endl;
        auto ns = t.get_n();
        std::size_t n = ns.first
                , c = 0
                , m = 1
                , max_n = owerride_max_n != 0 ? owerride_max_n : ns.second;
        while(n < max_n && t.m_status.get_status() == utils::Status::SE_OK)
        {
            utils::Timer::timediff_type unused;
            std::shared_ptr<TaskData> td =
                    utils::TimeMeasurement<std::shared_ptr<TaskData>>(
                        [&](){ return t.prepare_data(n); }
                        ).measure(unused, t.m_status);

            if (t.m_status.get_status() != utils::Status::SE_OK)
                continue;

            for (auto & a : t.get_algorithms())
            {
                Algorithm * alg = a.second.get();
                if (alg->m_statistics.m_status.get_status() != utils::Status::SE_OK)
                    continue;
                alg->prepare(*td.get());
                std::cout << a.second->get_name() << " N:" << n;
                for (int i=0; i<runs_per_n; i++)
                {
                    std::cout << " " << i << "/" << runs_per_n << std::flush;
                    std::shared_ptr<TaskData> td_clone(td->clone());
                    utils::Timer::timediff_type d;
                    utils::TimeMeasurement<void>(
                                [&](){ alg->run(*td_clone.get()); }
                                ).measure(d, alg->m_statistics.m_status);

                    if (d > max_round_time)
                        alg->m_statistics.m_status.set_status(utils::Status::SE_TIMEOUT);
                    alg->m_statistics.m_stat_run[n].add(d);

                    if (alg->m_statistics.m_status.get_status() != utils::Status::SE_TIMEOUT)
                        continue;
                    alg->check(*td_clone.get());
                }
                std::cout << std::endl;
            }
            c++;
            m *= 2;
            n = ns.first * m;
        }
    }
    std::cout << std::endl;
    utils::GnuplotOutput go("o.gnuplot");
    go.write();
    return 0;
}
