#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "e_lib.h"

#define E_CTIMER_MESH_0 (0xf)
#define E_CTIMER_MESH_1 (0xe)

unsigned mesh_type[2][13] = {{0x00000200, 0x00000300, 0x00000400, 0x00000500,
                              0x00000600, 0x00000700, 0x00000800, 0x00000900,
                              0x00000a00, 0x00000b00, 0x00000c00, 0x00000d00, 0x00000e00},
                             {0x00000020, 0x00000030, 0x00000040, 0x00000050,
                              0x00000060, 0x00000070, 0x00000080, 0x00000990,
                              0x000000a0, 0x000000b0, 0x000000c0, 0x000000d0, 0x000000e0}};


int main(void)
{
	unsigned mesh_reg;
	unsigned mesh_reg_modify;
	unsigned time_c, time_p;
	unsigned time;
	unsigned tran,k,i,j,q,h,m,n;
	unsigned *mailbox;
	unsigned *commander;
	unsigned *counter;
	unsigned *master, *slave, *p;
	unsigned *row, *col;
	unsigned *n_row, *n_col;
	unsigned *neighbour0, *neighbour1, *neighbour2, *neighbour3;
	
	master = (unsigned *)0x2000;
	row = (unsigned *)0x6000;
	col = (unsigned *)0x6004;
	p =(unsigned *) 0x2000;
	slave = (unsigned *) e_get_global_address(*row, *col, p);
	commander = (unsigned *)0x6100;
	p = (unsigned *) 0x6300;
	counter = (unsigned *) e_get_global_address(*row, *col, p);
	n_row = (unsigned *)0x5000;
	n_col = (unsigned *)0x5100;
	mailbox = (unsigned *)0x7000;
	tran = 2048;
	
	// Clear the start commander 
	commander[0] = 0x00000000;
	
	// Core number 
	k = (e_group_config.core_row)*e_group_config.group_cols + (e_group_config.core_col);
	
	// Broadcast to all the other neighbours
	p = (unsigned *)0x6100;
	e_neighbor_id(E_PREV_CORE, E_ROW_WRAP, n_row, n_col);
	neighbour0 = (unsigned *) e_get_global_address(*n_row, *n_col, p) ;
	
	e_neighbor_id(E_NEXT_CORE, E_ROW_WRAP, n_row, n_col);
	neighbour1 = (unsigned *) e_get_global_address(*n_row, *n_col, p) ;
	
	e_neighbor_id(E_PREV_CORE, E_COL_WRAP, n_row, n_col);
	neighbour2 = (unsigned *) e_get_global_address(*n_row, *n_col, p) ;
	
	e_neighbor_id(E_NEXT_CORE, E_COL_WRAP, n_row, n_col);
	neighbour3 = (unsigned *) e_get_global_address(*n_row, *n_col, p) ;
	
	// Initialize master and slave
	for(i=0; i<tran; i++)
	{
		master[i] = 0xdeadbee0;
		slave[i] = 0x00000000;
	}

	// Select mode of time
	q = 3;
	mesh_reg = e_reg_read(E_REG_MESH_CONFIG);
	mesh_reg_modify = mesh_reg & 0xffffff0f;
	mesh_reg_modify = mesh_reg_modify |mesh_type[1][q]; 
	e_reg_write(E_REG_MESH_CONFIG, mesh_reg_modify);
	
	// Set the ctimer
	e_ctimer_set(E_CTIMER_0, E_CTIMER_MAX) ;
	
	// Waiting for the signal to start transfering
	while(commander[0] != 0xdeadbeef)
	{};
	
	// Broadcast to all the other neighbours
	neighbour0[0] = 0xdeadbeef;
	neighbour1[0] = 0xdeadbeef;
	neighbour2[0] = 0xdeadbeef;
	neighbour3[0] = 0xdeadbeef;
	
	// Start the ctimer and select the time type
	time_p = e_ctimer_start(E_CTIMER_0, E_CTIMER_MESH_0);
	
	// Write to the corresponding core
	//for(i=0; i<tran; i++)
	//{
	//	slave[i] = master[i];
	//}
	e_dma_copy(slave, master, 0x2000);	
	//e_dma_start(&dma_desc, E_DMA_0);
	
	while(1)
	{
		if(slave[2047] == 0xdeadbee0)
		{
			counter[k] = 1;
			break;
		}
	}
	
	time_c = e_ctimer_get(E_CTIMER_0);
	
	time = time_p - time_c;		
		
	// Output the result
	mailbox[0] = time;
	
	//counter[k] = 1;	
	return 0;
}
