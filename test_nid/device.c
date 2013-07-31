#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "e_lib.h"



	
int main(void)
{
	unsigned k,i,j;
	e_coreid_t coreid;
	unsigned current_core;
	unsigned neighbour_core;
	unsigned *n_row, *n_col, *p;
	unsigned *mailbox, *mailboxA, *mailboxB, *mailboxC;
	p = 0x0000;
	n_row =(unsigned *)0x00006000;
	n_col = (unsigned *)0x00006004;

	// Get the core id of current core and the neighbour core
	coreid = e_get_coreid();
	e_neighbor_id(E_PREV_CORE, E_ROW_WRAP, n_row, n_col);

	current_core = coreid<<20;
	neighbour_core = (unsigned) e_get_global_address(*n_row, *n_col, p);

	// Define the mailbox
	mailbox = (unsigned *)(current_core + (unsigned)0x2000);
	mailboxA =(unsigned *)(current_core + (unsigned)0x3000);
	mailboxB =(unsigned *)(current_core + (unsigned)0x4000);
	mailboxC =(unsigned *)(current_core + (unsigned)0x5000);
	
	*mailbox = current_core;
	*mailboxA = n_row[0];
	*mailboxB = n_col[0];
	*mailboxC = neighbour_core;
	
	
	return 0;
}


