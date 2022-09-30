/* 
 * the next two lines are the header guards
 */
// #ifndef _OMIT_H
// #define _OMIT_H
/*
 * header file for omit
 * type definition (enum) for DFA states
 * function prototypes
 */
enum typestate {START, STRINGLITERAL, CHARACTERLITERAL, SLASH, LINECOMMENT, BLOCKCOMMENT, ASTERISK};
enum typestate STARTst(int);
enum typestate STRINGLITERALst(int);
enum typestate CHARACTERLITERALst(int);
enum typestate SLASHst(int);
enum typestate LINECOMMENTst(int);
enum typestate BLOCKCOMMENTst(int);
enum typestate ASTERISKst(int);
#endif
