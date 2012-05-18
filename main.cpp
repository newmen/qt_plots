#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    MainWindow mainWindow;
    mainWindow.show();
    mainWindow.readAndDraw("/home/newmen/c++/Qt/qt_plots/input_file.txt");

    return app.exec();
}
