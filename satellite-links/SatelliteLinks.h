#include <fstream>
#include <cstring>
#include <chrono>
#include <vector>
#include <ns3/FilePos.h>

#ifndef SATELLITES_LINKS_H
#define SATELLITES_LINKS_H

#include <ns3/Constellation.h>
class Constellation;

class LinksReader {
    public: 

        void createLinksFile(Constellation* satelliteConstellation, int maxRange);
        std::vector<std::vector<int>> getInstantLinks(int t);
};
#endif