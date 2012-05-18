#ifndef AREDRAWING_H
#define AREDRAWING_H

#include <QColor>
#include <QPainter>
#include <QPoint>
#include <QRect>

#define MARGIN_PERCENT 0.03

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
    QColor *makeColors(size_t nums) const;
};

template <class ConcreteDerived>
void AreDrawing<ConcreteDerived>::drawAxis(const QRect &contextGeometry, QPainter *painter) const {
    QPen pen(Qt::black);
    pen.setWidth(2);

    painter->save();
    painter->setPen(pen);
    drawXAxis(contextGeometry, painter);
    drawYAxis(contextGeometry, painter);
    painter->restore();
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
    QPen pen;
    pen.setWidth(2);

    QColor *colors = makeColors(this->_curves.size());

    painter->save();

    QPointF *curvePoints;
    int colorIndex = 0;
    for (const Curve &curve : this->_curves) {
        curvePoints = makeCurvePoints(contextGeometry, curve);
        pen.setColor(colors[colorIndex++]);
        painter->setPen(pen);
        painter->drawPolyline(curvePoints, this->_xValues.size());
        delete [] curvePoints;
    }

    painter->restore();

    delete [] colors;
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

template <class ConcreteDerived>
QColor *AreDrawing<ConcreteDerived>::makeColors(size_t nums) const {
    float part = 5.f / nums;

    float mod;
    auto modUp = [&mod]() { return mod * 255; };
    auto modDown = [&mod]() { return 255 * (1 - mod); };

    QColor *colors = new QColor[nums];
    for (size_t i = 0; i < nums; i++) {
        float currPart = i * part;
        mod = currPart - (int)currPart;

        if (currPart < 1.f) colors[i].setRgb(255, modUp(), 0);
        else if (currPart >= 1.f && currPart < 2.f) colors[i].setRgb(modDown(), 255, 0);
        else if (currPart >= 2.f && currPart < 3.f) colors[i].setRgb(0, 255, modUp());
        else if (currPart >= 3.f && currPart < 4.f) colors[i].setRgb(0, modDown(), 255);
        else if (currPart >= 4.f) colors[i].setRgb(modUp(), 0, 255);
    }

    return colors;
}

#endif // AREDRAWING_H
