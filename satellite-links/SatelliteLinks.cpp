#include "SatelliteLinks.h"


void LinksReader::createLinksFile(Constellation* satelliteConstellation,int maxRange) {
    if(!std::fopen("coordinates/satelliteLinks.txt","r")) {
        std::cout << "Creating satellite links file. \n";
        std::string line;
        std::fstream satelliteLinks("coordinates/satelliteLinks.txt", std::ios::out | std::ios::trunc);
        FilePos filePos(9999,100,10000);
        for(int k = 0; k <= 9998; k++) {
            std::string emptyString(9999,' ');
            emptyString.push_back('\n');
            satelliteLinks << emptyString;
        }
        filePos.updatePos(&satelliteLinks);
        for(int t = 0; t <= 9998; t++) {
            satelliteConstellation->updateCoordinates(t);
            satelliteConstellation->updateLinks(maxRange);
            for(NanoSatellite* nanoSatellite : satelliteConstellation->satellites) {
                line = "[";
                int nodeListSize = nanoSatellite->reachableNodes.size();
                for(int nodeID : nanoSatellite->reachableNodes) {
                    line += std::to_string(nodeID);
                    if(!(nodeID == nanoSatellite->reachableNodes[nodeListSize - 1])) {
                        line += ", ";
                    }
                }
                line += "];";
                satelliteLinks << line;
            }
            line.clear();
            filePos.updatePos(&satelliteLinks);
        }
        satelliteLinks.close();
    } else {
        std::cout << "Satellite links file already exists. \n";
    }
}


std::vector<std::vector<int>> LinksReader::getInstantLinks(int t) {
    std::vector<std::vector<int>> links;
    std::string line;
    std::ifstream satelliteLinks("coordinates/satelliteLinks.txt");
    bool lineFound = false;
    int satelliteCount = 0;
    satelliteLinks.seekg(10000*t);
    while(std::getline(satelliteLinks,line,';') && !lineFound) {
        std::vector<int> linkArray;
        line.erase(0,1);
        line.erase(line.length()-1,line.length());
        if(line.length() > 0) {
            while(line.find(',') != std::string::npos) {
                linkArray.push_back(std::stoi(line.substr(0,line.find(','))));
                line.erase(0,line.find(',')+2);
            }
            linkArray.push_back(std::stoi(line));
        }
        links.push_back(linkArray);
        if(++satelliteCount == 100) {
            lineFound = true;
        }
    }
    satelliteLinks.close();
    return links;
}
