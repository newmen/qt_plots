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

    string line;
    while (getline(_file, line)) {
        fillingPool->parseStrLine(line);
    }
}
