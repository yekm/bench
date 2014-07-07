#include "gnuplotoutput.hpp"

#include "taskcollection.hpp"

#include <iostream>
#include <iterator>

#include "dsvoutput.hpp"

namespace utils
{

GnuplotOutput::GnuplotOutput()
{
}

GnuplotOutput::GnuplotOutput(const std::string & filename)
    : BasicOutput(filename)
{
}

std::string make_gnuplot(std::string image_filename,
                         std::string plot_title,
                         std::string data_name,
                         std::string fit)
{
    std::string data_filename (data_name + ".data");
    std::ostringstream oss;
    if (!fit.empty())
        oss << "fit " << fit << " '" << data_filename << "' using 1:2:3 via a, b\n";
    oss << "set output '" << image_filename << "'\n"
        << "set title '" << plot_title << "'\n"
        << "plot '" << data_filename << "' using 1:2 with lines title '" << plot_title << "'"
        << ", \\\n\t'" << data_filename << "' using 1:2:3 with errorbars notitle";
    if (!fit.empty())
        oss << ", \\\n\t" << fit << " title sprintf(\"" << fit << "\\na=%g\\nb=%g\", a, b)";
    oss << "\n\n";
    return oss.str();
}

// FIXME: gnuplot's fit wont fit if error is 0
AlgorithmStat::timestat_type gnuplot_happy(const AlgorithmStat::timestat_type & ts)
{
    AlgorithmStat::timestat_type new_ts(ts);
    for (auto & x : new_ts)
    {
        utils::Stat & s = x.second;
        if (s.stddev() == 0)
            s.add(s.mean() + s.mean()/100.);
    }

    return new_ts;
}

void GnuplotOutput::write()
{
    int task = 0;
    for (const auto & x : TaskCollection::get())
    {
        Task & t = *x.second.get();
        m_output << "# Task: " << t.get_name() << "\n\n";
        m_output << t.get_plotsettings().all_for_gnuplot()
                 << "set terminal svg size 700,500 fsize 9\n";
        int algn = 0;
        std::vector<std::string> allinone;
        for (const auto & a : t.get_algorithms())
        {
            Algorithm * alg = a.second.get();
            std::ostringstream oss;
            oss << "task" << task << "_alg" << algn;
            std::string data_name(oss.str());
            std::string data_filename (data_name + ".data");
            if (t.m_status.get_status() != utils::Status::SE_SKIP)
            {
                DsvOutput(data_filename, "\t").write(gnuplot_happy(alg->m_statistics.m_stat_run));
            }
            std::string image_filename(data_name + ".svg");
            std::string title(alg->get_name());
            if (alg->m_statistics.m_status.get_status() != utils::Status::SE_OK)
                title = title + " (" + alg->m_statistics.m_status.str() + ")";
            m_output << make_gnuplot(image_filename, title, data_name, alg->complexity());
            allinone.push_back(" '" + data_filename + "' using 1:2 with lines title '" + title + "', \\\n\t'" +
                               data_filename + "' using 1:2:3 with errorbars notitle");
            algn++;
        }
        m_output
                << "set terminal svg size 1000,800 fsize 8\n"
                << "set output 'allinone_task" << task << ".svg'\n"
                << "set title 'all in one'\n"
                << "plot ";
        m_output << join(allinone, ", \\\n\t") << "\n\n\n";
        task++;
    }
}

/*
void GnuplotOutput::write_html5()
{
    // TODO: do not generate html here
    std::ofstream html("index.html");
    html << "<html><head></head>\n<body>\n";
    int task = 0;
    for (const auto & x : TaskCollection::get())
    {
        Task & t = *x.second.get();
        html << "<h2>" << t.get_name() << "</h2>\n";
        if (!t.m_status.ok())
            html << "<p>Task error: " << t.m_status.str() << "</p>\n";
        m_output << "# Task: " << t.get_name() << "\n\n";
        m_output << t.get_plotsettings().all_for_gnuplot()
                 ;
        //<< "set terminal svg size 700,500 fsize 9\n";
        int algn = 0;
        std::vector<std::string> allinone;
        for (const auto & a : t.get_algorithms())
        {
            Algorithm * alg = a.second.get();
            std::ostringstream oss;
            oss << "task" << task << "_alg" << algn;
            std::string data_name(oss.str());
            std::string data_filename (data_name + ".data");
            if (t.m_status.get_status() != utils::Status::SE_SKIP)
            {
                std::ofstream of(data_filename);
                of << "# task: " << x.first << " :: " << t.get_name() << std::endl;
                of << "# alg: " << alg->get_name() << std::endl;

                for (const auto & n_s : alg->m_statistics.m_stat_run)
                    of << n_s.first << " " << out_stat_gnuplot_happy(n_s.second) << std::endl;
            }
            std::string image_filename(data_name + ".js");
            std::string title(alg->get_name());
            if (alg->m_statistics.m_status.get_status() != utils::Status::SE_OK)
                title = title + " (" + alg->m_statistics.m_status.str() + ")";
            html << "<img width=49% src='" << image_filename << "'>\n";
            m_output << make_gnuplot(image_filename, title, data_name, alg->complexity());
            allinone.push_back(" '" + data_filename + "' using 1:2 with lines title '" + title + "', \\\n\t'" +
                               data_filename + "' using 1:2:3 with errorbars notitle");
            algn++;
        }
//        << "set terminal svg size 700,500 fsize 6\n"
        m_output
                << "set terminal canvas fsize 9 size 1200,800 jsdir '.' name 'allinone_task" << task << "'\n"
                << "set output 'allinone_task" << task << ".js'\n"
                << "set title 'all in one'\n"
                << "plot ";
        html << "<img width=100% src='allinone_task" << task << ".svg'>\n";
        m_output << join(allinone, ", \\\n\t") << "\n\n\n";
        task++;
    }
    html << "</body></html>";
}
*/

} // ns utils
