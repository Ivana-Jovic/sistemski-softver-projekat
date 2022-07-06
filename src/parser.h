#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <string.h>
#include <fstream>
#include<list>
#include<vector>

using namespace std;


class Parser{
private:
    ifstream file;
    
 //   list<string> listOfLines;// gde inicijalizujem
    vector<string> listOfLines;
public:
    Parser(string fileName);
    ~Parser();
    void parseFromFile(ifstream& file);
    vector<string> getListOfLines(){ return listOfLines;}
    //ifstream getFile(){return file;}


};

#endif