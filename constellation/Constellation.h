#include "ns3/NanoSatellite.h"
#include "ns3/Coordinates.h"
#include <string>
#include <map>
#include <vector>

#ifndef CONSTELLATION_H
#define CONSTELLATION_H

#include "ns3/SatelliteLinks.h"

class LinksReader;

class Constellation {

public:

    std::vector<NanoSatellite*> satellites;
    std::map<int, std::vector<int>> satelliteGroups;
    std::vector<int> groupMasters;
    int constellationSize;
    CoordinatesReader* coordinatesReader;
    LinksReader* linksReader;
    Constellation(int size);
    Constellation();
    ~Constellation();
    void initSatellites();
    void setGroups();
    std::map<int,int> setReachableNodes(int maxRange);
    void setInitialGroups(int maxRange);
    void setRandomGroups();
    void setReachableNodes(NanoSatellite* satellite,int connectionRange);
    void updateCoordinates(int t);
    void updateLinks(int maxRange);
    void createLinks();
    void setLinks(int t);
    void updateGroups();
};
#endif
