#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "e_lib.h"


int main(void)
{
	unsigned time_c, time_p;
	unsigned time;
	unsigned tran,k,i,j,q,h,m,n;
	unsigned *box;
	unsigned *p;
	unsigned *one, *other;
	unsigned *mas_row,*mas_col,*n_row,*n_col;
	unsigned address[e_group_config.group_rows][e_group_config.group_cols];
	
	// Define the mailbox
	box = (unsigned *) 0x5000;
	mas_row = (unsigned *)0x6000;
	mas_col = (unsigned *)0x6004;
	p = 0x2000;
	tran = 2048;

	// Get all the other cores address
	for(i=0; i<e_group_config.group_rows; i++)
	{
		for(j=0; j<e_group_config.group_cols; j++)
		{
			if((i!=*mas_row)|(j!=*mas_col))
			{
				address[i][j] = e_get_global_address(i, j, p) ;
			}else
			{
				address[i][j] = 0x2000;
			}
		}
	}	
	
	
	// Test the writing bandwidth	
	// Initialize the one selected core and all the other cores
	one = (unsigned *) address[*mas_row][*mas_col];
	
	e_neighbor_id(E_NEXT_CORE, E_ROW_WRAP, n_row, n_col);
	other = (unsigned *) e_get_global_address(*n_row, *n_col, p) ;
	
	for(i=0; i<tran; i++)
	{
		one[i] = 0x00000000;
		other[i] = 0xdeadbeef;
	}	
	
	for(i=0; i<e_group_config.group_rows; i++)
	{
		for(j=0; j<e_group_config.group_cols; j++)
		{
			if((i!=*mas_row)|(j!=*mas_col))
			{
				e_dma_copy((unsigned *)address[i][j], other, 0x2000);
			}
		}
	}
	
	// Set the ctimer
	e_ctimer_set(E_CTIMER_0, E_CTIMER_MAX) ;
	
	// Start the ctimer and select the time type
	time_p = e_ctimer_start(E_CTIMER_0, E_CTIMER_CLK);
	
	// All write to the core
	for(i=0; i<e_group_config.group_rows; i++)
	{
		for(j=0; j<e_group_config.group_cols; j++)
		{
			if((i!=*mas_row)|(j!=*mas_col))
			{
				e_dma_copy(one, (unsigned *)address[i][j], 0x2000);
			}	
		}
	}
		
	// Get the time now
	time_c = e_ctimer_get(E_CTIMER_0);	
		
	time = time_p - time_c;
		
	// Output the result
	box[0] = time;	

	// Test the reading bandwidth
	// Initialize the one selected core and all the other cores	
	for(i=0; i<tran; i++)
	{
		one[i] = 0xdeadbeef;
		other[i] = 0x00000000;
	}	
	
	for(i=0; i<e_group_config.group_rows; i++)
	{
		for(j=0; j<e_group_config.group_cols; j++)
		{
			if((i!=*mas_row)|(j!=*mas_col))
			{
				e_dma_copy((unsigned *)address[i][j], other, 0x2000);
			}
		}
	}	
	
	// Set the ctimer
	e_ctimer_set(E_CTIMER_0, E_CTIMER_MAX) ;
	
	// Start the ctimer and select the time type
	time_p = e_ctimer_start(E_CTIMER_0, E_CTIMER_CLK);
	
	// Read from all neighbour cores
	for(i=0; i<e_group_config.group_rows; i++)
	{
		for(j=0; j<e_group_config.group_cols; j++)
		{
			if((i!=*mas_row)|(j!=*mas_col))
			{
				e_dma_copy((unsigned *)address[i][j], one, 0x2000);
			}	
		}
	}
		
		
	// Get the time now
	time_c = e_ctimer_get(E_CTIMER_0);	
		
	time = time_p - time_c;
		
	// Output the result
	box[1] = time;	
	return 0;
}
