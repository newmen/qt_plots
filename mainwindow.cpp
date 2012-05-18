#include "mainwindow.h"
#include "filereader.h"
#include <QtGui>

MainWindow::MainWindow() {
    _curvesPool = new CurvesPool();
    _renderArea = new RenderArea(_curvesPool);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(_renderArea);
    setLayout(layout);

    setGeometry(0, 0, 1000, 700);
}

MainWindow::~MainWindow() {
    delete _renderArea;
    delete _curvesPool;
}

void MainWindow::readAndDraw(const char *fileName) {
    FileReader fr(fileName);
    fr.read(_curvesPool);

    _renderArea->update();
}

