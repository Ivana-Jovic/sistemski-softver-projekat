#include "asembler.h"
#include <iomanip>
#include<regex>
const char instrSize[]={//u B
    1,//   HALT,
    2,// INT,
    1,// IRET,
    3,// CALL,
    1,// RET,
    3,// JMP,
    3,// JEQ,
    3,// JNE,
    3,// JGT,
    1,// PUSH,?????????????????????????????????
    1,// POP,
    2,// XCHG,
    2,// ADD,
    2,// SUB,
    2,// MUL,
    2,// DIV,
    2,// CMP,
    2,// NOT,
    2,// AND,
    2,// OR,
    2,// XOR,
    2,// TEST,
    2,// SHL,
    2,// SHR,
    3,// LDR,
    3,// STR
};

vector<Symbol> Assembler::symbolTList=  vector<Symbol>();
//   unordered_map<string,Symbol> Assembler::symbolT= unordered_map<string,Symbol>();
  unordered_map<string,Section> Assembler::sectionT= unordered_map<string,Section>();
//   unordered_map<string,Relocation> Assembler::relocationT= unordered_map<string,Relocation>();
// vector<Relocation> Assembler::relocationTList=  vector<Relocation>();

Assembler::Assembler(string inputFileName, string outputFileName){
    
    assemble( inputFileName,  outputFileName);
}


void Assembler::assemble(string inputFileName, string outputFileName){
    this->inputFileName=inputFileName;
    this->outputFileName=outputFileName;



    outputFile.open("outputFilee.txt", fstream::trunc);

    parser= new Parser(this->inputFileName);
    asInput = parser->getListOfLines();
    //izbaci komentare
    int j;
    for(int i=0; i<asInput.size();i++){
        if(( j=asInput[i].find_first_of(";"))!= string::npos){
            if(j!=0) asInput[i]=asInput[i].substr(0,j);
        }
        else if(( j=asInput[i].find_first_of("#"))!= string::npos){
                if(j!=0) asInput[i]=asInput[i].substr(0,j);
        }
    }
//pretvori  heksa u dec 
int m,k;
int q=0, v=0,f=0;
for(int i=0; i<asInput.size();i++){
    if(f==1){q=v;}
    else {q=0;v=0;}
    if(( (j=asInput[i].find("0",q))!= string::npos) &&(( k=asInput[i].find_first_of("x",q))!= string::npos)){
       outputFile<<"nadjen 0 i x"<<endl;
        if(k-j==1){
            outputFile<<"fisrt:"<<endl<<asInput[i]<<"|"<<endl;
            string bef=asInput[i].substr(0,j);
            string from=asInput[i].substr(k);
            // outputFile<<from<<"|"<<endl;
            if(( m=from.find_first_of(" \t\r\n\v\f\\"))!= string::npos){
            //ovde nikad i nece uci
                    string bet=asInput[i].substr(j+2,m);
                    long dec=strtol(bet.c_str(),nullptr,16);
                    string decStr= to_string(dec);
                    string af=asInput[i].substr(m);
                    asInput[i]=bef+decStr+af;
                    outputFile<<asInput[i]<<"|"<<endl;
            }
            else{//0x je na kraju
                string bet=asInput[i].substr(j+2);
                    long dec=strtol(bet.c_str(),nullptr,16);
                    string decStr= to_string(dec);
            
                    asInput[i]=bef+decStr;
                    outputFile<<asInput[i]<<"|"<<endl;
            }
        }
        else{
            //mozda postoji vise 0 ili x
           i=i-1;
           v++;
           f=1;
        }
    }
    else {f=0;}
}
    firstPass();
//     std::vector<string> vals;
// vals.reserve(symbolT.size());

// for(const auto& kv : symbolT) {
//     // vals.push_back(kv.second);  
// } 
//     for(int i=0;i<symbolT.size();i++){
// // symbolT.
//     }
    // cout<<symbolT->second<<endl;
    printSymT();
    printSecT();


    secondPass();
    writeToOutputFile();
    //print sim table
    // secondPass();
//    cout << "{" << "name"<< ": }\t";

}

void Assembler::printSymT(){
    const char separator    = ' ';
    const int nameWidth     = 10;
    // const int numWidth      = 8;

    cout<<endl<<endl<<"SYMBOL TABLE"<<endl;
        cout << setw(nameWidth)<< setfill(separator) << "name "<< setw(nameWidth)<< setfill(separator) << "||\t";
        
        cout << "s"<< setw(nameWidth)<< setfill(separator)  << "||\t";
        cout << "o" << setw(nameWidth)<< setfill(separator) << "||\t";
        cout << "l"<< setw(nameWidth)<< setfill(separator) << "||\t";
        cout << "n" << setw(nameWidth)<< setfill(separator) << "||\n";
        cout<<"------------------------------------------------------------------------------------------------------"<<endl;
        //ako je mapa ide const
        // for (auto const &pair: symbolTList) {
            for(int i=0;i<symbolTList.size();i++){
            // cout << "{" << pair.first << ": }\t";
            cout << setw(nameWidth)<< setfill(separator) << symbolTList[i].name<< setw(nameWidth)<< setfill(separator) << "||\t";
            
            cout << symbolTList[i].section << setw(nameWidth)<< setfill(separator) << "||\t";
            cout << symbolTList[i].offset << setw(nameWidth)<< setfill(separator) << "||\t";
            cout << symbolTList[i].local<< setw(nameWidth) << setfill(separator) << "||\t";
            cout << symbolTList[i].ordinalNum << setw(nameWidth)<< setfill(separator) << "||\n";
        }
        //print u output file:
outputFile<<endl<<endl<<"SYMBOL TABLE"<<endl;
        outputFile << setw(nameWidth)<< setfill(separator) << "name "<< setw(nameWidth)<< setfill(separator) ;
        outputFile << "||"<< setw(nameWidth)<< setfill(separator);
        outputFile << "section"<< setw(nameWidth)<< setfill(separator)  ;
        outputFile << "||"<< setw(nameWidth)<< setfill(separator);
        outputFile << "offset" << setw(nameWidth)<< setfill(separator) ;
        outputFile << "||"<< setw(nameWidth)<< setfill(separator);
        outputFile << "local?"<< setw(nameWidth)<< setfill(separator) ;
        outputFile << "||"<< setw(nameWidth)<< setfill(separator);
        outputFile << "ordNum" << setw(nameWidth)<< setfill(separator) << "||\n";
        outputFile<<"---------------------------------------------------------------------------------------------------"<<endl;
        //ako je mapa ide const
        // for (auto const &pair: symbolTList) {
            for(int i=0;i<symbolTList.size();i++){
            // cout << "{" << pair.first << ": }\t";
            outputFile << setw(nameWidth)<< setfill(separator) << symbolTList[i].name<< setw(nameWidth)<< setfill(separator) ;
            outputFile << "||"<< setw(nameWidth)<< setfill(separator);
            outputFile << symbolTList[i].section << setw(nameWidth)<< setfill(separator) ;
            outputFile << "||"<< setw(nameWidth)<< setfill(separator);
            outputFile << symbolTList[i].offset << setw(nameWidth)<< setfill(separator) ;
            outputFile << "||"<< setw(nameWidth)<< setfill(separator);
            outputFile << symbolTList[i].local<< setw(nameWidth) << setfill(separator);
            outputFile << "||"<< setw(nameWidth)<< setfill(separator);
            outputFile << symbolTList[i].ordinalNum << setw(nameWidth)<< setfill(separator) << "||\n";
        }




}

