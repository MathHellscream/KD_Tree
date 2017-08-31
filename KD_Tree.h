//
// Created by alfred on 8/29/17.
//

#ifndef KD_TREE_KD_TREE_H
#define KD_TREE_KD_TREE_H

#include "KD_Tree.h"
#include "KD_Node.h"
#include "CSVLibrary/CSVHandler.h"
#include <vector>
#include <memory>
#include <thread>

class KD_Tree{
private:
    KD_Node* root;
    long dim;
    std::string *dataTypes;

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
    KD_Tree(){
        root = nullptr;
        dataTypes = nullptr;
    }

    void printDataTypes(){
        if(!dataTypes){
            std::cout << "No data loaded" << std::endl;
            return;
        }
        for (int i = 0; i < dim; ++i) {
            std::cout << dataTypes[i] << ' ';
        }
        std::cout << std::endl;
        return;
    }

    void loadDataFromCSV(std::string pathToCSV){
        CSVHandler handler(pathToCSV);
        std::vector<std::vector<std::string> > dataSet;
        handler.read(dataSet);
        dim = (long)dataSet.at(0).size();
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
            MyThreads[i] = std::thread(&KD_Tree::getDataTypes,i,PosiblyDataTypes,ini,std::min(ini+rowsPerThread-1,(long)dataSet.size()-1),std::ref(dataSet));
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
    ~KD_Tree(){
        if(root) delete root;
        if(dataTypes) delete dataTypes;
    }
};

#endif //KD_TREE_KD_TREE_H
