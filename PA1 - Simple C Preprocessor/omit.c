/*
 * Programming assignment 1 
 *
 * remove c comments from a source file and write to stdout
 * comments are replaced with a single space
 * handle single and double quoted sequences
 * comments and quoted sequences can span multiple lines
 *
 * error handling:
 *     prints starting line for unterminated comments or quote sequences
 */

#include <stdio.h>
#include <stdlib.h>
#include "omit.h"

int
main(void)
{
	int c;
	int prevc;
	int line = 1;
	int commentLine;
	int stringLine;
	int charLine;
	enum typestate prev = START;
	enum typestate state = START;
	int grid[][5] = {
		{2, 3, 4, 5, 6},
		{7, 8, 9, 10, 11},
		{12, 13, 14, 15, 16}
	};
	int *p1 = &grid[1][3];
	int *p2 = (grid[0] + 9);
	int *p3 = *((*grid + 2) + 4) + *(grid + 1) + *(grid + 2);
	int *p4 = *(grid + 1) + *(*(grid + 1) + 3) - *(*grid + 2);

	printf("%d", *p1);
	printf("\n");
	printf("%d", *p2);
	printf("\n");
	printf("%d", *p3);
	printf("\n");
	printf("%d", *p4);
	printf("\n");

	// Loops through every character in the file until the end of file.
	while ((c = getchar()) != EOF) {
		switch(state) {
			/* Calls the STARTst function and updates the line
			 * number, previous state, and previous character.
			 * It also saves the line any string or character
			 * literal begins on.
			 */
			case START:
				if (prev == SLASH)
					printf("/%c", prevc);
				prev = state;
				prevc = c;
				state = STARTst(c);
				if (c == '\n')
					line++;
				else if (c == '\"')
					stringLine = line;
				else if (c == '\'')
					charLine = line;
				break;
			/* Calls the STRINGLITERALst function and tracks the
			 * line number, previous state, and previous character.
			 */
			case STRINGLITERAL:
				prev = state;
				prevc = c;
				state = STRINGLITERALst(c);
				if (c == '\n')
					line++;
				break;
			/* Calls the CHARACTERLITERALst function and tracks
			 * the line number, previous state, and previous
			 * character
			 */
			case CHARACTERLITERAL:
				prev = state;
				prevc = c;
				state = CHARACTERLITERALst(c);
				if (c == '\n')
					line++;
				break;
			/* Calls the SLASHst function and tracks the line
			 * number, previous state, and previous character.
			 * It also saves the line a comment starts on.
			 */
			case SLASH:
				prev = state;
				prevc = c;
				state = SLASHst(c);
				commentLine = line;
				if (c == '\n')
					line++;
				break;
			/* Calls the LINECOMMENTst function and tracks the
			 * line number, previous state, and previous
			 * character.
			 */
			case LINECOMMENT:
				prev = state;
				prevc = c;
				state = LINECOMMENTst(c);
				if (c == '\n')
					line++;
				break;
			/* Calls the BLOCKCOMMENTst function and tracks the
			 * line number, previous state, and previous character.
			 */
			case BLOCKCOMMENT:
				prev = state;
				prevc = c;
				state = BLOCKCOMMENTst(c);
				if (c == '\n')
					line++;
				break;
			/* Calls the ASTERISKst function and tracks the line
			 * number, previous state, and previous character.
			 */
			case ASTERISK:
				prev = state;
				prevc = c;
				state = ASTERISKst(c);
				if (c == '\n')
					line++;
				break;
		}
	}
	// This prints / if the file ending with the state SLASH
	if (prev == SLASH)
		printf("/");
	/* These if and else if statements print the error messages if the
	 * file ends in an untermated comment, string literal, or character
	 * literal. It uses the corresponding line variables to track the
	 * line where it started.
	 */
	if (state == BLOCKCOMMENT)
		fprintf(stderr, "Error: line %i: unterminated comment\n",
				commentLine);
	else if (state == STRINGLITERAL)
		fprintf(stderr, "Error: line %i: unterminated quote(\")\n",
				stringLine);
	else if (state == CHARACTERLITERAL)
		fprintf(stderr, "Error: line %i: unterminated quote(\')\n", 
				charLine);
	return EXIT_SUCCESS;
}