void Assembler::printSecT(){
    const char separator    = ' ';
    const int nameWidth     = 10;
    // const int numWidth      = 8;
    cout<<endl<<endl<<"SECTION TABLE"<<endl;
        cout << setw(nameWidth)<< setfill(separator) << "name "<< setw(nameWidth)<< setfill(separator);
        cout << "||"<< setw(nameWidth)<< setfill(separator);
        cout << "s"<< setw(nameWidth)<< setfill(separator) ;
        cout << "||"<< setw(nameWidth)<< setfill(separator);
        cout << "n" << setw(nameWidth)<< setfill(separator) << "||\n";
        cout<<"----------------------------------------------------------------------"<<endl;
        for (auto const &pair: sectionT) {
            // cout << "{" << pair.first << ": }\t";
            cout << setw(nameWidth)<< setfill(separator) << pair.second.name << setw(nameWidth)<< setfill(separator) ;
           cout << "||"<< setw(nameWidth)<< setfill(separator);
            cout << pair.second.size << setw(nameWidth)<< setfill(separator) ;
            cout << "||"<< setw(nameWidth)<< setfill(separator);
            cout << pair.second.ordinalNum << setw(nameWidth)<< setfill(separator) << "||\n";
        }
        ///u output file
            outputFile<<endl<<endl<<"SECTION TABLE"<<endl;
        outputFile << setw(nameWidth)<< setfill(separator) << "name "<< setw(nameWidth)<< setfill(separator) ;
        outputFile << "||"<< setw(nameWidth)<< setfill(separator);
        outputFile << "size"<< setw(nameWidth)<< setfill(separator) ;
        outputFile << "||"<< setw(nameWidth)<< setfill(separator);
        outputFile << "ordNum" << setw(nameWidth)<< setfill(separator) << "||\n";
        outputFile<<"-----------------------------------------------------------"<<endl;
        for (auto const &pair: sectionT) {
            // cout << "{" << pair.first << ": }\t";
            outputFile << setw(nameWidth)<< setfill(separator) << pair.second.name << setw(nameWidth)<< setfill(separator) ;
            outputFile << "||"<< setw(nameWidth)<< setfill(separator);
            outputFile << pair.second.size << setw(nameWidth)<< setfill(separator);
            outputFile << "||"<< setw(nameWidth)<< setfill(separator);
            outputFile << pair.second.ordinalNum << setw(nameWidth)<< setfill(separator) << "||\n";
        }
        outputFile<<endl<<endl;
}
Assembler::~Assembler(){
   
    delete parser; 
    parser=nullptr;
    outputFile.close();
}


void Assembler::firstPass(){
    for(int i=0; i<asInput.size();i++){
        
        cout<<endl<<i<<" usao u first pass"<<endl;
        if(endFlag) break;
        asInput[i]=removeSpacesBefore(asInput[i]);
        //ako je prazna linija preskoci je--uradi ovo!!!!!!!!!!!!!!
        if(asInput[i].length()==0){ continue;}
        cout<<"asInpit[i]"<<asInput[i]<<endl;
        if(isLabel(asInput[i],i+1)){cout<<"  label je"<<endl;}
        else if(isDirectiveFirstPass(asInput[i],i+1)){cout<<"  direktiva je"<<endl;}
        else if(isInstructionFirstPass(asInput[i],i+1)){cout<<"  instrukcija je"<<endl;}
        else {
            cout<<"=====sth wrong in first pass=====-LINE:"<<i<<endl;
            exit (2);
        }
    }
    locCntr=0;
    endFlag=false;
    currSecIndex=0;
    //provera- ako global nedef to je greska

    for (int i=0;i<symbolTList.size();i++) {
       if(symbolTList[i].local==false &&symbolTList[i].section==0) {
           cout<<"=======GRESKA-GLOBALAN SIMB OSTAO NEDEFINISAN========sym:"<<symbolTList[i].name<<endl;
           exit(2);
       }
       //ako je extern a definisan je greska
       else  if(symbolTList[i].local==false &&symbolTList[i].section==-1 && symbolTList[i].offset!=0) {
            cout<<"=======GRESKA-EXTERNI SIMB VEC 'DEFINISAN'-fja fisrtPass========sym:"<<symbolTList[i].name<<endl;
             exit(2);
       }
   }
    
}



bool Assembler::isLabel(string line,int fileLineNum){// da li da bude bool
cout<<"u is Label"<<line[0]<<endl;

    if(line[0]=='.'){return false;}//blabla is directive
    int i=0;
    if((i=line.find(":"))!=string::npos){
        // cout<<"ovde1";
        string label= line.substr(0,i);
        // line=removeSpacesBefore(label);///makni bele znakove ispred
        if(!symbolExists(label)){
            Symbol sym(label,currSecIndex,locCntr,true);
            // symbolT.insert({label,sym});
            symbolTList.push_back(sym);
        }
        //postoji i proveri da li je global kako bi samo loc cntr+
        else{
            if(findSymbol(label)->local==false){
                findSymbol(label)->offset=locCntr;
                findSymbol(label)->section=currSecIndex;
            }
            else{//ako postoji lokalan vec
                cout<<"===SYMBOL NAME ALREADY EXISTS-isLab=====line:"<<fileLineNum<<endl;
                exit(2);
            }
        }
        if(line.size()==label.size()) return true;
        line= line.substr(i+1,string::npos);
        if(line.size()==0) return true;
            if (line.empty()) return true;
        line=removeSpacesBefore(line);///makni bele znakove ispred
        if(line.size()==0) return true;
    
        
cout<<"ima jos nesto posle labele"<<line<<"|"<<endl;
    if(isDirectiveFirstPass(line,fileLineNum)){cout<<"-direktiva"<<endl;}
    else {
   isInstructionFirstPass(line,fileLineNum);
   cout<<"-intrukcija"<<endl;
    }
        /////
        return true;
    }
    //else nije labela
    return false;
}

string Assembler::removeSpaces(string s){
    // int i;
    // string ret;
    // while(i=s.find_first_of(" \t")!= string::npos){                                  
    // ret.append(s.substr(0,i));
    // if(i!=(s.length()-1)){
    //     s=s.substr(i+1,string::npos);
    //     }
    // else return ret;
    // }
    // ret.append(s);
    // return ret;

string ret="";
    char ws=' ';
    int j;
    for( j=0;j<s.size();j++){
        if(s[j]!=ws){ ret= ret.append(s[j]+"");

    }
   
    }
 return ret;
}
string Assembler::removeSpacesBefore(string s){
    string ret="";
    char ws=' ';
    int j;
    for( j=0;j<s.size();j++){
        if(s[j]!=ws){ break;}

    }
ret=(s.substr(j,string::npos));
    return ret;


}
// bool Assembler::symbolExists(string name){
//     auto search=symbolT.find(name);;
//     if(search!= symbolT.end()){
//         return true;
//     }
//     else return false;
// }
bool Assembler::symbolExists(string name){
   for (int i=0;i<symbolTList.size();i++) {
       if(symbolTList[i].name==name) return true;
   }
   return false;
}


Symbol* Assembler::findSymbol(string name){
   for (int i=0;i<symbolTList.size();i++) {
       if(symbolTList[i].name==name) {
           return &symbolTList[i];
       }
   }
   cout<<"===POKUSAJ POZIVANJA FIND SYMBOL-a simbol ne postoji-lc:"<<locCntr<<endl;
    exit(2);
   return nullptr;
   
}


bool Assembler::sectionExists(string name){
     auto search=sectionT.find(name);;
    if(search!= sectionT.end()){
        return true;
    }
    else return false;
}


Directives Assembler::directiveToEnum(string d){
    if(d==".global") return GLOBAL;
    if(d==".extern") return EXTERN;
    if(d==".section") return SECTION;
    if(d==".word") return WORD;
    if(d==".skip") return SKIP;
    if(d==".equ") return EQU;
    if(d==".end") return END;
    return NOTFOUND;
}


