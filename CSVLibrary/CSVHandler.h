//
// Created by alfred on 8/29/17.
//

#ifndef CSVLIBRARY_CSVHANDLER_H
#define CSVLIBRARY_CSVHANDLER_H

#include <iostream>
#include <vector>
#include <fstream>

class CSVHandler {
private:
    std::string PathToFile;
    std::vector<std::string> processString(std::string StringToProcess);
public:
    CSVHandler(std::string pathToFile = "");
    bool read(std::vector<std::vector<std::string> > &dataSave, std::string pathToFile = "");
    bool create(std::vector<std::vector<std::string> > &dataSet, std::string pathToFile = "");
};

CSVHandler::CSVHandler(std::string pathToFile) {
    PathToFile = pathToFile;
}

bool CSVHandler::read(std::vector<std::vector<std::string> > &dataSave, std::string pathToFile){
    if (pathToFile=="") if (PathToFile=="") {std::cerr << " No PathToFile" << std::endl; return false;}
    if (pathToFile!="") PathToFile = pathToFile;
    std::ifstream CSVfile(PathToFile);
    if(!CSVfile.is_open())return false;
    std::string rowToProcess;
    std::vector<std::string> tmpVec;
    dataSave.empty();
    while(CSVfile >> rowToProcess){
        tmpVec = processString(rowToProcess);
        dataSave.push_back(tmpVec);
    }
    CSVfile.close();
    return true;
}

bool CSVHandler::create(std::vector<std::vector<std::string> > &dataSet, std::string pathToFile){
    if (pathToFile=="") if (PathToFile=="") {std::cerr << " No PathToFile" << std::endl; return false;}
    if (pathToFile!="") PathToFile = pathToFile;
    std::ofstream CSVfile(PathToFile);
    if(!CSVfile.is_open())return false;
    for (int i = 0; i < dataSet.size(); ++i) {
        for (int j = 0; j < dataSet[i].size(); ++j) {
            CSVfile << dataSet[i][j];
            if(j != dataSet[i].size() - 1) CSVfile << ',';
        }
        CSVfile << '\n';
    }
    CSVfile.close();
    return true;
}

std::vector<std::string> CSVHandler::processString(std::string StringToProcess) {
    std::vector<std::string> processed;
    std::string stringToAdd = "";
    for (int i = 0; i < StringToProcess.length(); ++i) {
        if (StringToProcess[i]==',' || i==StringToProcess.length()-1) {
            processed.push_back(stringToAdd);
            stringToAdd="";
        }
        else stringToAdd += StringToProcess[i];
    }
    return processed;
}

#endif //CSVLIBRARY_CSVHANDLER_H
