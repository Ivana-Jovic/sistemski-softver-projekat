#ifndef RELOCATION_H
#define RELOCATION_H

# include<iostream>
#include<string.h>
using namespace std;


class Relocation{
    public:
    // int section;//int ili string
    int offset;
    int tipRelokacije;//1 obicna 2 pc rel
    int ordNum;// ili string

bool local;//if local u relokaciji je rb sekcije
//ako je glob tu je rb simbola


    int adend;//???



    
};


#endif