bool Assembler::isDirectiveFirstPass(string line,int fileLineNum){
   // mozda provera za : labelu itd
   cout<<"u isDirective"<<line[0]<<endl;
   if (line[0]!='.'){return false;}

    int i;
    string directive;
    if((i=line.find_first_of(" "))!= string::npos){
        directive= line.substr(0,i);// ubacuje se sa .
        line= line.substr(i+1,string::npos);
        line=line.substr(line.find_first_not_of(" "));// ukoliko je bio neko razmak
    }
    else{
        cout<<"DIREKTIVA TREBA DA IMA NESTO POSLE"<<endl;
        directive= line;// da li treba da se gleda \r
        //  if (startIndex = directive.find_first_of("\r") != string::npos) {
        //     directive = directive.substr(0, startIndex);
        // }
    }
// int i;
string s;
string secName;
int brSekcije;
string op2;


line= removeSpaces(line);




    switch (directiveToEnum(directive)){
        case GLOBAL:
            //isto kao extern samo je globalflag true
             
        line= removeSpaces(line);// prvo uklonim sve razmake
            while((i= line.find_first_of(",") )!=string::npos){
                s= line.substr(0,i);
                //ako vec postoji promeniti da bude global
                if(symbolExists(s)){findSymbol(s)->local=false;}

                ///OZNAKA DA SU SIMBOLI UNDEF     
                else            addNewSymbol(s,0,locCntr,false);

                line= line.substr(i+1,string::npos);
            }
            s= line;
                        
        //ako vec postoji promeniti da bude global
        if(symbolExists(s)){findSymbol(s)->local=false;}     
        //ELSE?????????????????        
        else         addNewSymbol(s,0,locCntr,false);
        break;
          





        case EXTERN:// predpostavka da niko nece uneti ,, umesto ..,a,....
        // da lii treba global flag!!!!!!!!!!!!!
        ///??????????????????????????????????????????????//
       
        line= removeSpaces(line);// prvo uklonim sve razmake
            while((i= line.find_first_of(",") )!=string::npos){
                s= line.substr(0,i);
                //ako vec postoji promeniti da bude global
                if(symbolExists(s)){
                    cout<<"=======GRESKA-EXTERNI SIMB VEC 'DEFINISAN'-case ext========line:"<<fileLineNum<<endl;
                    
                     exit(2);
                    findSymbol(s)->local=false;
                    }
                //   ///OZNAKA DA SU SIMBOLI UNDEF        
                    else            addNewSymbol(s,-1,0,true);

                                line= line.substr(i+1,string::npos);
            }
            s= line;
                            
            //ako vec postoji promeniti da bude global
            if(symbolExists(s)){
                cout<<"=======GRESKA-EXTERNI SIMB VEC 'DEFINISAN'-case ext========line:"<<fileLineNum<<endl;
                 exit(2);
                findSymbol(s)->local=false;
            }          
            else         addNewSymbol(s,-1,0,false);
            break;





        case SECTION:
            // i= line.find_first_of(" \t\n");
            //proveeri da li postoji ako postoji da li treba dohvatiti loc cntr
            // secName= line.substr(0,string::npos);// pretpostavka da nema nista posle section x
            cout<<"in case section"<<endl;
            secName=line;
///TREBA GLEDATI ZA PRVU SEKCIJU I ZA POSLEDNJU
////////////////////////////////////
/////////////////////////////////////
////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            //u currsec ubaci size(osim ako nije ni jedna sekcija jos postavljena)
            if(currSecIndex>0){ 
                //proveri da li se menja orig a ne kopija
                Section* sec=findSectionUsingIndex(currSecIndex);
                sec->size=locCntr;
            }
           
            //size=0
            locCntr=0;
            if(findSectionUsingName(secName)!=nullptr){ 
                cout<<"===SECTION NAME ALREADY EXISTS-isDir=====line:"<<fileLineNum<<endl;
                exit(2);
            }
            brSekcije=addNewSection(secName);//ako ne postoji ubaciti u ts
            
            //currsec= nova
            if(brSekcije!=-1)currSecIndex= brSekcije;


            break;
        case WORD:
            //lcnt za 2B
           i=0;
            while((i= line.find_first_of(",") )!=string::npos){
                s= line.substr(0,i);
                line= line.substr(i+1,string::npos);
                locCntr+=2;
                //tab simb???
            }
             s= line;
                // line= line.substr(i+1,string::npos);
                locCntr+=2;
            break;
        case SKIP:
            //uveca loccntrza argument
            line=removeSpaces(line);
            cout<<"SKIP"<<atoi(line.c_str())<<";"<<locCntr<<endl;
            locCntr+=atoi(line.c_str());
            cout<<locCntr<<endl;
            break;
        case EQU:
            // u tsim 
            
            
            if((i=line.find_first_of(","))!= string::npos){
                s= line.substr(0,i);
                op2= line.substr(i+1,string::npos);
            }
            //else greska




            // s= line;
            if(!symbolExists(s)){
                //umesto loccntr za ofset je argument a sekcija ce biti -2
                addNewSymbol(s,-2,atoi(op2.c_str()),true);//dodaj u ts
            } else {
                //ok je samo ako postoji i globalan je
                if(findSymbol(s)->local==false){
                    findSymbol(s)->offset=atoi(op2.c_str());
                    findSymbol(s)->section=-2;
                }
                else {
                    cout<<"===SYMBOL NAME ALREADY EXISTS-isDir=====line:"<<fileLineNum<<endl;
                    exit(2);
                }
            } 
            //a vrednost???
            break;
        case END:
//za posl sekciju uzmi velicinu

   if(currSecIndex>0){ 
                //proveri da li se menja orig a ne kopija
                Section* sec=findSectionUsingIndex(currSecIndex);
                sec->size=locCntr;
            }
            endFlag=true;
            return true;

            break;
        case NOTFOUND:
        return false;
            cout<< "directive not found- unknown dir (isDirective())-line:"<<fileLineNum<<endl;
            break;
        default:
            cout<< "directive not found- unknown dir/ default branch (isDirective()-line:"<<fileLineNum<<endl;
            break;
    }
    return true;

}
//  } else if (*it == "INSTRUCTION") {
// 				if (current_section != ".text") {
// 					cout << "Error: instructions allowed only in .text section, at line: " << line << endl << flush;
// 					return false;
// 				}
// 				lc += 2;
// 				section_size += 2;
// 				it++;
// 			//	All addressing modes requires additional 2 bytes except regdir and need to skip helper symbols ,,CONST'' ,,CONDITION'' ,,SYMBOL'' 
// 			} else if (*it != "REGDIR" && *it != "CONST" && *it != "CONDITION" && *it!="SYMBOL" && *it!="PSW") {
// 				lc += 2;
// 				section_size += 2;
// 				it++;
// 			}
void Assembler::addNewSymbol(string name, int section,int offset,bool local){
    if(!symbolExists(name)){
        Symbol sym(name,section,offset,local);
        // symbolT.insert({name,sym});
        symbolTList.push_back(sym);
    } else {cout<<"===already defined symbol- "<<name<<"-lc:"<<locCntr<<endl; 
                exit(2);
    }
}
int Assembler::addNewSection(string name){
    if(!sectionExists(name)){
        Section sec(name);
        sectionT.insert({name,sec});
        return sec.ordinalNum;
    } else { cout<<"===already defined section- "<<name<<"-lc:"<<locCntr<<endl;
        exit(2);
       return -1;}
}
Section* Assembler::findSectionUsingIndex(int index){
    for(auto iter = sectionT.begin();iter!=sectionT.end();++iter){
        if(iter->second.ordinalNum== index) {return &iter->second;}// da li ovo vraca section ili treba paziti za  pokazivace

    }
    return nullptr;
}
Section* Assembler::findSectionUsingName(string name){
    for(auto iter = sectionT.begin();iter!=sectionT.end();++iter){
        if(iter->second.name== name){return &iter->second;}// da li ovo vraca section ili treba paziti za  pokazivace

    }
    cout<<"--SECTION NAME NOT FOUND--"<<"-lc:"<<locCntr<<endl;
    //  exit(2);
    return nullptr;
}


bool Assembler::isInstructionFirstPass(string line,int fileLineNum){

   cout<<"u isInstruction"<<line[0]<<"|"<<endl;
    if (line[0]=='.'){return false;}

    // if(line.empty())return ;

    int i,j;
    string instruction;
    string operands;
    string operand1="", operand2="";
    if((i=line.find_first_of(" "))!= string::npos){
        instruction= line.substr(0,i);
        operands= line.substr(i+1,string::npos);
        cout<<"Operands:"<<operands<<"|"<<endl;
        //operands=operands.substr(line.find_first_not_of(" "));// ukoliko je bio neko razmak
        operands=removeSpaces(operands);
        cout<<"Operands:"<<operands<<"|"<<endl;
        if((j=operands.find_first_of(","))!= string::npos){
        //     cout<<"ovde3";
            operand1=operands.substr(0,j);
            operand2=operands.substr(j+1);
        }
        else{
             operand1=operands;
            // operand2=null ili prazni 
            //takodje da li dole u elsu treba napomenuti to
        }
    }
    else{
        instruction= line;// da li treba da se gleda \r
        //  if (startIndex = directive.find_first_of("\r") != string::npos) {
        //     directive = directive.substr(0, startIndex);
        // }
    }
    //pitati da li je sekcija executable


    //  PREBACENO JE U instrOperandsFormatOk
    bool ret=instrOperandsFormatOk(instruction,operand1,operand2);
    cout<<"instruyction:"<<instruction<<"op1:"<<operand1<<"op2:"<<operand2<<"|"<<endl;
    if (ret==false){
        cout<<"==============INSTRUCTIO FORMAT NOT OK================-line:"<<fileLineNum<<endl;
         exit(2);
    }
    return ret;
    //ovo gore  sada nema mnogo poente
    // if(getInstructionSize(instruction)==-1) return false;
    //      locCntr+=getInstructionSize(instruction);
    //      cout<<"posle instrukcije loccntr: "
    //      <<locCntr<<endl;


     return true;
}




