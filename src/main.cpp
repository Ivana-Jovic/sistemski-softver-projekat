# include<iostream>
#include<string.h>
using namespace std;


#include"asembler.h"


int main(int argc, char* argv[]){

    // if (argc < 2) {
    //         std::cout<< "ERROR: insufficient number of parameters.\n" << std::endl;
    //         return -1;
    //     }

    // if (argc > 2) {
    //     std::cout << "ERROR: too many parameters.\n" << std::endl;
    //     return -1;
    // }

    string inputFileName; //= argv[1];
    string outputFileName; 


     if (argc == 4 && strcmp(argv[1],"-o")==0 ){
        
        outputFileName = argv[2];
        inputFileName = argv[3];
     }
    else {
        cout << "error: arguments\n";
        return 0;
    }
     
     Assembler * asembler= new Assembler(inputFileName, outputFileName);



     delete asembler;

    return 0;
}