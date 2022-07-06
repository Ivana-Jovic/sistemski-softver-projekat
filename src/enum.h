#ifndef ENUM_h
#define ENUM_H


enum Directives{
    GLOBAL,
    EXTERN,
    SECTION,
    WORD,
    SKIP,
    EQU,
    END,
    NOTFOUND
  
} ;

//  TEXT,
//     DATA,
//     BSS,
//     RODATA,
    
//     EQU
enum Instructions{
    HALT,
    INT,
    IRET,
    CALL,
    RET,
    JMP,
    JEQ,
    JNE,
    JGT,
    PUSH,
    POP,
    XCHG,
    ADD,
    SUB,
    MUL,
    DIV,
    CMP,
    NOT,
    AND,
    OR,
    XOR,
    TEST,
    SHL,
    SHR,
    LDR,
    STR

};
// const char operandNum[]={
//     0,//   HALT,
//     1,// INT,
//     0,// IRET,
//     1,// CALL,
//     0,// RET,
//     1,// JMP,
//     1,// JEQ,
//     1,// JNE,
//     1,// JGT,
//     1,// PUSH,
//     1,// POP,
//     2,// XCHG,
//     2,// ADD,
//     2,// SUB,
//     2,// MUL,
//     2,// DIV,
//     2,// CMP,
//     2,// NOT,
//     2,// AND,
//     2,// OR,
//     2,// XOR,
//     2,// TEST,
//     2,// SHL,
//     2,// SHR,
//     2,// LDR,
//     2,// STR
// };

#endif