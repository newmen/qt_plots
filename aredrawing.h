#ifndef AREDRAWING_H
#define AREDRAWING_H

#include <QColor>
#include <QPainter>
#include <QPoint>
#include <QRect>
#include <QString>
#include "axis.h"
#include "curve.h"

#define MARGIN_PERCENT 0.03
#define MIN_GRID_CELL_SIZE 89

#define TEXT_HALF_HEIGHT 4

template <class ConcreteDerived>
class AreDrawing : public ConcreteDerived
{
public:
    AreDrawing() {}

    void drawAxis(const QRect &contextGeometry, QPainter *painter) const;
    void drawCurves(const QRect &contextGeometry, QPainter *painter) const;

private:
    float translateValue(const Axis &axis, int width, float value) const;
    float translateXValue(const QRect &contextGeometry, float value) const;
    float translateYValue(const QRect &contextGeometry, float value) const;

    void findGridMinMaxStep(const Axis &axis, int width, float *min, float *max, float *step) const;

    void drawGrid(const QRect &contextGeometry, QPainter *painter) const;
    void drawXAxis(const QRect &contextGeometry, QPainter *painter) const;
    void drawYAxis(const QRect &contextGeometry, QPainter *painter) const;
    void drawCurvesInfo(const QRect &contextGeometry, QPainter *painter, QColor *colors) const;

    QPointF *makeCurvePoints(const QRect &contextGeometry, const Curve &curve) const;
    QColor *makeColors(size_t nums) const;
};

template <class ConcreteDerived>
void AreDrawing<ConcreteDerived>::drawAxis(const QRect &contextGeometry, QPainter *painter) const {
    drawGrid(contextGeometry, painter);

    QPen pen(Qt::black);
    pen.setWidth(2);

    painter->save();
    painter->setPen(pen);
    drawXAxis(contextGeometry, painter);
    drawYAxis(contextGeometry, painter);
    painter->restore();
}

template <class ConcreteDerived>
float AreDrawing<ConcreteDerived>::translateValue(const Axis &axis, int width, float value) const {
    float newWidth = width * (1 - 2 * MARGIN_PERCENT);
    return newWidth * value / axis.length() + width * MARGIN_PERCENT;
}

template <class ConcreteDerived>
float AreDrawing<ConcreteDerived>::translateXValue(const QRect &contextGeometry, float value) const {
    const Axis &x = this->_axisX;
    if (value < x.min()) {
        return 0;
    } else if (value > x.max()) {
        return contextGeometry.width();
    } else {
        return translateValue(x, contextGeometry.width(), value - x.min());
    }
}

template <class ConcreteDerived>
float AreDrawing<ConcreteDerived>::translateYValue(const QRect &contextGeometry, float value) const {
    const Axis &y = this->_axisY;
    if (value < y.min()) {
        return contextGeometry.height();
    } else if (value > y.max()) {
        return 0;
    } else {
        return translateValue(y, contextGeometry.height(), y.max() - value);
    }
}

template <class ConcreteDerived>
void AreDrawing<ConcreteDerived>::findGridMinMaxStep(const Axis &axis, int width, float *min, float *max, float *step) const {
    auto ceil = [](float value) { return (int)value; };
    auto floor = [](float value) { return (int)value + 1; };
    float order = axis.orderOfMagnitude() * 0.1;

    *min = floor(axis.min() / order) * order;
    *max = ceil(axis.max() / order) * order;
    float delta = *max - *min;

    // TODO: сделать, чтобы красиво делилось по оси, на "целые" значения
    int nums[] = { 10, 4, 2, 1 };
    for (int num : nums) {
        *step = delta / num;
        if (translateValue(axis, width, *min + *step) - translateValue(axis, width, *min) > MIN_GRID_CELL_SIZE) {
            break;
        }
    }
}

