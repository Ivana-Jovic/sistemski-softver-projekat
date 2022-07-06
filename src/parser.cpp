#include "parser.h"


Parser::Parser(string fileName){
    
    ifstream file (fileName);// in is always set for ifstream objects (even if explicitly not set in argument mode).
    //inicijalizacija listofLines???
    // file.seekg(0);
    ////listOfLines= new list<>; na netu ne rade ovo
    parseFromFile(file);

}
Parser::~Parser(){
    

}

void Parser::parseFromFile(ifstream& file){
    if(file.is_open()){
        while(!file.eof()){
            string newLine;	
            getline(file, newLine);
            listOfLines.push_back(newLine);
        }
        //lines = new Array(&l_lines); zasto pravi kopiju

    }else{
        cout << "Nije otvoren ulazni stream" << endl;
        // int a;
        // cin >> a;
        exit(1);
    }
    file.close();


}