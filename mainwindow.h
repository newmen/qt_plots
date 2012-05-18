#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include "curvespool.h"
#include "renderarea.h"

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    MainWindow();
    ~MainWindow();

    void readAndDraw(const char *fileName);
    
private:
    CurvesPool *_curvesPool;
    RenderArea *_renderArea;
};

#endif // MAINWINDOW_H
