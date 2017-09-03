//
// Created by alfred on 8/29/17.
//

#ifndef KD_TREE_KD_NODE_H
#define KD_TREE_KD_NODE_H

#include <iostream>
#include <vector>

class KD_Node{
private:
    long dim;
    std::string *Punto;
    std::string *typeOfData;
public:
    KD_Node* Sons[2];
    KD_Node(){
        Punto = nullptr;
        typeOfData = nullptr;
        Sons[0] = nullptr;
        Sons[1] = nullptr;
        dim = 0;
    }
    KD_Node(std::vector<std::string> &data, std::string* &TypeOfData){
        dim = (long)data.size();
        Punto = new std::string[dim];
        for (int i = 0; i < dim; ++i) {
            Punto[i] = data[i];
        }
        this->typeOfData = TypeOfData;
        Sons[0] = nullptr;
        Sons[1] = nullptr;
    }
    void printData(){
        for (int i = 0; i < dim; ++i) {
            std::cout << Punto[i] << ' ';
        }
        std::cout << std::endl;
        return;
    }
    std::vector<std::string> getData(){
        std::vector<std::string> ans;
        for (int i = 0; i < dim; ++i) {
            ans.push_back(Punto[i]);
        }
        return ans;
    }
    std::string getData(long pos){
        if(pos < 0 || pos > dim -1 || dim == 0) return "NoData!";
        return Punto[pos];
    }
    std::vector<std::string> getTypeOf() {
        std::vector<std::string> ans;
        for (int i = 0; i < dim; ++i) {
            ans.push_back(typeOfData[i]);
        }
        return ans;
    }
    std::string getTypeOf(long pos){
        if(pos < 0 || pos > dim -1 || dim == 0) return "NoDataType!";
        return typeOfData[pos];
    }

    ~KD_Node(){
        delete[] Punto;
    }
};

#endif //KD_TREE_KD_NODE_H