//U PRVOM PROLAZU PORVERI FORMAT
bool Assembler::instrOperandsFormatOk(string instruction,string op1, string op2){
    string ret="";
    ///(r[0-7]|pc|sp)

   //ako je halt itd nema nista posle
   if(instruction=="halt" || instruction=="iret" ||instruction=="ret"){
     if(op1=="" && op2==""){ return true;}
     else return false;
    }
   
    //ako je int,push itd jedan registar ocekije
     if(instruction=="int" || instruction=="push" ||instruction=="pop"){
           regex reg ;
           reg= ("^(r[0-7])$");
            smatch match;
        if(regex_search(op1,match,reg)&& op2==""){ return true;}
        else return false;
    }
    //ako je add... dva reg ocekuje
    if(instruction=="xchg" || instruction=="add" ||instruction=="sub"||instruction=="sub"
       ||instruction=="mul"||instruction=="div"||instruction=="cmp"||instruction=="not"
       ||instruction=="and" ||instruction=="or" ||instruction=="xor"||instruction=="test"
       ||instruction=="shl"||instruction=="shr"){

           regex reg1,reg2 ;
           reg1= ("^(r[0-7])$");
           reg2= ("^(r[0-7])$");
            smatch match;
        if(regex_search(op1,match,reg1)&& regex_search(op2,match,reg2)){ return true;}
        else return false;
    }
    //ako je skok jedan operand plus call adresiranjeJmp
      if(instruction=="jmp" || instruction=="jeq" ||instruction=="jne"||instruction=="jgt"){
       
        if((ret=adresiranjeJmp(op1))!="" && op2==""){ return true;}
        else return false;
    }
    //ako je newadr jedan op ili reg plus oper i call..
    if(instruction=="call" ){
        if((ret=adresiranjeNotJmp(op1))!="" && op2==""){ return true;}
        else return false;
    }
    
    if(instruction=="ldr" || instruction=="str"){
        regex reg1;
        reg1= ("^(r[0-7])$");
        smatch match;
cout<<"ovde0"<<op1<<endl;
//
bool a=regex_search(op1,match,reg1);
bool b=((adresiranjeNotJmp(op2))!="");
cout<<"ovde5"<<a<<"|"<<b<<endl;
        if(regex_search(op1,match,reg1)&&(ret=adresiranjeNotJmp(op2))!=""){ return true;}
        else return false;
    }
    //ako je jfja vratila imm ili reg ind pom ili mem  onda  ima 5B a ne 3B
    cout<<"ovde1"<<endl;
    if(getInstructionSize(instruction)==-1) return false;
    cout<<"ovde2"<<endl;
     locCntr+=getInstructionSize(instruction);
     if(ret=="imL"||ret=="imS"|| ret=="riPomL"||ret=="riPomS"||ret=="meL"||ret=="meS"){
         //u ovim slicajevima je duzina umesto 3, 5
         locCntr+=2;
cout<<"ovde3"<<endl;
     }
    //  cout<<"posle instrukcije loccntr: "
    //  <<locCntr<<endl;
    return true;
}



string Assembler::adresiranjeNotJmp(string operand1){
    //A PSW????????
     regex reg;
     smatch match;
//(r[0-7]|pc|sp)
    //  immedLiteral
    reg=("^\\$[0-9]+$");
    if(regex_search(operand1,match,reg)){  return "imL";   }

    //immedSimbol
    reg=("^\\$[a-zA-Z]+$");
    if(regex_search(operand1,match,reg)){return "imS"; }

    //regDir
    reg= ("^(r[0-7])$");
    if(regex_search(operand1,match,reg)){return "rdi"; }

    //regInd bez pom
    reg = ("^\\[(r[0-7])\\]$");
    if(regex_search(operand1,match,reg)){return "rin"; }

     //regInd sa 16-b oznacenim pomerajem literal
    reg = ("^\\[(r[0-7])\\+[0-9]+\\]$");
    if(regex_search(operand1,match,reg)){ return "riPomL";}

    //regInd sa 16-b oznacenim pomerajem simbol
    reg = ("^\\[(r[0-7])\\+([a-zA-Z]+)\\]$");
    if(regex_search(operand1,match,reg)){ return "riPomS"; }

     //mem lieral
    reg=("^[0-9]+$");
     if(regex_search(operand1,match,reg)){  return "meL"; }

    //mem simbol
    reg=("^[a-zA-Z]+$");
    if(regex_search(operand1,match,reg)){return "meS";  }

    //pc rel simbol 
    reg=("^%[a-zA-Z]+$");
    if(regex_search(operand1,match,reg)){ return "psR"; }

cout<<"===ovakvo adresiranje ne postoji==="<<endl;
return "";
}





string Assembler::adresiranjeJmp(string operand1){
    cout<<"inside adresiranjeJMP";
    //A PSW????????
     regex reg;
     smatch match;
//(r[0-7]|pc|sp)
    //  immedLiteral
    reg=("^[0-9]+$");
    if(regex_search(operand1,match,reg)){  return "imL";   }

    //immedSimbol
    reg=("^[a-zA-Z]+$");
    if(regex_search(operand1,match,reg)){ cout<<"inside adresiranjeJMP- 1"<<endl;return "imS"; }

    //regDir
    reg= ("^\\*(r[0-7])$");
    if(regex_search(operand1,match,reg)){return "rdi"; }

    //regInd bez pom
    reg = ("^\\*\\[(r[0-7])\\]$");
    if(regex_search(operand1,match,reg)){return "rin"; }

     //regInd sa 16-b oznacenim pomerajem literal
    reg = ("^\\*\\[(r[0-7])\\+[0-9]+\\]$");
    if(regex_search(operand1,match,reg)){ return "riPomL";}

    //regInd sa 16-b oznacenim pomerajem simbol
    reg = ("^\\*\\[(r[0-7])\\+([a-zA-Z]+)\\]$");
    if(regex_search(operand1,match,reg)){ return "riPomS"; }

     //mem lieral
    reg=("^\\*[0-9]+$");
     if(regex_search(operand1,match,reg)){  return "meL"; }

    //mem simbol
    reg=("^\\*[a-zA-Z]+$");
    if(regex_search(operand1,match,reg)){return "meS";  }

    //pc rel simbol 
    reg=("^%[a-zA-Z]+$");
    if(regex_search(operand1,match,reg)){ return "psR"; }

cout<<"===ovakvo adresiranje ne postoji==="<<"-lc:"<<locCntr<<endl;
return "";
}


int Assembler::getInstructionSize(string instruction){
    int inSize=-1;
   if(instruction=="halt"){
        inSize= instrSize[Instructions::HALT];
    }
    else if(instruction=="int"){
        inSize= instrSize[Instructions::INT];
    }
     else if(instruction=="iret"){
        inSize= instrSize[Instructions::IRET];
    }
     else if(instruction=="call"){
        inSize= instrSize[Instructions::CALL];
    }
     else if(instruction=="ret"){
        inSize= instrSize[Instructions::RET];
    }
     else if(instruction=="jmp"){
        inSize= instrSize[Instructions::JMP];
    }
     else if(instruction=="jeq"){
        inSize= instrSize[Instructions::JEQ];
    }
     else if(instruction=="jne"){
        inSize= instrSize[Instructions::JNE];
    }
     else if(instruction=="jgt"){
        inSize= instrSize[Instructions::JGT];
    }
     else if(instruction=="push"){
        inSize= instrSize[Instructions::PUSH];
    }
     else if(instruction=="pop"){
        inSize= instrSize[Instructions::POP];
    }
     else if(instruction=="xchg"){
        inSize= instrSize[Instructions::XCHG];
    }
     else if(instruction=="add"){
        inSize= instrSize[Instructions::ADD];
    }
     else if(instruction=="sub"){
        inSize= instrSize[Instructions::SUB];
    }
     else if(instruction=="mul"){
        inSize= instrSize[Instructions::MUL];
    }
  else if(instruction=="div"){
        inSize= instrSize[Instructions::DIV];
    }
     else if(instruction=="cmp"){
        inSize= instrSize[Instructions::CMP];
    }
     else if(instruction=="not"){
        inSize= instrSize[Instructions::NOT];
    }
     else if(instruction=="and"){
        inSize= instrSize[Instructions::AND];
    }
     else if(instruction=="or"){
        inSize= instrSize[Instructions::OR];
    }
     else if(instruction=="xor"){
        inSize= instrSize[Instructions::XOR];
    }
     else if(instruction=="test"){
        inSize= instrSize[Instructions::TEST];
    }
      else if(instruction=="shl"){
        inSize= instrSize[Instructions::SHL];
    }
     else if(instruction=="shr"){
        inSize= instrSize[Instructions::SHR];
    }
     else if(instruction=="ldr"){
        inSize= instrSize[Instructions::LDR];
    }
     else if(instruction=="str"){
        inSize= instrSize[Instructions::STR];
    }
     else {cout<<"====Nema ovakve instrukcije==="<<"-lc:"<<locCntr<<endl;}

return inSize;
}



