#ifndef PLOTSETTINGS_HPP
#define PLOTSETTINGS_HPP

#include <string>

namespace utils
{

class PlotSettings
{
public:
    enum AxisScale
    {
        AS_LINEAR,
        AS_LOGX,
        AS_LOGY,
        AS_LOGXY,
    };

    PlotSettings();
    PlotSettings(AxisScale a);
    PlotSettings(AxisScale a, std::string xlabel);

    std::string xlabel() const;
    std::string ylabel() const;

    std::string all_for_gnuplot() const;
private:

    AxisScale m_axis;
    const std::string m_xlabel, m_ylabel;
};

} // ns utils

#endif // PLOTSETTINGS_HPP