template <class ConcreteDerived>
void AreDrawing<ConcreteDerived>::drawGrid(const QRect &contextGeometry, QPainter *painter) const {
    QPen pen(Qt::black);
    pen.setWidth(1);
    pen.setStyle(Qt::DotLine);

    float min, max, step;
    painter->save();
    painter->setPen(pen);

    auto drawNumberLambda = [&painter](float n, float x, float y, char axis) {
        QString number = QString::number(n);
        const float margin = 3;
        const float halfWidth = TEXT_HALF_HEIGHT * number.length();
        const float halfHeight = TEXT_HALF_HEIGHT;

        QPen pen(Qt::black);
        painter->save();

        (axis == 'y') ? painter->translate(x - halfWidth, y) : painter->translate(x, y + halfHeight);

        painter->setPen(pen);
        painter->drawRect(-margin, margin, 2 * (halfWidth + margin), - 2 * (halfHeight + margin));
        painter->drawText(0, 0, number);

        painter->restore();
    };

    findGridMinMaxStep(this->_axisX, contextGeometry.width(), &min, &max, &step);
    for (float i = min; i <= max; i += step) {
        painter->save();
        painter->translate(translateXValue(contextGeometry, i), contextGeometry.height());
        painter->drawLine(0, 0, 0, -contextGeometry.height());
        drawNumberLambda(i, 0, -15, 'y');
        painter->restore();
    }

    findGridMinMaxStep(this->_axisY, contextGeometry.height(), &min, &max, &step);
    for (float i = min; i <= max; i += step) {
        painter->save();
        painter->translate(0, translateYValue(contextGeometry, i));
        painter->drawLine(0, 0, contextGeometry.width(), 0);
        drawNumberLambda(i, 10, 0, 'x');
        painter->restore();
    }

    painter->restore();
}

template <class ConcreteDerived>
void AreDrawing<ConcreteDerived>::drawXAxis(const QRect &contextGeometry, QPainter *painter) const {
    painter->save();
    painter->translate(0, translateYValue(contextGeometry, 0));
    painter->drawLine(0, 0, contextGeometry.width(), 0);
    painter->restore();
}

template <class ConcreteDerived>
void AreDrawing<ConcreteDerived>::drawYAxis(const QRect &contextGeometry, QPainter *painter) const {
    painter->save();
    painter->translate(translateXValue(contextGeometry, 0), 0);
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

    drawCurvesInfo(contextGeometry, painter, colors);

    delete [] colors;
}

template <class ConcreteDerived>
void AreDrawing<ConcreteDerived>::drawCurvesInfo(const QRect &contextGeometry, QPainter *painter, QColor *colors) const {
    size_t maxNameLength = 0;
    for (const Curve &curve : this->_curves) {
        size_t nameLength = curve.name().length();
        if (nameLength > maxNameLength) maxNameLength = nameLength;
    }

    if (maxNameLength == 0) return;

    const float margin = 10;
    const float previewLength = 21;
    const float spaceLength = 8;
    const float maxTextLength = 6 * maxNameLength;

    const float totalWidth = 2 * margin + previewLength + spaceLength + maxTextLength;
    const float totalHeight = 2 * margin + 2 * TEXT_HALF_HEIGHT * this->_curves.size() + spaceLength * (this->_curves.size() - 1);

    QPen pen(Qt::black);
    painter->save();
    painter->translate(contextGeometry.width() - (totalWidth + margin), margin);
    painter->setPen(pen);

    // рамка
    painter->drawRect(0, 0, totalWidth, totalHeight);

    // кривые
    pen.setWidth(2);
    float xSeek, ySeek;
    float textHalfHeight = TEXT_HALF_HEIGHT;
    for (size_t i = 0; i < this->_curves.size(); i++) {
        xSeek = margin;
        ySeek = margin + i * (2 * textHalfHeight + spaceLength);

        // превью
        painter->save();
        pen.setColor(colors[i]);
        painter->setPen(pen);
        painter->translate(xSeek, ySeek + textHalfHeight);
        painter->drawLine(0, 0, previewLength, 0);
        painter->restore();

        // название
        xSeek += previewLength + spaceLength;
        painter->save();
        painter->translate(xSeek, ySeek + 2 * textHalfHeight);
        painter->drawText(0, 0, this->_curves[i].name().c_str());
        painter->restore();
    }

    painter->restore();
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
