#include <iostream>
#include <cstring>
#include <fstream>
#include <chrono>
#include <vector>
#include <ns3/FilePos.h>

#ifndef COORDINATES_H
#define COORDINATES_H


class CoordinatesReader {
    public:

        const char* coordinatesFilePath;
        const char* originalCoordinatesPath;
        CoordinatesReader();
        CoordinatesReader(const char* path,const char* originalCoordinates);
        void createCoordinatesFile();
        std::vector<std::vector<float>> getInstantCoordinates(int t);
};

#endif
