#include "jsonoutput.hpp"

#include <vector>
#include "taskcollection.hpp"
#include "tsvoutput.hpp"

namespace utils
{

JsonOutput::JsonOutput()
{
}

JsonOutput::JsonOutput(const std::string & filename)
    : BasicOutput(filename)
{
}

/* Second reason to add Boost
 *
 *  { "tasks" : [
 *      {
 *      "name" : "Task1",
 *      "description" : "blabla",
 *      "status" : "ok",
 *      "algs" : [
 *          { "name" : "Alg1", "tsv" : "t1_a1.tsv", "status" : "ok" },
 *          { ... }
 *      ]
 *      }, ~~~ Task1
 *      {
 *      ...
 *      }, ~~~ Task2
 *  ] ~~~ tasks
 *  }
 */

template <typename T = std::string>
class NVP
{
public:
    NVP(std::string a, T b)
        : m_a(a)
        , m_b(b)
    {}
    operator std::string() const {
        return "\"" + m_a + "\" : \"" + m_b + "\"";
    }

private:
    const std::string m_a;
    T m_b;
};

void JsonOutput::write()
{
    typedef std::string _s;
    typedef std::vector<std::string> _vs;

    int taskn = 0;
    _vs _taskblock;
    for (const auto & x : TaskCollection::get())
    {
        Task & t = *x.second.get();
        int algn = 0;
        _vs _algblock;
        for (const auto & a : t.get_algorithms())
        {
            Algorithm & alg = *a.second.get();
            _vs _av;
            _av.push_back(NVP<>("name", alg.get_name()));
            _s common_filename("task" + std::to_string(taskn) + "_alg" + std::to_string(algn));
            _s tsv(common_filename + ".tsv");
            if (t.m_status.get_status() != utils::Status::StatusEnum::SE_SKIP)
            {
                TsvOutput to(tsv);
                to.write(alg.m_statistics.m_stat_run);
            }
            _av.push_back(NVP<>("tsv", tsv));
            _av.push_back(NVP<>("common_filename", common_filename));
            _av.push_back(NVP<>("status", alg.m_statistics.m_status.str()));
            _av.push_back(NVP<>("an", std::to_string(algn)));
            _algblock.push_back("\t{" + join(_av) + "}");
            algn++;
        }
        _vs _tv;
        _tv.push_back(NVP<>("name", t.get_name()));
        _tv.push_back(NVP<>("tn", std::to_string(taskn)));
        _tv.push_back(NVP<>("status", t.m_status.str()));
        _tv.push_back(NVP<>("xlabel", t.get_plotsettings().xlabel()));
        _tv.push_back(NVP<>("ylabel", t.get_plotsettings().ylabel()));
        _tv.push_back(NVP<>("scale", t.get_plotsettings().axis_scale_str()));
        _s _as(join(_algblock, ",\n"));
        _tv.push_back("\"algs\" : [\n" + _as + "\n\t]");
        _taskblock.push_back("{\n" + join(_tv, ",\n") + "\n}");
        taskn++;
    }
    _s ret("{ \"tasks\" : [\n" + join(_taskblock, ",\n") + "\n]}\n");
    m_output << ret;
}

} // utils
