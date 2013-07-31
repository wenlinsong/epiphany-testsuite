#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "e_lib.h"
#define E_CTIMER_MESH_0 (0xf)
#define E_CTIMER_MESH_1 (0xe)


unsigned *box;
unsigned *neighbour_n;
unsigned *neighbour_s;
unsigned *neighbour_w;
unsigned *neighbour_e;
unsigned *outbuf;
unsigned *master;

unsigned mesh_type[2][13] = {{0x00000200, 0x00000300, 0x00000400, 0x00000500,
                              0x00000600, 0x00000700, 0x00000800, 0x00000900,
                              0x00000a00, 0x00000b00, 0x00000c00, 0x00000d00, 0x00000e00},
                             {0x00000020, 0x00000030, 0x00000040, 0x00000050,
                              0x00000060, 0x00000070, 0x00000080, 0x00000990,
                              0x000000a0, 0x000000b0, 0x000000c0, 0x000000d0, 0x000000e0}};

int main(void)
{
	unsigned tran,k,i,j;
	unsigned mesh_reg;
	unsigned mesh_reg_modify;
	unsigned time_c, time_p;
	unsigned time;
	unsigned *n_row, *n_col, *p, *flag;
	e_dma_desc_t dma_desc[4];

	
	// Define the mailbox
	tran =2048;
	box = (unsigned *) 0x6500;
	p =(unsigned *) 0x2000;
	n_row = (unsigned *)0x6000;
	n_col = (unsigned *)0x6004;

	// Get the address of master and slave
	master = (unsigned *)0x2000;

	e_neighbor_id(E_NEXT_CORE, E_ROW_WRAP, n_row, n_col);
	neighbour_e = (unsigned *) e_get_global_address(*n_row, *n_col, p) ;
	
	e_neighbor_id(E_PREV_CORE, E_ROW_WRAP, n_row, n_col);
	neighbour_w = (unsigned *) e_get_global_address(*n_row, *n_col, p) ;
	
	e_neighbor_id(E_NEXT_CORE, E_COL_WRAP, n_row, n_col);
	neighbour_s = (unsigned *) e_get_global_address(*n_row, *n_col, p) ;
	
	e_neighbor_id(E_PREV_CORE, E_COL_WRAP, n_row, n_col);
	neighbour_n = (unsigned *) e_get_global_address(*n_row, *n_col, p) ;
	
	outbuf = (unsigned *)e_emem_config.base ;
	
	// Test different clock cycles
	for (k=0; k<13; k++)
	{

		// Initialize the master, receiver and outbuf
		for (j=0; j<tran; j++)
		{ 
			master[j] = 0x00000000;
			neighbour_e[j] = 0x00010002;
			neighbour_w[j] = 0x00300040; 
			neighbour_s[j] = 0x05000600; 
			neighbour_n[j] = 0x70008000;
		}
			

		// Select mode of time
		mesh_reg = e_reg_read(E_REG_MESH_CONFIG);
		mesh_reg_modify = mesh_reg & 0xfffff0ff;
		mesh_reg_modify = mesh_reg_modify |mesh_type[0][k]; 
		e_reg_write(E_REG_MESH_CONFIG, mesh_reg_modify);
	
		// Set the ctimer
		e_ctimer_set(E_CTIMER_0, E_CTIMER_MAX) ;
		
		// Set the descriptor
		e_dma_set_desc(E_DMA_0,(E_DMA_ENABLE|E_DMA_MASTER|E_DMA_DWORD), 0x0000,
		0x0008, 0x0008,
		0x0200, 0x0002,
		0x0008 , 0x0008,
		neighbour_e, master, &dma_desc[0]);
		
		e_dma_set_desc(E_DMA_0,(E_DMA_ENABLE|E_DMA_MASTER|E_DMA_DWORD), 0x0000,
		0x0008, 0x0008,
		0x0200, 0x0002,
		0x0008 , 0x0008,
		neighbour_w, master,&dma_desc[1]);
		
		e_dma_set_desc(E_DMA_0,(E_DMA_ENABLE|E_DMA_MASTER|E_DMA_DWORD), 0x0000,
		0x0008, 0x0008,
		0x0200, 0x0002,
		0x0008 , 0x0008,
		 neighbour_s, master,&dma_desc[2]);
		
		e_dma_set_desc(E_DMA_0,(E_DMA_ENABLE|E_DMA_MASTER|E_DMA_DWORD), 0x0000,
		0x0008, 0x0008,
		0x0200, 0x0002,
		0x0008 , 0x0008,
		neighbour_n, master, &dma_desc[3]);
		
		
		// Start the ctimer and select the time type
		time_p = e_ctimer_start(E_CTIMER_0, E_CTIMER_MESH_1);
		
		
		// Memory copy read

/*		for (i=0; i<tran; i++)
		{ 
			master[i]  = neighbour_e[i];
			master[i] = neighbour_w[i];
			master[i] = neighbour_s[i];
			master[i] = neighbour_n[i];
			//outbuf[i] = master[i];
			//outbuf[i] += master[i];
		}
*/

		// Memory copy write
		for (i=0; i<tran; i++)
		{ 
			neighbour_e[i]=master[i];
			neighbour_w[i]=master[i];
			neighbour_s[i]=master[i];
			neighbour_n[i]=master[i];
		}
	
		// Read
/*		e_dma_copy(master, neighbour_e, 0x0200);		
		e_dma_copy(master, neighbour_w, 0x0200);
		e_dma_copy(master, neighbour_s, 0x0200);
		e_dma_copy(master, neighbour_n, 0x0200);
*/
		// Write dma_copy
/*		e_dma_copy(neighbour_e, master, 0x0020);		
		e_dma_copy(neighbour_w, master, 0x0020);
		e_dma_copy(neighbour_s, master, 0x0020);
		e_dma_copy(neighbour_n, master, 0x0020);		
*/
/*		// Write dma
		// Start transaction
		e_dma_start(&dma_desc[0], E_DMA_0);
		for(i=0; i<60000000; i++)
		{};
		e_dma_start(&dma_desc[1], E_DMA_0);
		for(i=0; i<60000000; i++)
		{};
		e_dma_start(&dma_desc[2], E_DMA_1);
		for(i=0; i<60000000; i++)
		{};
		e_dma_start(&dma_desc[3], E_DMA_1);
		for(i=0; i<60000000; i++)
		{};
*/			
/*		// Read dma
		e_dma_start(&dma_desc[0], E_DMA_0);
		for(i=0; i<600000; i++)
		{};
		e_dma_start(&dma_desc[1], E_DMA_0);
		for(i=0; i<600000; i++)
		{};
		e_dma_start(&dma_desc[2], E_DMA_0);
		for(i=0; i<600000; i++)
		{};
		e_dma_start(&dma_desc[3], E_DMA_0);
		for(i=0; i<600000; i++)
		{};
		
*/						
		// Get the time now
		time_c = e_ctimer_get(E_CTIMER_0);	
		
		time = time_p - time_c;
		
		// Output the result
		box[k] = time;//e_reg_read(E_REG_DMA1STATUS);
		e_reg_write(E_REG_MESH_CONFIG, mesh_reg);

	} 
	return 0;
}
