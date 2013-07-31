#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <e-hal.h>

#define _BufSize   (128)
#define _BufOffset (0x01000000)



int main(int argc, char *argv[])
{
	unsigned row, col, coreid, i,j,m,n,k;
	unsigned int mask0, mask1;
	e_platform_t platform;
	e_epiphany_t dev;
	e_mem_t emem;
	char emsg[_BufSize];
	unsigned int reg_list[] = {E_REG_CONFIG, /*E_REG_STATUS, E_REG_IRET, E_REG_IMASK, E_REG_IPEND,*/ E_REG_DMA0CONFIG, E_REG_DMA0STRIDE, E_REG_DMA0COUNT,  E_REG_DMA0SRCADDR, E_REG_DMA0DSTADDR,/* E_REG_DMA0AUTODMA0,*/ E_REG_DMA0AUTODMA1, E_REG_DMA0STATUS, E_REG_DMA1CONFIG, E_REG_DMA1STRIDE, E_REG_DMA1COUNT, E_REG_DMA1SRCADDR, E_REG_DMA1DSTADDR, /*E_REG_DMA1AUTODMA0,*/ E_REG_DMA1AUTODMA1, E_REG_DMA1STATUS , E_REG_CTIMER0, E_REG_CTIMER1};
	unsigned int reg_name[3];
	unsigned int *pa;
	unsigned int reg_lat;
	unsigned int list_num;
	list_num = (sizeof(reg_list))/(sizeof(reg_list[0]));
	reg_lat = E_REG_ILAT;
	pa = &reg_name[0];
	
	
	
	srand(1);

	// initialize system, read platform params from
	// default HDF. Then, reset the platform and
	// get the actual system parameters.
	//e_set_host_verbosity(H_D2);
	//e_set_loader_verbosity(L_D1);
	e_init(NULL);
	e_reset_system();
	e_get_platform_info(&platform);

	// Allocate a buffer in shared external memory
	// for message passing from eCore to host.
	e_alloc(&emem, _BufOffset, _BufSize);	
	
    	// Open a workgroup
	e_open(&dev, 0, 0, platform.rows, platform.cols);
	
	// Reset the workgroup
	for (m=0; m<platform.rows; m++)
	{	for(n=0; n<platform.cols;n++)
		{	
			e_reset_core(&dev, m, n);
		}
	}
	
	// Load the device program onto all the eCores
	e_load_group("e-register-test2.srec", &dev, 0, 0, platform.rows, platform.cols, E_TRUE);

	// Select a register
	for(k=0; k<list_num; k++)
	{
		*(pa+2) = reg_list[k];
		
		switch (*(pa+2))
		{
		case E_REG_STATUS     : mask0 = (unsigned)0x0000080f; mask1 = (unsigned)0xfffff7f0; break;
		case E_REG_CONFIG     : mask0 = (unsigned)0xffb00000; mask1 = (unsigned)0x004fffff; break;
		case E_REG_DMA0CONFIG : mask0 = (unsigned)0x0000ff89; mask1 = (unsigned)0xffff0076; break;
		case E_REG_DMA1CONFIG : mask0 = (unsigned)0x0000ff89; mask1 = (unsigned)0xffff0076; break;
		case E_REG_DMA0STATUS : mask0 = (unsigned)0x0000ffff; mask1 = (unsigned)0xffff0000; break;
		case E_REG_DMA1STATUS : mask0 = (unsigned)0x0000ffff; mask1 = (unsigned)0xffff0000; break; 
		default : mask0 = (unsigned)0x00000000; mask1 = (unsigned)0xffffffff; break;
		}
		
		*pa = mask0;
		*(pa+1) = mask1;
		
		// Launch a core

		for (i=0; i<platform.rows; i++)
		{
			for (j=0; j<platform.cols; j++)
			{
				//Draw to a certain core
				row=i;
				col=j;
				coreid = (row + platform.row) * 64 + col + platform.col;
				fprintf(stderr,"%3d: Message from eCore 0x%03x (%2d,%2d) (reg number %d): ",
				(i*platform.cols+j),coreid,row,col,k);
				
				// Write 2 masks and register name to the local address	
				e_write(&dev, i, j, 0x6000, pa, sizeof(reg_name));
				
				usleep(100000);
			
				// Wait for core program execution to finish
				// Read message from shared buffer
			
			
				e_read(&emem, 0, 0, 0x0, emsg, _BufSize);

				// Print the message and close the workgroup.
				fprintf(stderr, "\"%s\"\n", emsg);
			}
		}
	}

	// Close the workgroup
	e_close(&dev);
	
	
	// Test E_ILAT register	
	// Open a workgroup
	e_open(&dev, 0, 0, platform.rows, platform.cols);
	
	// Reset the workgroup
	for (m=0; m<platform.rows; m++)
	{	for(n=0; n<platform.cols;n++)
		{	
			e_reset_core(&dev, m, n);
		}
	}
	
	// Load the device program onto all the eCores
	e_load_group("e-register-test3.srec", &dev, 0, 0,  platform.rows,  platform.cols, E_TRUE);
	
	for (i=0; i< platform.rows; i++)
	{
		for (j=0; j< platform.cols; j++)
		{
			//Draw to a certain core
			row=i;
			col=j;
			coreid = (row + platform.row) * 64 + col + platform.col;
			fprintf(stderr,"%3d: Message from eCore 0x%03x (%2d,%2d) (register E_ILAT): ",
			(i* platform.cols+j),coreid,row,col);
			// Disable all interrupts			
			//STATUS[1]
			e_write(&dev, i, j, 0x600c, &reg_lat, sizeof(reg_lat));
			usleep(10000);
			
			// Wait for core program execution to finish
			// Read message from shared buffer
			
			
			e_read(&emem, 0, 0, 0x0, emsg, _BufSize);

			// Print the message and close the workgroup.
			fprintf(stderr, "%s\n", emsg);
			// Enable all interrupts
			//STATUS[1] = 0;
		}
	}
				
	// Close the workgroup
	e_close(&dev);
	
	// Release the allocated buffer and finalize the
	// e-platform connection.
	e_free(&emem);
	e_finalize();

	return 0;
}

				




	


	






	/*for(k=0;k<list_num;k++)
	{
		reg_name=reg_list[k];	

		for(i=0;i<_SeqLen_row;i++)
		{
			for(j=0;j<_SeqLen_col;j++)
			{
				//Draw to a certain core
				row=i;
				col=j;
				coreid = (row + platform.row) * 64 + col + platform.col;
				fprintf(stderr, "%3d: Message from eCore 0x%03x (%2d,%2d) (reg number %d): ", (i*_SeqLen+j), coreid, row, col, k);
				// Open the single-core workgroup and reset the core, in
				// case a previous process is running. Note that we used
				// core coordinates relative to the workgroup.
				e_open(&dev, row, col, 1, 1);
				e_reset_core(&dev, 0, 0);
				// Load the device program onto the selected eCore
				// and launch after loading.
				e_load("e-register-test2.srec", &dev, 0, 0, e_false);
				e_write(&dev, 0, 0, 0x6000, &reg_name, sizeof(reg_name));
				e_start(&dev, 0, 0);
				usleep(10000);
			
				// Wait for core program execution to finish
				// Read message from shared buffer
			
			
				e_read(&emem, 0, 0, 0x0, emsg, _BufSize);

				// Print the message and close the workgroup.
				fprintf(stderr, "\"%s\"\n", emsg);
				e_close(&dev);
			}
		}
	}
	// Release the allocated buffer and finalize the
	// e-platform connection.
	e_free(&emem);
	e_finalize();

	return 0;
}*/






	
   
