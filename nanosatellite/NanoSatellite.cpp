#include "NanoSatellite.h"
#include <string>
#include <iostream>
#include <vector>
#include <cmath>


TimeCoordinates::TimeCoordinates(float x, float y, float z,int timeInstant) {
    this -> x = x;
    this -> y = y;
    this -> z = z;
    this -> timeInstant = timeInstant;
}

int TimeCoordinates::getTimeInstant() {
    return timeInstant;
}

std::vector<float> TimeCoordinates::getCoordinates() {
    return std::vector<float>({x,y,z});
}

void TimeCoordinates::setCoordinates(std::vector<float> newCoords, int t) {
    x = newCoords[0];
    y = newCoords[1];
    z = newCoords[2];
    timeInstant = t;
}


std::ostream& operator << (std::ostream& consoleOutput,TimeCoordinates* coords) {

    consoleOutput << "(" << (coords->getCoordinates())[0] << ", " << (coords->getCoordinates())[1]
    << ", " << (coords->getCoordinates())[2] << ") t = " << (coords->getTimeInstant());
    return consoleOutput;    
}


NanoSatellite::NanoSatellite(int id, std::string name,TimeCoordinates* initialCoordinates) {
    satelliteID = id;
    satelliteName = name;
    satelliteGroup.first = -1;
    satelliteCoordinates = initialCoordinates;
}

NanoSatellite::~NanoSatellite() {
    delete satelliteCoordinates;
}


float NanoSatellite::getPropagationTime(NanoSatellite* satellite) {
    return (distance(satellite -> getTimeCoordinates())/(float) LIGHT_SPEED);
}

float NanoSatellite::distance(TimeCoordinates* coords) {
    float x1 = (satelliteCoordinates -> getCoordinates())[0];
    float y1 = (satelliteCoordinates -> getCoordinates())[1];
    float z1 = (satelliteCoordinates -> getCoordinates())[2];
    float x2 = (coords -> getCoordinates())[0];
    float y2 = (coords -> getCoordinates())[1];
    float z2 = (coords -> getCoordinates())[2];
    return std::sqrt(std::pow(x1-x2,2) + std::pow(y1-y2,2) + std::pow(z1-z2,2));
}

bool NanoSatellite::inGroup() {
    return satelliteGroup.first >= 0;
}

std::vector<int> NanoSatellite::getGroup() {
    return satelliteGroup.second;
}

void NanoSatellite::setGroup(int groupNumber, std::vector<int> group) {
    satelliteGroup.first = groupNumber;
    satelliteGroup.second = group;
}

TimeCoordinates * NanoSatellite::getTimeCoordinates() {
    return satelliteCoordinates;
}

void NanoSatellite::setReachableNode(int nodeID, char mode) {
    switch (mode) {
        case 'a': {
            reachableNodes.push_back(nodeID);
        }
        break;
        case 'r': {
            std::vector<int>::iterator iter = reachableNodes.begin();
            for(int k; (std::size_t) k <= reachableNodes.size(); k++) {
                if(reachableNodes[k] == nodeID) {
                    reachableNodes.erase(iter+k);
                    return;
                }
            }
        }
        break;
        default:
        break;
    }
}

void NanoSatellite::setCoordinates(TimeCoordinates* newCoordinates) {
    satelliteCoordinates = newCoordinates;
}

std::ostream& operator << (std::ostream& consoleOutput, NanoSatellite* nanoSatellite) {
    std::string nodes = "[";
    if(nanoSatellite -> reachableNodes.size() == 0) {
        nodes += "]";
    } else {
        for(int k = 1; (std::size_t) k <= nanoSatellite -> reachableNodes.size() - 1; k++) {
            nodes += std::to_string(nanoSatellite -> reachableNodes[k-1]) + ", ";
        }
        nodes += std::to_string(nanoSatellite -> reachableNodes[nanoSatellite -> reachableNodes.size() - 1]) + "]";
    }
    std::string group = "[";
    if(nanoSatellite -> satelliteGroup.first < 0) {
        group += "]";
    } else {
        for(int k = 1; (std::size_t) k <= nanoSatellite -> satelliteGroup.second.size() - 1; k++) {
            group += std::to_string(nanoSatellite->satelliteGroup.second[k-1]) + ", ";
        }
        group += std::to_string(nanoSatellite -> satelliteGroup.second[nanoSatellite -> satelliteGroup.second.size() - 1]) + "]";
    }
    consoleOutput << "[ ID: "
                  << nanoSatellite -> satelliteID << "\n"
                  << "  NAME : " << nanoSatellite -> satelliteName  << "\n" 
                  << "  MASTER_ID : " << nanoSatellite -> masterNode -> satelliteID << "\n"
                  << "  GROUP : " << nanoSatellite -> satelliteGroup.first << " " << group << " \n"
                  << "  REACHABLE_NODES : " << nodes << "\n"
                  << "  COORDINATES : " << nanoSatellite -> getTimeCoordinates() << " ]";
    return consoleOutput;
}

std::ostream& operator << (std::ostream& consoleOutput,std::vector<int>& reachableNodes) {
    consoleOutput << "[";
    for(int k = 0; (std::size_t) k <= reachableNodes.size() - 2; k++) {
        consoleOutput << reachableNodes[k] << ", ";
    }
    consoleOutput << reachableNodes[reachableNodes.size() - 1] << "]";
    return consoleOutput;
}

/*
int main() {
    TimeCoordinates* coords = new TimeCoordinates(0,0,0,0);
    NanoSatellite nanoSatellite(0,"N7_0",coords);
    std::vector<int> nodes({1,2,10});
    nanoSatellite.reachableNodes = nodes;
    std::cout << &nodes << "\n";
    return 0;
}
*/
