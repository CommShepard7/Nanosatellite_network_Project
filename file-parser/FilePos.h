#include <iostream>
#include <fstream>
#include <vector>

#ifndef FILEPOS_H
#define FILEPOS_H

struct FilePos {

    int pos;
    int line;
    int column;
    int maxLine;
    int maxColumn;
    int stringSpace;
    int fileWidth;

    FilePos(int maxline, int stringspace,int filewidth);
    static std::vector<std::string> stringSplit(std::string line,char delimiter);
    void updatePos(std::fstream *file);
};

#endif