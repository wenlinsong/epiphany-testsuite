#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "e_lib.h"

int main(void)
{
	unsigned time_c, time_p;
	unsigned time;
	unsigned tran,k,i,j,q,h,m,n;
	unsigned *commander;
	unsigned *n_row, *n_col, *p, *nei_row, *nei_col;
	unsigned *neighbour, *neighbour0;
	unsigned *master;
	unsigned *counter;
	
	// Define the mailbox
	master = (unsigned *)0x2000;
	n_row = (unsigned *)0x6000;
	n_col = (unsigned *)0x6004;
	neighbour0 = (unsigned *)0x6008;
	nei_row = (unsigned *) 0x600c;
	nei_col = (unsigned *) 0x6010;
	p =(unsigned *) 0x2000;
	commander = (unsigned *)0x6100;
	counter = (unsigned *)0x6300;
	tran = 2048;
	
	// Get the neighbour global address
	e_neighbor_id(E_PREV_CORE, E_ROW_WRAP, n_row, n_col);
	neighbour = (unsigned *) e_get_global_address(*n_row, *n_col, p) ;
	k = (*n_row)*e_group_config.group_cols + (*n_col);
	
	commander[0] = 0x00000000;
	counter[0] = 0;
	
	// Broadcast to the next core
	p = (unsigned *)0x6100;
	e_neighbor_id(E_NEXT_CORE, E_COL_WRAP, nei_row, nei_col);
	neighbour0 = (unsigned *) e_get_global_address(*nei_row, *nei_col, p) ;
	
	// Initialize master and slave
	for(i=0; i<tran; i++)
	{
		master[i] = 0xdeadbee1;
		neighbour[i] = 0x00000000;
	}
			
	// Waiting for the signal to start transfering
	while(commander[0] != (unsigned) 0xdeadbeef)
	{};
	
	// Broadcast the signal to neighbour
	neighbour0[0] = 0xdeadbeef;
	
	// Write to all neighbour cores
	e_dma_copy(neighbour, master, 0x2000);	
	
	while(1)
	{
		if(neighbour[2047] == 0xdeadbee1)
		{
			counter[0] = 1;
			break;
		}
	}
		
	return 0;
}
