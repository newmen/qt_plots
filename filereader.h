#ifndef FILEREADER_H
#define FILEREADER_H

#include <fstream>
#include "curvespool.h"

class FileReader
{
public:
    FileReader(const char *filename);
    void read(CurvesPool *curvesPool);

private:
    std::ifstream _file;
};

#endif // FILEREADER_H
