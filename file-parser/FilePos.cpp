#include "FilePos.h"

FilePos::FilePos(int maxline, int stringspace, int filewidth) {
    pos = 0;
    line = 0;
    column = 0;
    maxLine = maxline;
    stringSpace = stringspace;
    fileWidth = filewidth;
}

std::vector<std::string> FilePos::stringSplit(std::string line,char delimiter) {
    std::vector<std::string> coordString;
    while(line.find(delimiter) != std::string::npos) {
        std::size_t pos = line.find(delimiter);
        coordString.push_back(line.substr(0,line.find(delimiter)));
        line.erase(0,pos + 1);
    }
    return coordString;
}


void FilePos::updatePos(std::fstream *file) {
    if(line == maxLine) {
        pos = stringSpace*(++column);
        line = 1;
    } else {
        pos = line*fileWidth+stringSpace*column;
        line++;
    }
    file -> seekp(pos);
}