#ifndef PLOTSETTINGS_HPP
#define PLOTSETTINGS_HPP

#include <string>

namespace utils
{

class PlotSettings
{
public:
    enum class PlotType
    {
        PT_LINES,
        PT_BARS,
    };

    enum class AxisScale
    {
        AS_LINEAR,
        AS_LOGX,
        AS_LOGY,
        AS_LOGXY,
    };

    explicit PlotSettings(AxisScale a);
    PlotSettings(AxisScale a, std::string xlabel);

    PlotSettings& operator=(const PlotSettings&) = default;

    void set_axis_scale(AxisScale a);
    void set_plot_type(PlotType t);

    std::string xlabel() const;
    std::string ylabel() const;

    std::string all_for_gnuplot() const;

    std::string axis_scale_str() const;
    std::string plot_type_str() const;
private:

    AxisScale m_axis;
    PlotType m_plottype = PlotType::PT_LINES;
    std::string m_xlabel, m_ylabel;
};

} // ns utils

#endif // PLOTSETTINGS_HPP
