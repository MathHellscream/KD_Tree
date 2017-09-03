//
// Created by alfred on 8/29/17.
//

#ifndef CSVLIBRARY_CSVHANDLER_H
#define CSVLIBRARY_CSVHANDLER_H

#include <iostream>
#include <vector>
#include <fstream>
#include "thread"

class CSVHandler {
private:
    std::string PathToFile;
    std::vector<std::string> processString(std::string StringToProcess);
    static bool isDigit(char &a){
        return a >= 48 && a <= 57;
    }

    static bool getTypeOfOne(std::string &StringToAnalyze){
        int numpoints = 0;
        int itr = 0;
        for(auto &i:StringToAnalyze){
            if (!isDigit(i) && i!='.' && i!='-') return false;
            if(i=='-' && itr!=0)return false;
            if(i=='.' && numpoints==1) return false;
            else if (i =='.' && numpoints==0) ++numpoints;
            ++itr;
        }
        return true;
    }

    static void getDataTypes(long pos, std::string **ans, long ini, long fin, std::vector<std::vector<std::string> > &dataSet){
        long rowWidth = dataSet[0].size();
        for (long i = ini; i <= fin; ++i) {
            for (int j = 0; j < rowWidth; ++j) {
                if (!getTypeOfOne(dataSet[i][j]) && ans[pos][j]!="string") ans[pos][j] = "string";
            }
        }
    }
public:
    CSVHandler(std::string pathToFile = "");
    bool read(std::vector<std::vector<std::string> > &dataSave,  std::string *&dataType,std::string pathToFile = "");
    bool create(std::vector<std::vector<std::string> > &dataSet, std::string pathToFile = "");
    void getDataType(std::vector<std::vector<std::string> > &dataSet, std::string *&dataType);
};

CSVHandler::CSVHandler(std::string pathToFile) {
    PathToFile = pathToFile;
}

bool CSVHandler::read(std::vector<std::vector<std::string> > &dataSave,  std::string *&dataType,std::string pathToFile){
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
    getDataType(dataSave,dataType);
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

void CSVHandler::getDataType(std::vector<std::vector<std::string> > &dataSet, std::string *&dataTypes) {
    int dim = (long)dataSet.at(0).size();
    dataTypes = new std::string[dim];
    for (int i = 0; i < dim; ++i) {
        dataTypes[i] = "number";
    }
    //Get Type of Data of each column using multithreading
    int numThreads = std::min(std::thread::hardware_concurrency(),(unsigned int)dataSet.size());
    long rowsPerThread = (long)dataSet.size() / (long)numThreads;
    std::thread MyThreads[numThreads];
    std::string **PosiblyDataTypes = new std::string* [numThreads];
    for (int i = 0; i < numThreads; ++i) {
        PosiblyDataTypes[i] = new std::string[dim];
        for (int j = 0; j < dim; ++j) {
            PosiblyDataTypes[i][j] = "number";
        }
    }
    int ini = 1;
    for (int i = 0; i < numThreads; ++i) {
        MyThreads[i] = std::thread(&CSVHandler::getDataTypes,i,PosiblyDataTypes,ini,std::min(ini+rowsPerThread-1,(long)dataSet.size()-1),std::ref(dataSet));
        ini += rowsPerThread;
    }
    for (int i = 0; i < numThreads; ++i) {
        MyThreads[i].join();
    }
    for (int i = 0; i < numThreads; ++i) {
        for (int j = 0; j < dim; ++j) {
            if (PosiblyDataTypes[i][j] == "string" && dataTypes[j]!="string") dataTypes[j] = "string";
        }
    }
    for (int i = 0; i < numThreads; ++i) {
        delete[] PosiblyDataTypes[i];
    }
    delete[] PosiblyDataTypes;
    return;
}

#endif //CSVLIBRARY_CSVHANDLER_H
