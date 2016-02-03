#include "plotsettings.hpp"

#include <sstream>
#include <stdexcept>

namespace utils
{

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

void PlotSettings::set_axis_scale(PlotSettings::AxisScale a)
{
    m_axis = a;
}

void PlotSettings::set_plot_type(PlotSettings::PlotType t)
{
    m_plottype = t;
}

std::string PlotSettings::all_for_gnuplot() const
{
    std::stringstream ss;
    switch (m_axis)
    {
    case AxisScale::AS_LOGX:
        ss << "set logscale x 2\n"; break;
    case AxisScale::AS_LOGY:
        ss << "set logscale y 2\n"; break;
    case AxisScale::AS_LOGXY:
        ss << "set logscale xy 2\n"; break;
    case AxisScale::AS_LINEAR:
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
    case AxisScale::AS_LOGX:
        return "log";
    case AxisScale::AS_LOGY:
        return "log";
    case AxisScale::AS_LOGXY:
        return "log";
    case AxisScale::AS_LINEAR:
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
