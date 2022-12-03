#include "Coordinates.h"


CoordinatesReader::CoordinatesReader(const char* coordinatesPath,const char* originalCoordinates) {
    originalCoordinatesPath = originalCoordinates;
    coordinatesFilePath = coordinatesPath;
}

CoordinatesReader::CoordinatesReader() {
    originalCoordinatesPath = "coordinates/Traces.csv";
    coordinatesFilePath =  "coordinates/reshapedTraces.txt";
}

void CoordinatesReader::createCoordinatesFile() {
     if(!std::fopen(coordinatesFilePath,"r")) {
        std::cout << "Creating coordinates file.\n";
        std::string line;
        std::ifstream coordinatesFile (originalCoordinatesPath);
        std::fstream newCoordinatesFile(coordinatesFilePath, std::ios::out | std::ios::trunc);
        FilePos filePos(9999,20,7500);
        for(int k = 0; k <= 9998; k++) {
            std::string emptyString(7499,' ');
            emptyString.push_back('\n');
            newCoordinatesFile << emptyString;
        }
        filePos.updatePos(&newCoordinatesFile);
        std::vector<std::string> coordsVector;
        char coordDelimiter;
        for(int k = 0; k <= 299; k++) {
            std::getline(coordinatesFile, line);
            coordsVector = FilePos::stringSplit(line,',');
            for(int i = 0; (std::size_t) i <= coordsVector.size()-1; i++) {
                if((k+1)%3 == 0) {
                    coordDelimiter = ';';
                } else {
                    coordDelimiter = ' ';
                }
                newCoordinatesFile << coordsVector[i] << ',' << coordDelimiter;
                filePos.updatePos(&newCoordinatesFile);
            }
        }
        coordinatesFile.close();
        newCoordinatesFile.close();
    } else {
        std::cout << "Coordinates file already exists.\n";
    }
}

std::vector<std::vector<float>> CoordinatesReader::getInstantCoordinates(int t) {
    std::string line;
    std::ifstream coordinatesFile ("coordinates/reshapedTraces.txt");
    std::vector<std::vector<float>> coordinatesVector;
    std::string x,y,z;
    int satelliteCounter = 0;
    bool lineFound = false;
    coordinatesFile.seekg(7500*t);
    while(std::getline(coordinatesFile,line,';') && !lineFound) {
        x = line.substr(0,line.find(','));
        if(x.find('\n') != std::string::npos) {
            x = x.substr(x.find('\n')+1);
        }
        line.erase(0,line.find(',')+1);
        y = line.substr(0,line.find(','));
        line.erase(0,line.find(',')+1);
        z = line.substr(0,line.find(','));
        line.erase(0,line.find(',')+1);
        std::vector<float> coords = {std::stof(x),std::stof(y),std::stof(z)};
        coordinatesVector.push_back(coords);
        if(++satelliteCounter == 100) {
                lineFound = true;
        }        
    }
    coordinatesFile.close();
    return coordinatesVector;
}

/*
int main(int argc, char* argv[]) {
    CoordinatesReader* reader = new CoordinatesReader();
    auto start = std::chrono::high_resolution_clock::now();
    //getInstantCoordinates(9998);
    for(int t = 0; t <= 9998; t++) {
        reader -> getInstantCoordinates(t);
    }
    delete reader;
    auto stop = std::chrono::high_resolution_clock::now();
    std::cout << "Execution time : " << std::chrono::duration_cast<std::chrono::milliseconds>(stop-start).count() << "\n";
}
*/
