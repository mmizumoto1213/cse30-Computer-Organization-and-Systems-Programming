#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "misc.h"
#include "hashdb.h"

//uncomment the next line when you want to use your routine
#define MYCODE
#ifndef MYCODE
TODO(USING THE SOLUTION FUNCTION NOT MY CODE)
#else

/*
 * split_input
 * 
 * usage:   input buffer csv format, builds table of pointers to each column
 * assumes: input buffer has at least one char in it (not counting the '\0')
 * returns:  0 if all the columns were found,  -1 otherwise
 */
int
split_input(char *buf, char delim, int cnt, char **table, unsigned long lineno,
            char **argv)
{
	char *pt = buf;
	*table = buf;
	int quotecnt = 0;  /* The amounts of quotes found */
	int quotefield = 0;/* 1 if in a quote 0 if not */
	int fieldcnt = 0;  /* The number of fields in the line */
	int newline = 0;   /* 1 when the newline is found */ 
	int tabcnt = 0;    /* increment for in table */
	char startchar;    /* The first character in the field */
	char lastchar;     /* The last character in the field */

	while (newline == 0 && fieldcnt != cnt) {
		startchar = *pt;
		quotecnt = 0;
		*(table + tabcnt) = pt;
		tabcnt++;
		while (*pt != 0) {
			// For when there is a delim found in buf
			if ((*pt == delim)) {
				if ((quotecnt % 2 == 0)) {
					lastchar = *(pt - 1);
					*pt++ = '\0';
					fieldcnt++;
					break;
				}
			// For when there is a newline found in buf
			} else if (*pt == '\n') {
				lastchar = *(pt - 1);
				*pt = '\0';
				fieldcnt++;
				newline = 1;
				break;
			// For when a quote is found in buf
			} else if (*pt == '\"') {
				quotecnt++;
				if (quotecnt % 2 == 1)
					quotefield = 1;
				else
					quotefield = 0;
			}
			pt++;
		}
		/*
		 * Throws errors using dropmsg when there is incorrect 
		 * quotation format or when there is an incorrect number of
		 * fields in a row.
		 */
		if ((tabcnt == cnt) && (newline == 0)) {
			dropmsg("too many columns", lineno, argv);
			return -1;
		}
		if ((tabcnt < cnt) && (newline == 1)) {
			dropmsg("too few columns", lineno, argv);
			return -1;
		}
		if ((quotefield == 1) && *(pt) != '\"') {
			dropmsg("Quote field not terminated properly",
					lineno, argv);
			return -1;
		}
		if (quotecnt % 2 == 1) {
			dropmsg("Quoted field missing final quote",
					lineno, argv);
			return -1;
		}
		if ((quotecnt > 0) && ((startchar != '\"') ||
			       (lastchar != '\"')))	{
			dropmsg("A \" is not allowed inside unquoted field",
					lineno, argv);
			return -1;
		}
	}
	return 0;
}
#endif
