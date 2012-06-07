#include "filereader.h"
#include "arefilling.h"

#include <iostream>

FileReader::FileReader(const char *filename) : _file(filename) {
    if (!_file) {
        std::cerr << "Ошибка чтения файла " << filename << std::endl;
    }
}

void FileReader::read(CurvesPool *curvesPool) {
    AreFilling<CurvesPool> *fillingPool = static_cast<AreFilling<CurvesPool> *>(curvesPool);

    std::string line;
    while (getline(_file, line)) {
        fillingPool->parseStrLine(line);
    }
}
