#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "e_lib.h"

unsigned checkbuffer(unsigned *po, unsigned a, unsigned num);		

int main(void)
{
	unsigned k,i,j;
	unsigned *auto0, *p, *mst;
	unsigned tran;	
	unsigned *r_row, *r_col;
	unsigned *dst, *dst1, *dst2;
	e_dma_desc_t dma_desc;
	unsigned *mailbox;
	unsigned index;

	tran = 128;
	
	// Define the mailbox
	mailbox = (unsigned *)0x6100;
	mailbox[0] = 0x00000000;
	
	// Define the auto0 register address
	r_row = (unsigned *)0x6500;
	r_col = (unsigned *)0x6504;
	p = (unsigned *)0xf0514;
	auto0 = (unsigned *) e_get_global_address(*r_row, *r_col, p);
	
	p = (unsigned *)0x2000;
	dst = (unsigned *) e_get_global_address(*r_row, *r_col, p);
	
	p = (unsigned *)0x2400;
	dst1 = (unsigned *) e_get_global_address(*r_row, *r_col, p);
	
	p = (unsigned *)0x2800;
	dst2 = (unsigned *) e_get_global_address(*r_row, *r_col, p);
	
	p = (unsigned *)0x4000;
	mst = (unsigned *) e_get_global_address(*r_row, *r_col, p);
	
	mst[0] = 0xdeadbeef;
	
	// Start writing to the auto register of slave core
	for(j=0; j<3; j++)
	{
		for(i=0; i<tran; i++)
		{
			e_wait(E_REG_CTIMER0 , 100000);
			auto0[0] += 4 ;
			
		}
		e_wait(E_REG_CTIMER0, 100000);
	}
	// Check the destination buffer value
	index = checkbuffer(dst,  (unsigned)0x00000004, tran*3);

	if(index == 0)
	{
		mailbox[0] = 0xffffffff;
	}else
	{
		mailbox[0] = 0x00000000;
	}

	return 0;
}


unsigned checkbuffer(unsigned *po, unsigned a, unsigned num)
{
	unsigned k;
	unsigned flag;
	flag = 0;
	for(k=0; k<num; k++)
	{
		if(po[k] != a)
		{
			flag = 1;
		}
		a = a + 4;
	}	
	return flag;
}
