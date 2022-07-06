#ifndef ASEMBLER_H
#define ASEMBLER_H

# include<iostream>
#include<string.h>

#include<unordered_map>

using namespace std;

#include"parser.h"
#include"symbol.h"
#include"section.h"
#include"relocation.h"

#include "enum.h"

class Assembler{
    
private:
    int locCntr=0;
    int currSecIndex=0;// static
    string inputFileName; 
    string outputFileName;  
    bool endFlag;
ofstream outputFile;

    Parser * parser;

public:
    Assembler(string inputFileName, string outputFileName);
    ~Assembler();

    void assemble(string inputFileName, string outputFileName);
    void firstPass();
    void secondPass();//restartuj end flag


static vector<Symbol> symbolTList;
    // static unordered_map<string,Symbol> symbolT;
    static unordered_map<string,Section> sectionT;
    // static unordered_map<string,Relocation> relocationT;

// static vector<Relocation> relocationTList;


bool isLabel(string line,int lineNum);
string removeSpaces(string s);
string removeSpacesBefore(string s);
vector<string> asInput;
void printSymT();
void printSecT();

bool isDirectiveFirstPass(string line,int lineNum);
bool isInstructionFirstPass(string line,int lineNum);
bool isDirektiveSecondPass(string line);
bool isInstructionSecondPass(string line);


Directives directiveToEnum(string diective);
void addNewSymbol(string name1, int section1,int offset1,bool local1);
int addNewSection(string name1);// vraca broj dodate sekcije ili -1 ako vec postoji
bool symbolExists(string name);
Symbol* findSymbol(string name);
bool sectionExists(string name);
Section* findSectionUsingIndex(int index);
Section* findSectionUsingName(string name);

bool instrOperandsFormatOk(string instruction,string  op1, string op2);
string adresiranjeNotJmp(string op);
string adresiranjeJmp(string op);
int getInstructionSize(string instr);
int getInstructionSizeReal(string instr,string op1,string op2);

void writeToOutputFile();

int giveAdrModeValue(int flag,string instruction, string operand);

ElemSec fiveBytesNotJMP(string operand1,ElemSec elemSec);
ElemSec fiveBytesJMP(string operand1,ElemSec elemSec);
    };

 
    #endif