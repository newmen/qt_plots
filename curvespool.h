#ifndef CURVESPOOL_H
#define CURVESPOOL_H

#include <vector>
#include "axis.h"
#include "curve.h"

class CurvesPool
{
public:
    CurvesPool();

protected:
    Axis _axisX, _axisY;
    std::vector<Curve> _curves;
    std::vector<float> _xValues;
};

#endif // CURVESPOOL_H
