#include "renderarea.h"
#include "aredrawing.h"
#include <QtGui>

RenderArea::RenderArea(const CurvesPool *pool) : _pool(pool) {
}

void RenderArea::paintEvent(QPaintEvent *) {
    QPainter *painter = new QPainter(this);

    painter->setPen(Qt::white);
    painter->setBrush(Qt::white);
    painter->drawRect(0, 0, width(), height());

    const AreDrawing<CurvesPool> *drawingPool = static_cast<const AreDrawing<CurvesPool> *>(_pool);
    drawingPool->drawAxis(geometry(), painter);
    drawingPool->drawCurves(geometry(), painter);

    delete painter;
}