int Assembler::getInstructionSizeReal(string instruction,string op1, string op2){
    string ret="";
    int size=0;
    //ako je skok jedan operand plus call adresiranjeJmp
      if(instruction=="jmp" || instruction=="jeq" ||instruction=="jne"||instruction=="jgt"){
       
        ret=adresiranjeJmp(op1);
      }
    //ako je newadr jedan op ili reg plus oper i call..
    else if(instruction=="call" ){
        ret=adresiranjeNotJmp(op1);
    }
    
    else if(instruction=="ldr" || instruction=="str"){
       ret=adresiranjeJmp(op2);
    }
    size+=getInstructionSize(instruction);
     if(ret=="imL"||ret=="imS"|| ret=="riPomL"||ret=="riPomS"||ret=="psR"||ret=="meL"||ret=="meS"){
         //u ovim slicajevima je duzina umesto 3, 5
         size+=2;

     }
     return size;
}
//==============================SECOND PASS=============================================================================

void Assembler::secondPass(){
    cout<<endl<<endl<<endl<<"============== SECOND PASS ========================"<<endl;
 for(int i=0; i<asInput.size();i++){
        
        cout<<endl<<i<<" usao u second pass"<<endl;
        if(endFlag) break;
        asInput[i]=removeSpacesBefore(asInput[i]);
        cout<<"asInpit[i]"<<asInput[i]<<endl;
        //ako je prazna linija preskoci je--uradi ovo!!!!!!!!!!!!!!
        if(asInput[i].size()==0) continue;
        
        // if(isLabel(asInput[i])){cout<<"  label je"<<endl;}
        // else 
        if(isDirektiveSecondPass(asInput[i])){cout<<"  direktiva je"<<endl;}
        else if(isInstructionSecondPass(asInput[i])){cout<<"  instrukcija je"<<endl;}
        else {cout<<"===sth wrong in second pass===";}
    }
}



bool Assembler::isDirektiveSecondPass(string line){
    //ako je labela pre pazi
    int i;
    if(( i=line.find_first_of(":"))!= string::npos){
        line= line.substr(i+1);
        line= removeSpacesBefore(line);
    }

   cout<<"u isDirective"<<line[0]<<endl;
   if (line[0]!='.'){return false;}

    // int i;
    string directive;
    if((i=line.find_first_of(" "))!= string::npos){
        directive= line.substr(0,i);// ubacuje se sa .
        line= line.substr(i+1,string::npos);
        line=line.substr(line.find_first_not_of(" "));// ukoliko je bio neko razmak
    }
    else{
        cout<<"DIREKTIVA TREBA DA IMA NESTO POSLE"<<endl;
        directive= line;// da li treba da se gleda \r
        //  if (startIndex = directive.find_first_of("\r") != string::npos) {
        //     directive = directive.substr(0, startIndex);
        // }
    }

    string s;
    string secName;
    int brSekcije;
    string op2;

//za .word
    ElemSec es;
    regex reg;
    smatch match;
    int num;

    line= removeSpaces(line);

    switch (directiveToEnum(directive)){
        case GLOBAL:
        //NISTA SE NE RADI SAD????
            break;
          

        case EXTERN:
        //NISTA SE NE RADI SAD????
            break;


        case SECTION:
            // i= line.find_first_of(" \t\n");
            //proveeri da li postoji ako postoji da li treba dohvatiti loc cntr
            // secName= line.substr(0,string::npos);// pretpostavka da nema nista posle section x
            cout<<"in case section"<<endl;
            secName=line;
            if(currSecIndex>0){ 
                //proveri da li se menja orig a ne kopija
                Section* sec=findSectionUsingIndex(currSecIndex);
                sec->size=locCntr;
            }
           
            locCntr=0;
    
            // brSekcije=addNewSection(secName);//ako ne postoji ubaciti u ts
            // if(brSekcije!=-1)currSecIndex= brSekcije;
            cout<<"SECTION NAME:"<<secName<<endl;
            currSecIndex= findSectionUsingName(secName)->ordinalNum;
cout<<currSecIndex<<endl;
            break;


        case WORD:
        cout<<"u is directive .word"<<endl;
            //lcnt za 2B
           i=0;
            while((i= line.find_first_of(",") )!=string::npos){
                s= line.substr(0,i);
                line= line.substr(i+1,string::npos);
                //////////ispis
                s=line;
                //  Literal
                reg=("^[0-9]+$");
                if(regex_search(s,match,reg)){ num=atoi(s.c_str()); }

                //Simbol
                reg=("^[a-zA-Z]+$");
                if(regex_search(s,match,reg)){num=findSymbol(s)->offset; }

                es=  ElemSec();
                es.offsetFrom=locCntr;
                es.offsetTo=locCntr+ 1;
                es.sizeNizByte=2;

                //ako je word ff onda pise ff 00, tako da prvo pushujemo ff pa 00
                es.nizByte.push_back(num%256);
                es.nizByte.push_back(num/256);
                findSectionUsingIndex(currSecIndex)->listOfSecElem.push_back(es);
            ////////
                locCntr+=2;
            }
             s= line;

             //////////ispis
                //  Literal
                reg=("^[0-9]+$");
                if(regex_search(s,match,reg)){ num=atoi(s.c_str()); }

                //Simbol
                reg=("^[a-zA-Z]+$");
                if(regex_search(s,match,reg)){num=findSymbol(s)->offset; }
 cout<<"u is directive .word 1"<<endl;
                es=  ElemSec();
                es.offsetFrom=locCntr;
                es.offsetTo=locCntr+ 1;
                es.sizeNizByte=2;
 cout<<"u is directive .word 2"<<endl;
                //ako je word ff onda pise ff 00, tako da prvo pushujemo ff pa 00
                es.nizByte.push_back(num%256);
                cout<<"u is directive .word 4"<<endl;
                es.nizByte.push_back(num/256);
                cout<<"u is directive .word 5->"<<currSecIndex<<endl;
                findSectionUsingIndex(currSecIndex)->listOfSecElem.push_back(es);
            ////////

                locCntr+=2;
cout<<"u is directive .word 3"<<endl;
            break;

//////////////////////////////==============================================================
/////////////////===================== .word ff u mem stoji ff 00
////////////////////////////////================================================            
        case SKIP:
            //uveca loccntrza argument
            line=removeSpaces(line);
            // cout<<"SKIP"<<atoi(line.c_str())<<";"<<locCntr<<endl;
            locCntr+=atoi(line.c_str());
            // cout<<locCntr<<endl;
            break;


        case EQU:
   //nista se  ne radi??
            break;


        case END:
            if(currSecIndex>0){ 
                //proveri da li se menja orig a ne kopija
                Section* sec=findSectionUsingIndex(currSecIndex);
                sec->size=locCntr;
            }
            endFlag=true;
            return true;

            break;
        case NOTFOUND:
        return false;
            cout<< "===directive not found- unknown dir (isDirective())==="<<"-line:"<<locCntr<<endl;
             exit(2);
            break;
        default:
            cout<< "==directive not found- unknown dir/ default branch (isDirective())=="<<"-line:"<<locCntr<<endl;
             exit(2);
            break;
    }
    return true;
}


