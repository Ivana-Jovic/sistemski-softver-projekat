#ifndef SYMBOL_H
#define SYMBOL_H

# include<iostream>
#include<string.h>
using namespace std;

static int numSymbols=0;
class Symbol{
private:
 public:   
    int section;//string sectionName
    //ako je sekcija 0 onda je undef
    int offset;
   
    int ordinalNum;
 
    ///size i symtype?????

 bool local;
   string name;
    Symbol(string name1, int section1,int offset1,bool local1){
        name=name1;
        section= section1;
        offset=offset1;
        local=local1;
        ordinalNum=++numSymbols;


    }
};



#endif