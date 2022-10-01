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
 * del_ticket
 *      remove ticket (summons) from the database
 *      look up the summons for a vehicle and pay it (remove the ticket) 
 *
 *      Find the vehicle by the state and plate. if not found return -1
 *
 *      convert the summ string to a long long int using
 *      using strtosumid. This long long int is how the summons number
 *      is stored in the database in struct ticket.
 *      After being converted you can use it find a summons id match
 *      in the linked list of tickets attached to the strcut vehicle.      
 *
 *      After you have the vehicle, find the summons on the ticket chain
 *      that matches the converted summons string (summid in the example)
 *      example:
 *        unsigned long long summid
 *        if (strtosumid(summ, &summid, argv) != 0)
 *           return -1;
 *        now search the ticket chain to find that summons
 *        struct ticket *tptr;
 *        if (tptr->summons == summid)
 *          found it
 *           
 *      If found, delete it from the ticket chain. If the vehicle
 *      no longer has any tickets, then remove the vehicle from the hash chain
 *      Otherwise you must update the tot_fine and cnt_tickets fields in
 *      the struct vehicle so the are accurate after removing the summons
 *      Make sure to delete all space that malloc()'d 
 *
 * Inputs:
 *  hashtab pointer to hashtable (pointer to an array of pointers)
 *  tabsz   number of elements in the hash table
 *  fineTab table mapping fine code to description and fine   
 *  plate   plate id string to be found
 *  state   state id string to be found
 *  summ    summon id string to be found
 *  argv    for error printing
 *
 * returns 0 if ok -1 if error
 */

int
del_ticket(struct vehicle **hashtab, uint32_t tabsz, struct fine *fineTab,
    char *plate, char *state, char *summ, char **argv)
{
/* insert your code here */
	if ((plate == NULL) || (state == NULL) || (summ == NULL))
		return -1;
	unsigned long long summid;
	if (strtosumid(summ, &summid, argv) != 0)
		return -1;
	uint32_t key = hash(plate, argv) % tabsz;
	struct vehicle *vlist = *(hashtab + key);
	struct vehicle *vprev = *(hashtab + key);

	// Finds vehicle
	while (vlist != NULL) {
		if ((strcmp(vlist->plate, plate) == 0) &&
				(strcmp(vlist->state, state) == 0))
			break;
		vprev = vlist;
		vlist = vlist->next;
	}
	// Vehicle not found
	if (vlist == NULL)
		return -1;

	// Finds ticket
	if (vlist->head == NULL)
		return -1;
	struct ticket *tlist = vlist->head;
	struct ticket *tprev = vlist->head;
	
	// Loops through ticket list
	while (tlist != NULL) {
		if (summid == (tlist->summons))
			break;
		tprev = tlist;
		tlist = tlist->next;
	}
	// Ticket not found
	if (tlist == NULL)
		return -1;
	// For when it is the vehicle's last ticket
	if (vlist->cnt_ticket == 1) {
		free(tlist);
		if (vlist == *(hashtab + key))
			*(hashtab + key) = vlist->next;
		else
			vprev->next = vlist->next;
		free(vlist->plate);
		free(vlist->state);
		free(vlist);
		return 0;
	// For when the vehicle has more than 1 ticket
	} else {
		if (tlist == vlist->head)
			vlist->head = tlist->next;
		else
			tprev->next = tlist->next;
		vlist->tot_fine = vlist->tot_fine - (fineTab + tlist->code)->fine;
		vlist->cnt_ticket--;
		free(tlist);
		return 0;
	}
}
#endif
