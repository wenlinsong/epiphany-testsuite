#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "e_lib.h"
	
int main(void)
{
	unsigned k,i,j;
	e_dma_desc_t dma_desc;
	e_dma_desc_t dma_desc1;
	unsigned *dst, *src;
	unsigned tran;
	unsigned *p;
	unsigned dma_busy;
	unsigned *test_c1;
	unsigned *test_c2;
	tran = 128;
	p = (unsigned *)0x8f000000;
		
	// Initialize the buffer address 
	dst = (int *)0x2000;
	test_c1 = (unsigned *)0x6000;
	test_c2 = (unsigned *)0x6004;
	src = p;
	
	// Under the message mode
	// Initialize the source, destination buffer
	for (k=0; k<tran; k++)
	{ 
		src[k]  = 0x12345678;
		dst[k] = 0x00000000;			
	}
	
	src[tran-1] = 0x87654321;
				
	// Prepare for the descriptor for 2d dma 
	
	e_dma_set_desc(E_DMA_0,(E_DMA_ENABLE|E_DMA_MASTER|E_DMA_WORD|E_DMA_MSGMODE), 0x0000,
	0x0004, 0x0004,
	0x0080, 0x0001,
	0x0000, 0x0000,
	(void *)src,(void *)dst, &dma_desc);
	
	// Start transaction
	e_dma_start(&dma_desc, E_DMA_0);	
	
	do 
	{
		dma_busy = e_reg_read(E_REG_DMA0STATUS) & 0xf;
	}
	while (dma_busy);
	
	test_c1[0] = dst[tran-1];
	
	// In normal mode
	// Initialize the source, destination buffer
	for (k=0; k<tran; k++)
	{ 
		src[k]  = 0x12345678;
		dst[k] = 0x00000000;			
	}
	
	src[tran-1] = 0x87654321;
				
	// Prepare for the descriptor for 2d dma 
	
	e_dma_set_desc(E_DMA_0,(E_DMA_ENABLE|E_DMA_MASTER|E_DMA_WORD), 0x0000,
	0x0004, 0x0004,
	0x0080, 0x0001,
	0x0000, 0x0000,
	(void *)src,(void *)dst, &dma_desc);
	
	// Start transaction
	e_dma_start(&dma_desc, E_DMA_0);	
	
	do 
	{
		dma_busy = e_reg_read(E_REG_DMA0STATUS) & 0xf;
	}
	while (dma_busy);
	
	test_c2[0] = dst[tran-1];			
	
	return 0;
}

