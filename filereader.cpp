#include "filereader.h"
#include "arefilling.h"

#include <iostream>

using namespace std;

FileReader::FileReader(const char *filename) : _file(filename) {
    if (!_file) {
        std::cerr << "Ошибка чтения файла " << filename << std::endl;
    }
}

void FileReader::read(CurvesPool *curvesPool) {
    AreFilling<CurvesPool> *fillingPool = (AreFilling<CurvesPool> *)curvesPool;

    char xAxisName[32], yAxisName[32];
    _file >> xAxisName >> yAxisName;
    fillingPool->setAxisNames(xAxisName, yAxisName);

    string line;
    while (getline(_file, line)) {
        fillingPool->parseStrLine(line);
    }
}
