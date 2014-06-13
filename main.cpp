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
    std::size_t override_max_n = std::numeric_limits<std::size_t>::max();
    while ((opt = getopt(argc, argv, "r:dn:t:")) != -1) {
        switch (opt) {
        case 'r':
            runs_per_n = atoi(optarg);
            break;
        case 'n':
            override_max_n = atof(optarg); // for e-notation
            break;
        case 'd':
            utils::Debugging::get().set(true);
            break;
        case 't':
            max_round_time = atof(optarg);
            break;
        default:
            std::cout << "Usage: bench [-r int] [-d] [-n size_t] [-t float]\n"
                      << "\t-r\talgorithms run per each n (default: 3)\n"
                      << "\t-d\tenable debug output (default: false)\n"
                      << "\t-n\tmaximum n (default: task dependent)\n"
                      << "\t-t\tmaximum algorithm run time (default: 60 seconds).\n"
                      << "\nexample: ../bench -r 5 -n 7e4 -t 0.5 -d && gnuplot o.gnuplot && $BROWSER index.html\n"
                         ;
            return -1;
            break;
        }
    }

    //std::cout << TaskCollection::get().size() << std::endl;
    for (auto & x : TaskCollection::get())
    {
        const std::unique_ptr<Task> & task = x.second;
        std::cout << "Task: " << task->get_name() << std::endl;
        std::size_t n = 1;
        while(task->get_n(n) && n < override_max_n)
        {
            if (!task->algorithms_ok())
                break;

            std::shared_ptr<TaskData> td;
            utils::TimeMeasurement(
                        [&](){ td = task->prepare_data(n); }
            ).get_status(task->m_status);

            if (!task->m_status.ok())
                break;

            for (auto & a : task->get_algorithms())
            {
                std::unique_ptr<Algorithm> & alg = a.second;

                if (!alg->m_statistics.m_status.ok())
                    continue;

                utils::Timer::timediff_type d;
                utils::TimeMeasurement(
                            [&](){ alg->prepare(*td.get()); }
                ).get_time(d).get_status(alg->m_statistics.m_status);
                alg->m_statistics.m_stat_prepare[n].add(d);

                std::cout << a.second->get_name() << " N:" << n;

                for (int i=0; i<runs_per_n; i++)
                {
                    std::cout << " " << i << "/" << runs_per_n << std::flush;
                    std::shared_ptr<TaskData> td_clone(td->clone());
                    utils::Timer::timediff_type d;
                    utils::TimeMeasurement(
                                [&](){ alg->run(*td_clone.get()); }
                    ).set_timeout(max_round_time).get_time(d).get_status(alg->m_statistics.m_status);

                    alg->m_statistics.m_stat_run[n].add(d);

                    if (!alg->m_statistics.m_status.ok())
                        break;

                    alg->check(*td_clone.get());
                }
                std::cout << std::endl;
            }
        }
    }
    std::cout << std::endl;
    utils::GnuplotOutput go("o.gnuplot");
    go.write();
    return 0;
}
