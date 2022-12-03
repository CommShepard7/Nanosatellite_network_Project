#include "Constellation.h"
#include <cstdlib>
#include <random>
#include <ctime>
#include <chrono>

bool checkGroup(std::vector<int> group,  int id) {
    bool inGroup = false;
    for(int k = 0; (std::size_t) k <= group.size() - 1; k++) {
        if(group[k] == id) {
            inGroup = true;
        }
    }
    return inGroup;
}

Constellation::Constellation(int size) {
    coordinatesReader = new CoordinatesReader();
    linksReader = new LinksReader();
    constellationSize = size;
}

Constellation::Constellation() {
    coordinatesReader = new CoordinatesReader();
    linksReader = new LinksReader();
    constellationSize = 100;
}

Constellation::~Constellation() {
    delete coordinatesReader;
    delete linksReader;
    for(int k = 0; k <= constellationSize - 1; k++) {
        delete satellites[k];
    }
}

void Constellation::initSatellites() {
    std::vector<std::vector<float>> coordsVector = coordinatesReader -> getInstantCoordinates(0);
    for(int k = 0; k <= constellationSize - 1; k++) {
        TimeCoordinates* coords = new TimeCoordinates(coordsVector[k][0],coordsVector[k][1],coordsVector[k][2],0);
        NanoSatellite* nanoSatellite = new NanoSatellite(k,"N7_"+std::to_string(k),coords);
        nanoSatellite-> masterNode = nanoSatellite;
        satellites.push_back(nanoSatellite);
    }
}

void Constellation::updateCoordinates(int t) {
    std::vector<std::vector<float>> coordsVector = coordinatesReader -> getInstantCoordinates(t);
    for(int k = 0; k <= constellationSize - 1; k++) {
        satellites[k] -> satelliteCoordinates->setCoordinates(coordsVector[k],t);
    }
}

void Constellation::updateLinks(int maxRange) {
    for(int k = 0; k <= constellationSize - 1; k++) {
       satellites[k] -> reachableNodes.clear();
       for(int i = 0; i <= constellationSize - 1; i++) {
            TimeCoordinates* coords = satellites[i] -> getTimeCoordinates();
            if(satellites[k] -> distance(coords) <= maxRange*1000 && i != k) {
                satellites[k]->setReachableNode(i,'a');
            }
        }
    }
}

void Constellation::createLinks() {
    linksReader->createLinksFile(this,20);
}

void Constellation::setLinks(int t) {
    std::vector<std::vector<int>> links = linksReader->getInstantLinks(t);
    for(int k = 0; k <= constellationSize - 1; k++) {
        satellites[k]->reachableNodes = links[k];
    }
}

void Constellation::setRandomGroups() {
    //std::srand(std::time(0));
    std::srand(10);
    int groupNumber = 0;
    std::vector<int> group;
    for(NanoSatellite* nanoSatellite : satellites) {
        bool groupComplete = nanoSatellite->inGroup();
        int randomSatellite;
        if(!groupComplete) {
            group.push_back(nanoSatellite->satelliteID);
            while(!groupComplete) {
                randomSatellite = std::rand() % 100;
                if(!satellites[randomSatellite]->inGroup() && !checkGroup(group,randomSatellite)) {
                    group.push_back(randomSatellite);
                }
                groupComplete = (group.size() == 10);
            }
            for(int k = 0; k < 10; k++) {
                satellites[group[k]]->setGroup(groupNumber,group);
            }
            group.clear();
            groupNumber++;
        }
    }
}

/*
std::map<int,int> Constellation::setReachableNodes(int maxRange) {
    std::map<int,int> reachableNodesNumber;
    for(int k = 0; k <= constellationSize - 1; k++) {
       int nodesNumber = 0;
       for(int i = 0; i <= constellationSize - 1; i++) {
            TimeCoordinates* coords = satellites[i] -> getTimeCoordinates();
            if(satellites[k] -> distance(coords) <= maxRange*1000 && i != k) {
                satellites[k]->setReachableNode(i,'a');
                nodesNumber++;
            }
        }
        reachableNodesNumber[k] = nodesNumber;
    }
    return reachableNodesNumber;
}
*/

/*
void Constellation::setInitialGroups(int maxRange) {
    std::map<int,int> reachableNodesNumber = setReachableNodes(maxRange);
    std::map<int,std::vector<int>> groups;
    int groupNumber = 0;
    for(int k = 0; k <= constellationSize - 1; k++) {
        NanoSatellite* currentSatellite = satellites[k];
        if(currentSatellite->reachableNodes.size() > 0) {
            int reachableSetSize = currentSatellite -> reachableNodes.size();
            bool groupComplete = false;
            bool newGroupCreated = false;
            int satelliteCount = 0;
            int lastGroup = 0;
            std::vector<int> currentNodes = currentSatellite -> reachableNodes;
            std::vector<int> nodes = currentNodes;
            while(!groupComplete) {
                int nodesInGroups = 0;
                std::vector<int> nodesGroups;
                for(int id : nodes) {
                    std::cout<< k << " : " << id << " " << satellites[id]->inGroup() << " " << satellites[id]->satelliteGroup.first <<   "\n";
                    if(!satellites[id]->inGroup()) {
                        groups[groupNumber].push_back(id);
                        satellites[id]->setGroup(groupNumber);
                    } else {
                        //std::cout<< satellites[id]->satelliteName + " : " << satellites[id]->satelliteGroup.first << "\n";
                        nodesInGroups++;
                    }
                    if(groups[groupNumber].size() == 10) {
                        break;
                    }
                }
                lastGroup = satellites[nodes[0]]->satelliteGroup.first;
                nodes = satellites[currentNodes[satelliteCount]]->reachableNodes;
                satelliteCount++;
                groupComplete = (groups[groupNumber].size() == 10 || satelliteCount == reachableSetSize);
                if(groups[groupNumber].size() == 0 && groupComplete) {
                    std::cout<< k << " : JOINING "  << lastGroup << "\n";
                    if(!checkGroup(groups[lastGroup],currentSatellite->satelliteID)) {
                        groups[lastGroup].push_back(currentSatellite->satelliteID);
                    }
                    currentSatellite->setGroup(lastGroup);
                }
            }
            if(!newGroupCreated) {
                groupNumber++;
            }
        }
    }
    int groupSize = 0;
    for(int k = 0; k <= constellationSize - 1; k++) {
        NanoSatellite* currentSatellite = satellites[k];
        int groupNumber = currentSatellite->satelliteGroup.first;
        groupSize += groups[k].size();
        if(groupNumber >= 0) {
            currentSatellite->setGroup(groupNumber,groups[groupNumber]);
        }
        //std::cout << k << " : " << groups[k].size() << "\n";
        std::cout << satellites[k] << "\n";
    }
     std::cout<< groupSize << "\n";
}
*/

