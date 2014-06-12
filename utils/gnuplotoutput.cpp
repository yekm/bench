#include "gnuplotoutput.h"

#include "taskcollection.h"

#include <iostream>
#include <sstream>
#include <iterator>

namespace utils
{

GnuplotOutput::GnuplotOutput()
    : m_output(std::cout.rdbuf())
{
}

GnuplotOutput::GnuplotOutput(const std::string & filename)
    : m_output_file(filename.c_str())
    , m_output(m_output_file.rdbuf())
{
}

std::string make_gnuplot(std::string image_filename,
                         std::string plot_title,
                         std::string data_filename,
                         std::string fit)
{
    std::ostringstream oss;
    if (!fit.empty())
        oss << "fit " << fit << " '" << data_filename << "' using 1:2:3 via a, b\n";
    oss << "set terminal svg size 700,500 fsize 9\n"
        << "set xlabel 'items'\n"
        << "set ylabel 'seconds'\n"
        << "set output '" << image_filename << "'\n"
        << "set title '" << plot_title << "'\n"
        << "set logscale xy 2\n"
        << "plot '" << data_filename << "' using 1:2 with lines title '" << plot_title << "'"
        << ", '" << data_filename << "' using 1:2:3 with errorbars notitle";
    if (!fit.empty())
        oss << ", " << fit << " title sprintf(\"" << fit << "\\na=%g\\nb=%g\", a, b)";
    oss << "\n\n";
    return oss.str();
}

void GnuplotOutput::write()
{
    // TODO: do not generate html here
    std::ofstream html("index.html");
    html << "<html><head></head>\n<body>\n";
    int task = 0;
    for (const auto & x : TaskCollection::get())
    {
        Task & t = *x.second.get();
        html << "<h2>" << t.get_name() << "</h2>\n";
        int algn = 0;
        std::vector<std::string> allinone;
        for (const auto & a : t.get_algorithms())
        {
            std::ostringstream oss;
            oss << "task" << task << "_alg" << algn << ".data";
            std::ofstream of(oss.str());
            of << "# task: " << x.first << " :: " << t.get_name() << std::endl;
            Algorithm * alg = a.second.get();
            of << "# alg: " << alg->get_name() << std::endl;

            for (const auto & n_s : alg->m_statistics.m_stat_run)
            {
                of << n_s.first << " "
                         << n_s.second.mean() << " "
                         << n_s.second.stddev() << " "
                         << n_s.second.min() << " "
                         << n_s.second.max() << std::endl;
            }
            std::string image_filename(oss.str() + ".svg");
            std::string title(alg->get_name());
            if (alg->m_statistics.m_status.get_status() != utils::Status::SE_OK)
                title = title + " (" + alg->m_statistics.m_status.str() + ")";
            html << "<img width=49% src='" << image_filename << "'>\n";
            m_output << make_gnuplot(image_filename, title, oss.str(), alg->complexity());
            allinone.push_back(" '" + oss.str() + "' using 1:2 with lines title '" + title + "', '" + oss.str() + "' using 1:2:3 with errorbars notitle");
            algn++;
        }
        m_output
                << "set terminal svg size 700,500 fsize 6\n"
                << "set output 'allinone_task" << task << ".svg'\n"
                << "set title 'all in one'\n"
                << "set logscale xy 2\n"
                << "plot ";
        html << "<img width=100% src='allinone_task" << task << ".svg'>\n";
        std::copy(allinone.begin(), allinone.end(), std::ostream_iterator<std::string>(m_output, ","));
        task++;
    }
    html << "</body></html>";
}

} // ns utils
