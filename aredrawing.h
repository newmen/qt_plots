#ifndef AREDRAWING_H
#define AREDRAWING_H

#include <QPainter>
#include <QPoint>
#include <QRect>

#define MARGIN_PERCENT 0.08

template <class ConcreteDerived>
class AreDrawing : public ConcreteDerived
{
public:
    AreDrawing() {}

    void drawAxis(const QRect &contextGeometry, QPainter *painter) const;
    void drawCurves(const QRect &contextGeometry, QPainter *painter) const;

private:
    float translateXValue(const QRect &contextGeometry, float value) const;
    float translateYValue(const QRect &contextGeometry, float value) const;

    void drawXAxis(const QRect &contextGeometry, QPainter *painter) const;
    void drawYAxis(const QRect &contextGeometry, QPainter *painter) const;

    QPointF *makeCurvePoints(const QRect &contextGeometry, const Curve &curve) const;
};

template <class ConcreteDerived>
void AreDrawing<ConcreteDerived>::drawAxis(const QRect &contextGeometry, QPainter *painter) const {
    drawXAxis(contextGeometry, painter);
    drawYAxis(contextGeometry, painter);
}

template <class ConcreteDerived>
float AreDrawing<ConcreteDerived>::translateXValue(const QRect &contextGeometry, float value) const {
    float width = contextGeometry.width() * (1 - 2 * MARGIN_PERCENT);
    const Axis &x = this->_axisX;
    return width * (value - x.min()) / (x.max() - x.min()) + contextGeometry.width() * MARGIN_PERCENT;
}

template <class ConcreteDerived>
float AreDrawing<ConcreteDerived>::translateYValue(const QRect &contextGeometry, float value) const {
    float height = contextGeometry.height() * (1 - 2 * MARGIN_PERCENT);
    const Axis &y = this->_axisY;
    return height * (y.max() - value) / (y.max() - y.min()) + contextGeometry.height() * MARGIN_PERCENT;
}

template <class ConcreteDerived>
void AreDrawing<ConcreteDerived>::drawXAxis(const QRect &contextGeometry, QPainter *painter) const {
    painter->save();
    painter->translate(0, translateYValue(contextGeometry, this->_axisY.zero()));
    painter->drawLine(0, 0, contextGeometry.width(), 0);
    painter->restore();
}

template <class ConcreteDerived>
void AreDrawing<ConcreteDerived>::drawYAxis(const QRect &contextGeometry, QPainter *painter) const {
    painter->save();
    painter->translate(translateXValue(contextGeometry, this->_axisX.zero()), 0);
    painter->drawLine(0, 0, 0, contextGeometry.height());
    painter->restore();
}

template <class ConcreteDerived>
void AreDrawing<ConcreteDerived>::drawCurves(const QRect &contextGeometry, QPainter *painter) const {
    QPointF *curvePoints;
    for (const Curve &curve : this->_curves) {
        curvePoints = makeCurvePoints(contextGeometry, curve);
        painter->drawPolyline(curvePoints, this->_xValues.size());
        delete [] curvePoints;
    }
}

template <class ConcreteDerived>
QPointF *AreDrawing<ConcreteDerived>::makeCurvePoints(const QRect &contextGeometry, const Curve &curve) const {
    QPointF *curvePoints = new QPointF[this->_xValues.size()];
    for (size_t i = 0; i < this->_xValues.size(); i++) {
        curvePoints[i] = QPoint(translateXValue(contextGeometry, this->_xValues[i]),
                                translateYValue(contextGeometry, curve.values()[i]));
    }

    return curvePoints;
}

#endif // AREDRAWING_H