bool Assembler::isInstructionSecondPass(string line){
   
    //ako je labela pre pazi
      //ako je labela pre pazi
      int i;
    if((i=line.find_first_of(":"))!= string::npos){
        line= line.substr(i+1);
        line= removeSpacesBefore(line);
    }

   cout<<"u isInstruction"<<line[0]<<"|"<<endl;
    if (line[0]=='.'){return false;}

    //if(line.empty())return;

    int j;
    string instruction;
    string operands="";
    string operand1="", operand2="";
    if((i=line.find_first_of(" "))!= string::npos){
        instruction= line.substr(0,i);
        operands= line.substr(i+1,string::npos);
        //operands=operands.substr(line.find_first_not_of(" "));// ukoliko je bio neko razmak
        operands=removeSpaces(operands);
        if((j=operands.find_first_of(","))!= string::npos){
            operand1=operands.substr(0,j);
            operand2=operands.substr(j+1);
        }
        else{
             operand1=operands;
        }
    }
    else{
        instruction= line;// da li treba da se gleda \r
    }

    string origInstr=instruction;
    string orogOp1=operand1;
    string orogOp2=operand2;
    //pitati da li je sekcija executable
//============================ostatak koda
// int firstByte, secondByte,thirdByte,forthByte,fifthByte;
//   outputFile<<locCntr<<"-"<<(locCntr+ getInstructionSizeReal(instruction,operand1,operand2)-1)<<"=>";
//         outputFile<<hex<<firstByte<<dec<<endl;
    ElemSec es=  ElemSec();
    int sizeInstr=getInstructionSizeReal(instruction,operand1,operand2);
    es.offsetFrom=locCntr;
    es.offsetTo=locCntr+ sizeInstr-1;
    es.sizeNizByte=sizeInstr;

    if(instruction=="halt"){
        es.nizByte.push_back(0);
         findSectionUsingIndex(currSecIndex)->listOfSecElem.push_back(es);
    }
    else if(instruction=="int"){
        es.nizByte.push_back(16);
        int num= (atoi(operand1.substr(1).c_str())<<4) + 15;
        es.nizByte.push_back(num);
        findSectionUsingIndex(currSecIndex)->listOfSecElem.push_back(es);
    }
    else if(instruction=="iret"){
        es.nizByte.push_back(32);
        findSectionUsingIndex(currSecIndex)->listOfSecElem.push_back(es);
    }
    else if(instruction=="call"){
        es.nizByte.push_back(48);
            int num1=  (15<<4);
            //ako je regstarsko onda dodaj jos br reg na num1
           
            //PCRel-ako je pc upisi 7 jer nigde ne pise da je pc osim sto ima %
             if(adresiranjeNotJmp(operand1)=="psR" ){
                     num1+=7;
             }
            //regDir- samo izvuci broj reg   
            if(adresiranjeNotJmp(operand1)=="rdi" ){
                num1+=(atoi(operand1.substr(1).c_str()));
            }
            //regInd-skloni zagradu i slovo r plus reg imaju samo jedan broj
            //za reg ind sa pomerajima je slicno iako je posle +...
            if(adresiranjeNotJmp(operand1)=="rin"  ||
                adresiranjeNotJmp(operand1)=="riPomL" ||adresiranjeNotJmp(operand1)=="riPomS"){
                num1+=(atoi(operand1.substr(2,1).c_str()));
            }
        es.nizByte.push_back(num1);
            int num2=giveAdrModeValue(1,instruction,operand1);
        es.nizByte.push_back(num2);
        if(es.sizeNizByte==5){//onda ima  jos dva bajta
            es=fiveBytesNotJMP(operand1,es);
            cout<<"------ima 5B-----"<<endl;

        }
        findSectionUsingIndex(currSecIndex)->listOfSecElem.push_back(es);
    }
    else if(instruction=="ret"){
        es.nizByte.push_back(64);
        findSectionUsingIndex(currSecIndex)->listOfSecElem.push_back(es);
    }
    else if(instruction=="xchg"){
        es.nizByte.push_back(96);
        int num= (atoi(operand1.substr(1).c_str())<<4) + (atoi(operand2.substr(1).c_str()));
        es.nizByte.push_back(num);
        findSectionUsingIndex(currSecIndex)->listOfSecElem.push_back(es);
    }
    else if(instruction=="add"||instruction=="sub"||instruction=="mul"||instruction=="div"||instruction=="smp"){
        if(instruction=="add")es.nizByte.push_back(112);
        if(instruction=="sub")es.nizByte.push_back(112+1);
        if(instruction=="mul")es.nizByte.push_back(112+2);
        if(instruction=="div")es.nizByte.push_back(112+3);
        if(instruction=="cmp")es.nizByte.push_back(112+4);

        int num= (atoi(operand1.substr(1).c_str())<<4) + (atoi(operand2.substr(1).c_str()));
        es.nizByte.push_back(num);

        findSectionUsingIndex(currSecIndex)->listOfSecElem.push_back(es);
    }
    else if(instruction=="not"||instruction=="and"||instruction=="or"||instruction=="xor"||instruction=="test"){
        if(instruction=="not")es.nizByte.push_back(128);
        if(instruction=="and")es.nizByte.push_back(128+1);
        if(instruction=="or")es.nizByte.push_back(128+2);
        if(instruction=="xor")es.nizByte.push_back(128+3);
        if(instruction=="test")es.nizByte.push_back(128+4);

        int num= (atoi(operand1.substr(1).c_str())<<4) + (atoi(operand2.substr(1).c_str()));
        es.nizByte.push_back(num);

        findSectionUsingIndex(currSecIndex)->listOfSecElem.push_back(es);
    }
    else if(instruction=="shl"||instruction=="shr"){
        if(instruction=="shl")es.nizByte.push_back(144);
        if(instruction=="shr")es.nizByte.push_back(144+1);

        int num= (atoi(operand1.substr(1).c_str())<<4) + (atoi(operand2.substr(1).c_str()));
        es.nizByte.push_back(num);

        findSectionUsingIndex(currSecIndex)->listOfSecElem.push_back(es);
    }
    else if(instruction=="ldr"||instruction=="str"){
        if(instruction=="ldr")es.nizByte.push_back(160);
        if(instruction=="str")es.nizByte.push_back(176);

        int num1= (atoi(operand1.substr(1).c_str())<<4);
        /////
        //PCRel-ako je pc upisi 7 jer nigde ne pise da je pc osim sto ima %
             if(adresiranjeNotJmp(operand1)=="psR" ){
                     num1+=7;
             }
            //regDir- samo izvuci broj reg   
            if(adresiranjeNotJmp(operand1)=="rdi" ){
                num1+=(atoi(operand1.substr(1).c_str()));
            }
            //regInd-skloni zagradu i slovo r plus reg imaju samo jedan broj
            //za reg ind sa pomerajima je slicno iako je posle +...
            if(adresiranjeNotJmp(operand1)=="rin"  ||
                adresiranjeNotJmp(operand1)=="riPomL" ||adresiranjeNotJmp(operand1)=="riPomS"){
                num1+=(atoi(operand1.substr(2,1).c_str()));
            }
        es.nizByte.push_back(num1);
            int num2=giveAdrModeValue(1,instruction,operand1);
        es.nizByte.push_back(num2);
        if(es.sizeNizByte==5){//onda ima  jos dva bajta
            es=fiveBytesNotJMP(operand1,es);
            cout<<"------ima 5B-----"<<endl;

        }
        ////
        
        findSectionUsingIndex(currSecIndex)->listOfSecElem.push_back(es);
    }
    else if(instruction=="push"||instruction=="pop"){
        if(instruction=="push")es.nizByte.push_back(160);
        if(instruction=="pop")es.nizByte.push_back(176);

            int num1= (atoi(operand1.substr(1).c_str())<<4);
        es.nizByte.push_back(num1);
            int num2;//up<<4 da bi nastao num2
            if(instruction=="push")num2=16;//push sp-2-> up=1b
            if(instruction=="pop")num2=64;//pop sp+2-> up=4b

        es.nizByte.push_back(num2);
       
        findSectionUsingIndex(currSecIndex)->listOfSecElem.push_back(es);
    }
    else if(instruction=="jmp"||instruction=="jeq"||instruction=="jne"||instruction=="jgt"){
       cout<<"INSIDE JMP PART"<<es.sizeNizByte<<"|"<<endl;
        if(instruction=="jmp")es.nizByte.push_back(80);
        if(instruction=="jeq")es.nizByte.push_back(80+1);
        if(instruction=="jne")es.nizByte.push_back(80+2);
        if(instruction=="jgt")es.nizByte.push_back(80+3);

        int num1=  (15<<4);
            //ako je regstarsko onda dodaj jos br reg na num1
           string rez=adresiranjeJmp(operand1);
            //PCRel-ako je pc upisi 7 jer nigde ne pise da je pc osim sto ima %
             if(rez=="psR" ){
                     num1+=7;
             }
             
            //regDir- samo izvuci broj reg- od 2 zbog *   !!
            else if(rez=="rdi" ){
                num1+=(atoi(operand1.substr(2).c_str()));
            }
            //regInd-skloni zagradu i*!! i slovo r plus reg imaju samo jedan broj
            //za reg ind sa pomerajima je slicno iako je posle +...
            else if(rez=="rin"  ||
                rez=="riPomL" ||rez=="riPomS"){
                num1+=(atoi(operand1.substr(3,1).c_str()));
            }
        es.nizByte.push_back(num1);
            int num2=giveAdrModeValue(0,instruction,operand1);//FLAG JE 0!
        es.nizByte.push_back(num2);
        if(es.sizeNizByte==5){//onda ima  jos dva bajta
        cout<<"------ima 5B--1--"<<endl;
            es=fiveBytesJMP(operand1,es);
            cout<<"------ima 5B---2-"<<endl;

        }
        
        findSectionUsingIndex(currSecIndex)->listOfSecElem.push_back(es);
    }






    else{cout<<"======GRE$SKA u sec pass instr-======"<<endl;
     exit(2);}
////////////////////////////////////////////////////////////////////////////POVECAVANJE LOCCNTR
        string ret="";
        string op1=orogOp1;
        string op2=orogOp2;
            //ako je skok jedan operand plus call adresiranjeJmp
            if(instruction=="jmp" || instruction=="jeq" ||instruction=="jne"||instruction=="jgt"){
                cout<<"pred kraj sec pass- za jmp loc cntr"<<endl;
                ret=adresiranjeJmp(op1);
            }
            //ako je newadr jedan op ili reg plus oper i call..
            else if(instruction=="call" ){
                ret=adresiranjeNotJmp(op1);
            }
            else if(instruction=="ldr" || instruction=="str"){
            ret=adresiranjeJmp(op2);
            }
            //ako je jfja vratila imm ili reg ind pom ili mem  onda  ima 5B a ne 3B
            locCntr+=getInstructionSize(instruction);
            if(ret=="imL"||ret=="imS"|| ret=="riPomL"||ret=="riPomS"||ret=="meL"||ret=="meS"){
                //u ovim slicajevima je duzina umesto 3, 5
                locCntr+=2;
            }
        //   =instrOperandsFormatOk(origInstr,orogOp1,orogOp2);

return true;
}




