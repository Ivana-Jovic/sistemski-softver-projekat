#ifndef SECTION_H
#define SECTION_H

# include<iostream>
#include<string.h>
#include "relocation.h"
using namespace std;

static int numSections=0;
// vector<ElemSec> listOfSecElem= vector<ElemSec>();
class ElemSec{
    public:
    int offsetFrom,offsetTo;
    // int firstByte=0, secondByte=0,thirdByte=0,forthByte=0,fifthByte=0;
    vector<int> nizByte= vector<int>();
    int sizeNizByte;

};

class Section{
    private:
   
    
   

public: 
 string name;
    int ordinalNum;
    int size;//size_t
    Section(string name1){
        name=name1;
        size=0;
        ordinalNum=++numSections;
    }
     vector<ElemSec> listOfSecElem= vector<ElemSec>();
     vector<Relocation> listOfRelocations= vector<Relocation>();
};


#endif