#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "e_lib.h"
	
void int_isr(int signum);	
unsigned *mailbox;

int main(void)
{
	unsigned k,i,j;
	unsigned *dst, *src, *dst1, *src1, *dst2, *src2;
	unsigned tran;
	e_dma_desc_t dma_desc;
	unsigned neighbour_core;
	unsigned *n_row, *n_col, *p;
	unsigned *mailbox1;
	n_row = (unsigned *)0x00006000;
	n_col = (unsigned *)0x00006004;
	tran = 128;
	p = 0x0000;
	
	// Get the core id of current core and the neighbour core
	e_neighbor_id(E_NEXT_CORE, E_ROW_WRAP, n_row, n_col);

	neighbour_core = (unsigned) e_get_global_address(*n_row, *n_col, p);

	// Define the mailbox
	mailbox = (unsigned *)0x2250;
	mailbox1 = (unsigned *)0x3000;
	mailbox[0] = 0x00000000;
	mailbox1[0] = 0x00000000;

	// Initialize the buffer address for dma chain test
	src = (int *)0x2000;
	dst = (int *)(neighbour_core + (unsigned)0x2000);
	src1 = (int *)0x2300;
	dst1 = (int *)(neighbour_core + (unsigned)0x2500);
	src2 = (int *)0x2600;
	dst2 = (int *)(neighbour_core + (unsigned)0x2a00);

 	// Initialize the source and destination buffer
	for (i=0; i<tran; i++)
	{ 
		src[i] = 0xaaaaaaaa;
		dst[i] = 0x00000000;
		src1[i] = 0xbbbbbbbb;
		dst1[i] = 0x00000000;
		src2[i] = 0xcccccccc;
		dst2[i] = 0x00000000;
	}
	
	// Attach the ISR with this interrupt
	e_irq_attach(E_DMA0_INT, int_isr);

	// Clear the IMASK
	e_irq_mask(E_DMA0_INT, E_FALSE);
	
	// Enable the global interrupt
	e_irq_global_mask(E_FALSE);

	// Prepare for the descriptor for 2d dma 
	
	e_dma_set_desc(E_DMA_0,(E_DMA_ENABLE|E_DMA_MASTER|E_DMA_WORD|E_DMA_IRQEN), 0x0000,
	0x0004, 0x0004,
	0x0081, 0x0003,
	0x0100 , 0x0300,
	(void *)src,(void *)dst, &dma_desc);
	
	// Start transaction
	e_dma_start(&dma_desc, E_DMA_0);
	
	// Going into idle
	__asm__ __volatile__("idle");
	
	// Test if the process goes back to the original place
	mailbox1[0] = 0xdeadbeef;
	
	// Set the IMASK
	e_irq_mask(E_DMA0_INT, E_TRUE);
	
	// Disable the global interrupt
	e_irq_global_mask(E_TRUE);
	
	return 0;
}

void __attribute__((interrupt)) int_isr(int signum)
{
	mailbox[0] = 0xdeadbeef;
	return;
}	

