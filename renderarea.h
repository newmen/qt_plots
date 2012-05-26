#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QWidget>
#include "curvespool.h"

class RenderArea : public QWidget
{
    Q_OBJECT
public:
    RenderArea(const CurvesPool *pool);

protected:
    void paintEvent(QPaintEvent *);

private:
    const CurvesPool *_pool;
};

#endif // RENDERAREA_H
