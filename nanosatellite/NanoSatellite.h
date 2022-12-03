#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>
#include <map>


#ifndef NANOSATELLITE_H
#define NANOSATELLITE_H

#define LIGHT_SPEED 299792458


class TimeCoordinates {
    public:

        int timeInstant = 0;
        float x;
        float y;
        float z;

    TimeCoordinates();
    TimeCoordinates(float x, float y, float z, int timeInstant);
    int getTimeInstant();
    std::vector<float> getCoordinates();
    void setCoordinates(std::vector<float> newCoords,int t);
    friend std::ostream& operator <<(std::ostream& consoleOutput, TimeCoordinates* coords);
};


class NanoSatellite {

    public:

        int satelliteID;
        std::pair<int,std::vector<int>> satelliteGroup;
        TimeCoordinates* satelliteCoordinates;
        NanoSatellite* masterNode;
        std::vector<int> reachableNodes;        
        std::string satelliteName;

        NanoSatellite(int id, std::string name,TimeCoordinates * initialCoordinates);
        ~NanoSatellite();
        int getSatelliteID();
        int getReachableNodesNumber();
        bool inGroup();
        std::vector<int> getGroup();
        void setGroup(int groupNumber,std::vector<int> group = {});
        float getPropagationTime(NanoSatellite* satellite);
        float distance(TimeCoordinates* coords2);
        TimeCoordinates * getTimeCoordinates();
        void setCoordinates(TimeCoordinates* newCoordinates);
        void setReachableNode(int nodeID, char mode);
        friend std::ostream& operator << (std::ostream& consoleOutput, NanoSatellite* nanoSatellite);
};      

#endif