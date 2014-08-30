#include <iostream>
#include <set>
#include <chrono>
#include <sstream>

#include <getopt.h>

#include "taskcollection.hpp"
#include "utils/stat.hpp"
#include "utils/timer.hpp"
#include "utils/dbg.hpp"
#include "utils/output/jsonoutput.hpp"
#include "utils/output/gnuplotoutput.hpp"
#include "utils/timemeasurement.hpp"

void list_tasks()
{
    int n = 0;
    for (auto & x : TaskCollection::get())
    {
        const std::unique_ptr<Task> & task = x.second;
        std::cout << "Task " << n << ": " << task->get_name() << std::endl;
        for (auto & a : task->get_algorithms())
        {
            std::cout << "  " << a.second->get_name() << std::endl;
        }
        n++;
    }
}

void usage()
{
    std::cout << "Usage: bench [-r int] [-d] [-n size_t] [-t float]\n"
              << "\t-a\talgorithm runs per each n (default: 3)\n"
              << "\t-b\talgorithm runs per each n with newly generated data (default: 1)\n"
              << "\t-d\tset debug verbosity level [0,3] (default: 0, error)\n"
              << "\t-1\tstarting n value (default: 1)\n"
              << "\t-2\tmaximum n (default: task dependent)\n"
              << "\t-t\tmaximum algorithm run time (default: 60 seconds)\n"
              << "\t-l\tlist tasks\n"
              << "\t-s\tskip certain tasks (default: none)\n"
              << "\nexample: ../bench -a 5 -b 2 -1 128 -2 7e4 -t 0.5 -d 2 -s 1,2,5,10 && gnuplot o.gnuplot && $BROWSER index.html\n"
                 ;
}

std::set<int> make_skiplist(std::string arg)
{
    std::set<int> res;
    std::replace(arg.begin(), arg.end(), ',', ' ');
    std::stringstream ss(arg.c_str());
    copy(std::istream_iterator<int>(ss),
         std::istream_iterator<int>(),
         std::inserter(res, res.begin()));
    I() << "skiplist: " << res;
    return res;
}

void mark_skip(std::set<int> skiplist)
{
    int n = 0;
    for (auto & x : TaskCollection::get())
    {
        const std::unique_ptr<Task> & task = x.second;
        if (skiplist.find(n) != skiplist.end())
            task->m_status.set_status(utils::Status::SE_SKIP);
        n++;
    }
}

int main(int argc, char * argv[])
{
    int opt;
    int runs_per_n = 3, reset_runs_per_n = 1;
    utils::Timer::timediff_type max_round_time = 60; // seconds
    std::size_t max_n = std::numeric_limits<std::size_t>::max();
    std::size_t start_n = 1;
    while ((opt = getopt(argc, argv, "a:b:d:1:2:t:ls:")) != -1) {
        switch (opt) {
        case 'a':
            runs_per_n = atoi(optarg);
            break;
        case 'b':
            reset_runs_per_n = atoi(optarg);
            break;
        case '1':
            start_n = atof(optarg); // for e-notation
            break;
        case '2':
            max_n = atof(optarg); // for e-notation
            break;
        case 'd':
            utils::Debugging::get().set_verbosity(
                        static_cast<utils::Debugging::Verbosity>(atoi(optarg))
                        );
            break;
        case 't':
            max_round_time = atof(optarg);
            break;
        case 'l':
            list_tasks();
            return 0;
        case 's':
            mark_skip(make_skiplist(optarg));
            break;
        default:
            usage();
            return -1;
        }
    }

    //std::cout << TaskCollection::get().size() << std::endl;
    for (auto & x : TaskCollection::get())
    {
        const std::unique_ptr<Task> & task = x.second;
        std::cout << "Task: " << task->get_name();
        if (!task->m_status.ok())
        {
            std::cout << " Skipping" << std::endl;
            continue;
        }
        std::cout << std::endl;
        std::size_t n = start_n;
        while(task->get_n(n) && n < max_n)
        {
            if (!task->algorithms_ok())
                break;

            for (int j=0; j<reset_runs_per_n; j++)
            {
                if (reset_runs_per_n > 1)
                    std::cout << "Reset " << j+1 << "/" << reset_runs_per_n << std::endl;
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
                        std::cout << " " << i+1 << "/" << runs_per_n << std::flush;
                        std::shared_ptr<TaskData> td_clone(td->clone());

                        std::unique_ptr<AResult> ares;
                        utils::Timer::timediff_type d;
                        utils::Status & astatus = alg->m_statistics.m_status;
                        utils::TimeMeasurement(
                                    [&](){ ares = alg->run(td_clone); }
                        ).set_timeout(max_round_time).get_time(d).get_status(astatus);

                        if (astatus.ok_timeout())
                        { // we can continue only if alg runs totally ok or timed out
                            utils::Status check_status;
                            utils::TimeMeasurement(
                                        [&](){ task->validate(*ares); }
                            ).get_status(check_status);

                            if (!check_status.ok())
                            { // no matter if alg is ok ot timed out. it failed
                                astatus = check_status;
                                break;
                            }
                            // count ok or timed out time stats
                            alg->m_statistics.m_stat_run[n].add(d);
                        }

                        if (!astatus.ok())
                            break;

                    }
                    std::cout << std::endl;
                }
            }
        }
    }
    std::cout << std::endl;
    utils::JsonOutput("main.json").write();
    utils::GnuplotOutput("o.gnuplot").write();
    return 0;
}
