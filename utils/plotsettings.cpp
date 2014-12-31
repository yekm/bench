#include "plotsettings.hpp"

#include <sstream>
#include <stdexcept>

namespace utils
{

PlotSettings::PlotSettings()
    : m_xlabel("N (number of elements)")
    , m_ylabel("Time (sec)")
{
}

PlotSettings::PlotSettings(PlotSettings::AxisScale a)
    : m_axis(a)
    , m_xlabel("N (number of elements)")
    , m_ylabel("Time (sec)")
{
}

PlotSettings::PlotSettings(AxisScale a, std::string xlabel)
    : m_axis(a)
    , m_xlabel(xlabel)
    , m_ylabel("Time (sec)")
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

std::string PlotSettings::axis_scale_str() const
{
    switch (m_axis)
    {
    case AS_LOGX:
        return "log";
    case AS_LOGY:
        return "log";
    case AS_LOGXY:
        return "log";
    case AS_LINEAR:
        return "linear";
    default:
        throw std::runtime_error("PlotSettings::axis_scale_str unimplemented scale");
        break;
    }
}

std::string PlotSettings::xlabel() const
{
    return m_xlabel;
}

std::string PlotSettings::ylabel() const
{
    return m_ylabel;
}

} // ns utils
