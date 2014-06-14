#include "plotsettings.hpp"

#include <sstream>

namespace utils
{

PlotSettings::PlotSettings()
    : m_xlabel("items")
    , m_ylabel("seconds")
{
}

PlotSettings::PlotSettings(AxisScale a, std::string xlabel)
    : m_axis(a)
    , m_xlabel(xlabel)
    , m_ylabel("seconds")
{
}

std::string PlotSettings::all_for_gnuplot() const
{
    std::stringstream ss;
    switch (m_axis)
    {
    case AS_LOGX:
        ss << "set logscale x 2\n"; break;
    case AS_LOGY:
        ss << "set logscale y 2\n"; break;
    case AS_LOGXY:
        ss << "set logscale xy 2\n"; break;
    case AS_LINEAR:
        ss << "unset logscale\n"; break;
    default:
        // FIXME
        break;
    }
    ss << "set ylabel '" << m_ylabel << "'\n" <<
          "set xlabel '" << m_xlabel << "'\n";
    return ss.str();
}

std::string PlotSettings::xlabel()
{
    return m_xlabel;
}

std::string PlotSettings::ylabel()
{
    return m_ylabel;
}

} // ns utils
