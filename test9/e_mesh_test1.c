#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "e_lib.h"

int main(void)
{
	unsigned tran,k,k1,i,index;
	unsigned *n_row, *n_col, *p;
	unsigned *commander, *flag;
	unsigned *master, *slave;
	unsigned *mas_row, *mas_col;
	mas_row = (unsigned *)0x6008;
	mas_col = (unsigned *)0x600c;
	master = (unsigned *)0x2000;
	p = (unsigned *)0x2000;
	slave = (unsigned *) e_get_global_address(*mas_row, *mas_col, p);
	p = (unsigned *)0x6300;
	flag = (unsigned *) e_get_global_address(*mas_row, *mas_col, p);
	commander = (unsigned *)0x6100;
	tran = 2048;
	
	// Find the index of the core
	k = e_group_config.core_row *e_group_config.group_cols + e_group_config.core_col;
	k1 = (*mas_row) *e_group_config.group_cols + (*mas_col);
	if(k == (k1 - 4))
	{
		index = 0;
	}else if(k == (k1 + 4))
	{
		index = 3;
	}else if(k == (k1 - 1))
	{
		index = 1;
	}else if(k == (k1 + 1))
	{
		index = 2;
	}
	
	// Initialize the master and slave
	for(i=0; i<tran; i++)
	{
		master[i] = 0xaaaaaaaa;
		slave[i] = 0x00000000;
	}
	while(1)
	{
		commander[0] = 0x00000000;
		while(commander[0] != 0xdeadbeef)
		{};
	
		// Start transfering
		e_dma_copy(slave, master, 0x2000);
	
		while(slave[tran-1] != 0xaaaaaaaa)
		{};
		flag[index] = 1;
	}
	return 0;
}
