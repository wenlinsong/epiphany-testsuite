#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "e_lib.h"
		
unsigned checkbuffer(unsigned *po, unsigned a, unsigned num);

int main(void)
{
	unsigned k,i,j;
	unsigned *dst, *dst1, *dst2, *mst;
	unsigned tran;
	e_dma_desc_t dma_desc;
	tran = 128;

	dst = (unsigned *)0x2000;
	dst1 = (unsigned *)0x2400;
	dst2 = (unsigned *)0x2800;
	mst = (unsigned *)0x4000;

	// Clear the buffer
	for (i=0; i<tran*5; i++)
	{ 
		dst[i]  = 0x00000000;
	}
	
	mst[0] = 0x00000000;
	e_reg_write(E_REG_DMA0AUTODMA0, 0x00000000);

	// Prepare for the descriptor slave dma 
	
	e_dma_set_desc(E_DMA_0,(E_DMA_ENABLE|E_DMA_WORD), 0x0000,
	0x0000, 0x0004,
	0x0400, 0x0001,
	0x0000, 0x0004,
	0x0000,(void *)dst, &dma_desc);
	
	while(*mst == 0x00000000){};
	
	e_dma_start(&dma_desc, E_DMA_0);
	
	
	e_dma_wait(E_DMA_0);
	
	return 0;
}