ElemSec Assembler::fiveBytesNotJMP(string operand1,ElemSec es){
    cout<<"------------inside fiveByteNotJMP"<<endl;
    Relocation rel= Relocation();
    //skloniti $ i procitati broj
    if(adresiranjeNotJmp(operand1)=="imL" ){
        // cout<<"# IML";
        es.nizByte.push_back(atoi(operand1.substr(1).c_str())/256);
        es.nizByte.push_back(atoi(operand1.substr(1).c_str())%256);
    }
    //skloniti $ i procitati vrednost simbola
    else if(adresiranjeNotJmp(operand1)=="imS" ){
        // cout<<"# IMS";
        operand1=operand1.substr(1);
        //globalni
        if(!findSymbol(operand1)->local){
            es.nizByte.push_back(0);
            es.nizByte.push_back(0);


            // Relocation rel= Relocation();
            rel.tipRelokacije=1;
            rel.local=false;
            rel.offset=es.offsetFrom+3;
            rel.ordNum= findSymbol(operand1)->ordinalNum;
            findSectionUsingIndex(currSecIndex)->listOfRelocations.push_back(rel);


        }
        else{
                    //nadji u tabeli simbola i uzmi vrednost
            //=================AKO NE NADJEMO========================
            // Relocation rel= Relocation();
            rel.tipRelokacije=1;
            rel.local=true;
            rel.offset=es.offsetFrom+3;
            rel.ordNum= currSecIndex;
            findSectionUsingIndex(currSecIndex)->listOfRelocations.push_back(rel);

            int of=(findSymbol(operand1)->offset);
        
            es.nizByte.push_back(of/256);
            es.nizByte.push_back(of%256);
            }
    }
  
    //regIndPom literal
    else if(adresiranjeNotJmp(operand1)=="riPomL" ){
        //  cout<<"# ripomL";
         int i=operand1.find_first_of("+");
         operand1=operand1.substr(i+1);
         operand1=removeSpacesBefore(operand1);
         operand1=operand1.substr(0,operand1.size()-1);//sklanjanje zagrade
        es.nizByte.push_back(atoi(operand1.c_str())/256);
        es.nizByte.push_back(atoi(operand1.c_str())%256);
    }
    //regIndPom simbol 
    else  if(adresiranjeNotJmp(operand1)=="riPomS" ){
        //  cout<<"# ripomS";
         int i=operand1.find_first_of("+");
         operand1=operand1.substr(i+1);
         operand1=removeSpacesBefore(operand1);
         operand1=operand1.substr(0,operand1.size()-1);//sklanjanje zagrade
           if(!findSymbol(operand1)->local){
            es.nizByte.push_back(0);
            es.nizByte.push_back(0);


            // Relocation rel= Relocation();
            rel.tipRelokacije=1;
            rel.local=false;
            rel.offset=es.offsetFrom+3;
            rel.ordNum= findSymbol(operand1)->ordinalNum;
            findSectionUsingIndex(currSecIndex)->listOfRelocations.push_back(rel);


        }
        else{
                //nadji u tabeli simbola i uzmi vrednost
//=================AKO NE NADJEMO========================

            // Relocation rel= Relocation();
            rel.tipRelokacije=1;
            rel.local=true;
            rel.offset=es.offsetFrom+3;
            rel.ordNum= currSecIndex;
            findSectionUsingIndex(currSecIndex)->listOfRelocations.push_back(rel);


        int of=findSymbol(operand1)->offset;
        es.nizByte.push_back(of/256);
        es.nizByte.push_back(of%256);
        }
    }
    //pcREl
    else if(adresiranjeNotJmp(operand1)=="psR" ){
        // cout<<"# psR";
        operand1=operand1.substr(1);//sklanjamo%
          if(!findSymbol(operand1)->local){
            //////////////////////////

            //   Relocation rel= Relocation();
            rel.tipRelokacije=2;
            rel.local=false;
            rel.offset=es.offsetFrom+3;
            rel.ordNum= findSymbol(operand1)->ordinalNum;
            findSectionUsingIndex(currSecIndex)->listOfRelocations.push_back(rel);
int k= (-2)& 65535;
            es.nizByte.push_back(k/256);
            es.nizByte.push_back(k%256);
        }
        else{
         //nadji u tabeli simbola i uzmi vrednost
//=================AKO NE NADJEMO========================

            //   Relocation rel= Relocation();
            rel.tipRelokacije=2;
            rel.local=true;
            rel.offset=es.offsetFrom+3;
            rel.ordNum= currSecIndex;
            findSectionUsingIndex(currSecIndex)->listOfRelocations.push_back(rel);

        int of=findSymbol(operand1)->offset;

        es.nizByte.push_back((of-2)/256);
        es.nizByte.push_back((of-2)%256);


        }
    }
    //mem lit
    else  if(adresiranjeNotJmp(operand1)=="meL" ){
        //  cout<<"# meL";
        es.nizByte.push_back(atoi(operand1.c_str())/256);
        es.nizByte.push_back(atoi(operand1.c_str())%256);
    }
    //mem simb
    else if(adresiranjeNotJmp(operand1)=="meS" ){
        //  cout<<"# meS";
           if(!findSymbol(operand1)->local){
            es.nizByte.push_back(0);
            es.nizByte.push_back(0);


            //   Relocation rel= Relocation();
            rel.tipRelokacije=1;
            rel.local=false;
            rel.offset=es.offsetFrom+3;
            rel.ordNum= findSymbol(operand1)->ordinalNum;
            findSectionUsingIndex(currSecIndex)->listOfRelocations.push_back(rel);


        }
        else{
            //nadji u tabeli simbola i uzmi vrednost
//=================AKO NE NADJEMO========================  Relocation rel= Relocation();
            // Relocation rel= Relocation();
            rel.tipRelokacije=1;
            rel.local=true;
            rel.offset=es.offsetFrom+3;
            rel.ordNum= currSecIndex;
            findSectionUsingIndex(currSecIndex)->listOfRelocations.push_back(rel);



        int of=findSymbol(operand1)->offset;
        es.nizByte.push_back(of/256);
        es.nizByte.push_back(of%256);
        }
    }
    return es;
}






