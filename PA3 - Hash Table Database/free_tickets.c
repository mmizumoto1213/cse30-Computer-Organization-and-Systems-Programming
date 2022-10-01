#include <limits.h>
#include <stdint.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "hashdb.h"

//uncomment the next line when you want to use your routine
//#define MYCODE
#ifndef MYCODE
TODO(USING THE SOLUTION FUNCTION NOT MY CODE)
#else

/*
 * free_tickets
 *      tears down the database freeing all allocated memory
 *
 *      go down each hash chain and free all the vehicles and the tickets
 *      for each vehicle
 * args
 *  hashtab pointer to hashtable (pointer to an array of pointers)
 *  tabsz   number of elements in the hash table
 */
void
free_tickets(struct vehicle **hashtab, uint32_t tabsz)
{
	// Loops through every chain in the hashtable
	long unsigned int cnt = 0;
	for (int i = 0; i < (int)tabsz; i++) {
		// Gets one chain on the hashtable
		struct vehicle *vlist = *(hashtab + i);
		while (vlist != NULL) {
			struct ticket *tlist = vlist->head;
			// Loops through ticket list and frees all tickets
			while (tlist != NULL) {
				struct ticket *ttemp = tlist;
				tlist = tlist->next;
				free(ttemp);
				cnt++;
			}
			struct vehicle *vtemp = vlist;
			vlist = vlist->next;
			free(vtemp->state);
			free(vtemp->plate);
			free(vtemp);

		}
	}
	free(hashtab);
	printf("Total tickets freed: %lu\n", cnt);
}
#endif
