#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QWidget>
#include "curvespool.h"

class RenderArea : public QWidget
{
    Q_OBJECT
public:
    RenderArea(CurvesPool *pool);

protected:
    void paintEvent(QPaintEvent *);

private:
    CurvesPool *_pool;
};

#endif // RENDERAREA_H