ElemSec Assembler::fiveBytesJMP(string operand1,ElemSec es){
    //skloniti $ i procitati broj
    string res=adresiranjeJmp(operand1);
    Relocation rel= Relocation();
    if(res=="imL" ){
        // cout<<"# IML";
        es.nizByte.push_back(atoi(operand1.c_str())/256);
        es.nizByte.push_back(atoi(operand1.c_str())%256);
    }
    //skloniti $ i procitati vrednost simbola
    else if(res=="imS" ){
        // cout<<"# IMS";
        // operand1=operand1.substr(1);
        //globalni
        if(!findSymbol(operand1)->local){
            es.nizByte.push_back(0);
            es.nizByte.push_back(0);
// cout<<"# IMS 1";

            // Relocation rel= Relocation();
            rel.tipRelokacije=1;
            rel.local=false;
            rel.offset=es.offsetFrom+3;
            rel.ordNum= findSymbol(operand1)->ordinalNum;
            findSectionUsingIndex(currSecIndex)->listOfRelocations.push_back(rel);


        }
        else{
            // cout<<"# IMS 2";
                    //nadji u tabeli simbola i uzmi vrednost
            //=================AKO NE NADJEMO========================
            // Relocation rel= Relocation();
            rel.tipRelokacije=1;
            rel.local=true;
            rel.offset=es.offsetFrom+3;
            rel.ordNum= currSecIndex;
            findSectionUsingIndex(currSecIndex)->listOfRelocations.push_back(rel);

            int of=(findSymbol(operand1)->offset);
        
            es.nizByte.push_back(of/256);
            es.nizByte.push_back(of%256);
            }
    }
  
    //regIndPom literal
    else if(res=="riPomL" ){
        //  cout<<"# ripomL";
         int i=operand1.find_first_of("+");
         operand1=operand1.substr(i+1);
         operand1=removeSpacesBefore(operand1);
         operand1=operand1.substr(0,operand1.size()-1);//sklanjanje zagrade
        es.nizByte.push_back(atoi(operand1.c_str())/256);
        es.nizByte.push_back(atoi(operand1.c_str())%256);
    }
    //regIndPom simbol 
    else  if(res=="riPomS" ){
        //  cout<<"# ripomS";
         int i=operand1.find_first_of("+");
         operand1=operand1.substr(i+1);
         operand1=removeSpacesBefore(operand1);
         operand1=operand1.substr(0,operand1.size()-1);//sklanjanje zagrade
           if(!findSymbol(operand1)->local){
            es.nizByte.push_back(0);
            es.nizByte.push_back(0);


            // Relocation rel= Relocation();
            rel.tipRelokacije=1;
            rel.local=false;
            rel.offset=es.offsetFrom+3;
            rel.ordNum= findSymbol(operand1)->ordinalNum;
            findSectionUsingIndex(currSecIndex)->listOfRelocations.push_back(rel);


        }
        else{
                //nadji u tabeli simbola i uzmi vrednost
//=================AKO NE NADJEMO========================

            // Relocation rel= Relocation();
            rel.tipRelokacije=1;
            rel.local=true;
            rel.offset=es.offsetFrom+3;
            rel.ordNum= currSecIndex;
            findSectionUsingIndex(currSecIndex)->listOfRelocations.push_back(rel);


        int of=findSymbol(operand1)->offset;
        es.nizByte.push_back(of/256);
        es.nizByte.push_back(of%256);
        }
    }
    //pcREl
    else if(res=="psR" ){
        // cout<<"# psR";
        operand1=operand1.substr(1);//sklanjamo%
          if(!findSymbol(operand1)->local){
            //////////////////////////

            //   Relocation rel= Relocation();
            rel.tipRelokacije=2;
            rel.local=false;
            rel.offset=es.offsetFrom+3;
            rel.ordNum= findSymbol(operand1)->ordinalNum;
            findSectionUsingIndex(currSecIndex)->listOfRelocations.push_back(rel);
int k= (-2)& 65535;
            es.nizByte.push_back(k/256);
            es.nizByte.push_back(k%256);
        }
        else{
         //nadji u tabeli simbola i uzmi vrednost
//=================AKO NE NADJEMO========================

            //   Relocation rel= Relocation();
            rel.tipRelokacije=2;
            rel.local=true;
            rel.offset=es.offsetFrom+3;
            rel.ordNum= currSecIndex;
            findSectionUsingIndex(currSecIndex)->listOfRelocations.push_back(rel);

        int of=findSymbol(operand1)->offset;

        es.nizByte.push_back((of-2)/256);
        es.nizByte.push_back((of-2)%256);


        }
    }
    //mem lit
    else  if(res=="meL" ){
        //  cout<<"# meL";
        es.nizByte.push_back(atoi(operand1.substr(1).c_str())/256);
        es.nizByte.push_back(atoi(operand1.substr(1).c_str())%256);
    }
    //mem simb
    else if(res=="meS" ){
        //  cout<<"# meS";
        operand1=operand1.substr(1);
           if(!findSymbol(operand1)->local){
            es.nizByte.push_back(0);
            es.nizByte.push_back(0);


            //   Relocation rel= Relocation();
            rel.tipRelokacije=1;
            rel.local=false;
            rel.offset=es.offsetFrom+3;
            rel.ordNum= findSymbol(operand1)->ordinalNum;
            findSectionUsingIndex(currSecIndex)->listOfRelocations.push_back(rel);


        }
        else{
            //nadji u tabeli simbola i uzmi vrednost
//=================AKO NE NADJEMO========================  Relocation rel= Relocation();
            // Relocation rel= Relocation();
            rel.tipRelokacije=1;
            rel.local=true;
            rel.offset=es.offsetFrom+3;
            rel.ordNum= currSecIndex;
            findSectionUsingIndex(currSecIndex)->listOfRelocations.push_back(rel);



        int of=findSymbol(operand1)->offset;
        es.nizByte.push_back(of/256);
        es.nizByte.push_back(of%256);
        }
    }
    // cout<<"kraj fiveBytesJMP"<<endl;
    return es;
}





int Assembler::giveAdrModeValue(int flag,string instruction, string operand){
    string adr;
    int val=0;
    //flag 1 nije jmp 0 jeste jmp
    if(flag==1){
        adr=adresiranjeNotJmp(operand);
    }
    else  adr=adresiranjeJmp(operand);

    if(adr=="imL" || adr=="imS"){
        val=0;
    }
    else if(adr=="rdi"){
        val=1;
    }
    else if(adr=="rin" ){
        val=2;
        // vrednost U bita i shiftuj za 4 i dodaj
    }
    else if(adr=="riPomL" || adr=="riPomS"){
        val=3;
            // vrednost U bita i shiftuj za 4 i dodaj
    }
    else if(adr=="meL" || adr=="meS"){
        val=4;
    }
    else if(adr=="psR"){
        val=3;
            // vrednost U bita i shiftuj za 4 i dodaj
    }
    
 return val;


}




void Assembler::writeToOutputFile(){
     const char separator    = ' ';
    const int nameWidth     = 5;
     const int nameWidth1     = 5;
    // const int numWidth      = 8;
        // cout << setw(nameWidth)<< setfill(separator) << "name "<< setw(nameWidth)<< setfill(separator);
        // cout << "||"<< setw(nameWidth)<< setfill(separator);
    outputFile<<"====START===="<<endl;
    for(const auto& kv : sectionT){
        outputFile<<"SECTION:"<<kv.first<<endl;    
        Section* ss=findSectionUsingName(kv.first);
        for(int i=0;i<ss->listOfSecElem.size();i++){

            outputFile<< setw(nameWidth)<< setfill(separator)<<ss->listOfSecElem[i].offsetFrom<< setw(nameWidth1)<< setfill(separator);
            // <<"-"<<ss->listOfSecElem[i].offsetTo<< setw(nameWidth1)<< setfill(separator);
            outputFile<< setw(nameWidth)<< setfill(separator)<<":"<< setw(nameWidth)<< setfill(separator);
            for(int j=0; j < ss->listOfSecElem[i].sizeNizByte; j++){
                outputFile<< setw(nameWidth)<< setfill(separator);
                if(j==3){outputFile<<hex<<ss->listOfSecElem[i].nizByte[4]<<dec<<" ";}
                else if(j==4){outputFile<<hex<<ss->listOfSecElem[i].nizByte[3]<<dec<<" ";}
                else
                outputFile<<hex<<ss->listOfSecElem[i].nizByte[j]<<dec<<" ";
                outputFile<< setw(nameWidth)<< setfill(separator);
            }
        outputFile<<endl;
        }
        outputFile<<endl<<endl;
    }
    const int nameWidth2     = 10;
 outputFile<<endl<<endl<<"====RELOCATIONS===="<<endl<<"*if local=>ordNum is section, else ordNum iz sym*"<<endl<<endl;
    for(const auto& kv : sectionT){
        outputFile<<"SECTION:"<<kv.first<<endl;    
        Section* ss=findSectionUsingName(kv.first);
        outputFile<< setw(nameWidth2)<< setfill(separator)<<"offset"<< setw(nameWidth2)<< setfill(separator);
        outputFile<< setw(nameWidth2)<< setfill(separator)<<"rel type"<< setw(nameWidth2)<< setfill(separator);
        outputFile<< setw(nameWidth2)<< setfill(separator)<<"ordNum"<< setw(nameWidth2)<< setfill(separator);
        outputFile<< setw(nameWidth2)<< setfill(separator)<<"local?"<< setw(nameWidth2)<< setfill(separator)<<endl;
        for(int i=0;i<ss->listOfRelocations.size();i++){

            outputFile<< setw(nameWidth2)<< setfill(separator)<<ss->listOfRelocations[i].offset<< setw(nameWidth2)<< setfill(separator);
            outputFile<< setw(nameWidth2)<< setfill(separator)<<ss->listOfRelocations[i].tipRelokacije<< setw(nameWidth2)<< setfill(separator);
            outputFile<< setw(nameWidth2)<< setfill(separator)<<ss->listOfRelocations[i].ordNum<< setw(nameWidth2)<< setfill(separator);
            outputFile<< setw(nameWidth2)<< setfill(separator)<<ss->listOfRelocations[i].local<< setw(nameWidth2)<< setfill(separator)<<endl;
        }
        outputFile<<endl<<endl;
    }
}
