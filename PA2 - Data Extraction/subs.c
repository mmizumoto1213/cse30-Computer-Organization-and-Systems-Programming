#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "subs.h"
#include "misc.h"

/*
 * Instructions for PA2
 *
 * put functions in here for
 * (1) filling out the input table of tokens
 * (2) Helper function for (1) that validates a field
 * (3) Function for writing out the fields specified in
 *     the output table
 *     Plus any helper functions you like
 * Below are descriptions of the two functions called by main()
 * Feel free to change the names of the parameters
 */

 /*
  * put function prototype for any helper function here.
  * since they are only called by functions within this file
  * define them as static functions.
  *
  * static int helper(void);   // example of a private helper prototype
  */

/*
 * split_input
 *
 * usage:   input buffer csv format, builds table of pointers to each column
 * assumes: input buffer has at least one char in it (not counting the '\0')
 * args:
 *  buf     one record from read from standind input stream by getline()
 *  delim   the input field delimiter
 *  cnt     number of fields (in table)
 *  table   point to a table of pointers to the start of each record
 *  lineno  line number of record (for passing to dropmesg)
 *  argv    for drop mesg to print name of program
 * returns: 0 if all the columns were found,
 *          -1 if a record did not have all the columns
 *          -1 if a record had an bad field value
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

/*
 * wr_row
 *
 * usage:   given an array of pointers to columns and an array of columns to
 *          ouput and a count of output columns
 *          output those columns specified in the output array (contents are
 *          index numbers of the input array)
 * args:
 *  in_tab  pointer to a table of pointers to the start of each input field
 *  out_tab pointer to a table of ints containing the array index to print
 *          the order in this array is the order the cols/fields are printed
 *  delim   the output record delimiter to use
 *  lineno  line number of record for dropmsg
 *  argv    program name for dropmesg
 * return:  the number of records dropped during output
 *          0 if the record was printed
 *          1 if the record was dropped (special case 1 col and is empty)
 */
int
wr_row(char **in_tab, int *out_tab, int out_cnt, char delim,
        unsigned long lineno, char **argv)
{
	// Loops through the size of out table
	for (int i = 0; i < out_cnt; i++) {
		int index = *(out_tab + i);
		char **s = in_tab + index;
		// Errors when there is empty one field line
		if ((out_cnt == 1) && ((**s) == '\0')) {
			dropmsg("empty column", lineno, argv);
			return 1;
		}
		printf("%s", *s);
		if (i != (out_cnt - 1))
			printf("%c", delim);
		else
			printf("\n");
	}
	return 0;
}
