/*
 * subroutines for omit program
 * one subroutine for each state of the DFA
 */
#include <stdio.h>
#include "omit.h"

/*
 * function STARTst(int)
 *
 * Operation: Identifies the start of a potential start to a comment, string 
 * literal, or character literal and returns a typestate corresponding to the
 * identified state. If nothing is found it prints the current character being
 * looked at.
 * params: int c
 * returns: SLASH if the a / was found, STRINGLITERAL if \" was found, 
 * CHARACTERLITERAL if \' was found, START if nothing was found.
 */
enum typestate STARTst(int c)
{
	if (c == '/')
		return SLASH;
	else if(c == '\"') {
		printf("%c", c);
		return STRINGLITERAL;
	} else if(c == '\'') {
		printf("%c", c);
		return CHARACTERLITERAL;
	}
	printf("%c", c);
	return START;
}

/*
 * function STRINGLITERALst(int)
 *
 * Operation: Prints the current character and returns to the START state if
 * another \" is found.
 * params: int c
 * returns: START if another \" is found, STRINGLITERAL otherwise.
 */
enum typestate STRINGLITERALst(int c)
{
	printf("%c", c);
	if (c == '\"')
		return START;
	return STRINGLITERAL;
}

/*
 * function CHARACTERLITERALst(int)
 *
 * Operation: Prints the current character and returns to the START state if
 * another \' is found.
 * param: int c
 * returns: START if another \' is found, CHARACTERLITERAL otherwise.
 */
enum typestate CHARACTERLITERALst(int c)
{
	printf("%c", c);
	if (c == '\'')
		return START;
	return CHARACTERLITERAL;
}

/*
 * function SLASHst(int)
 *
 * Operation: Identifies if the current character is a / or * and enters the
 * LINECOMMENT or BLOCKCOMMENT state depending on the character. If it is
 * neither it enters the START state.
 * param: int c
 * returns: LINECOMMENT if a / is found, BLOCKCOMMENT if a * is found,
 * otherwise START.
 */
enum typestate SLASHst(int c)
{
	if (c == '/') {
		printf(" ");
		return LINECOMMENT;
	} else if (c == '*') {
		printf(" ");
		return BLOCKCOMMENT;
	}
	return START;
}

/*
 * function LINECOMMENTst(int)
 * 
 * Operation: Looks for the next new line and enters the START state and
 * prints a newline when a newline is found. Otherwise it will remain in
 * the LINECOMMENT state.
 * param: int c
 * returns: START if a newline is found, otherwise LINECOMMENT.
 */
enum typestate LINECOMMENTst(int c)
{
	if (c == '\n') {
		printf("%c", c);
		return START;
	}
	return LINECOMMENT;
}

/*
 * function BLOCKCOMMENTst(int)
 *
 * Operation: Looks for the next * and enters the ASTERISK state when it is
 * found. If a newline is found a newline will be printed. If an * is not
 * found it will remain in BLOCKCOMMENT.
 * param: int c
 * returns: ASTERISK if a * is found, otherwise BLOCKCOMMENT.
 */
enum typestate BLOCKCOMMENTst(int c)
{
	if (c == '*')
		return ASTERISK;
	else if (c == '\n')
		printf("%c", c);
	return BLOCKCOMMENT;
}

/*
 * function ASTERISKst(int)
 *
 * Operation: Looks for a / or *, and if a / is found the START state will be
 * returned and if a * is found the ASTERISK state will be returned. If a
 * newline is found it will print a newline. If a / or * is not found it will
 * return BLOCKCOMMENT.
 * param: int c
 * returns: START if a / is found, ASTERISK if a * is found, otherwise
 * BLOCKCOMMENT.
 */
enum typestate ASTERISKst(int c)
{
	if (c == '/')
		return START;
	else if (c == '*')
		return ASTERISK;
	else if (c == '\n')
		printf("%c", c);
	return BLOCKCOMMENT;
}
