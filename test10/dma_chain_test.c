#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <e-hal.h>

#define _BufSize   (1024)
#define _BufOffset (0x01000000)


int main(int argc, char *argv[])
{
	unsigned row, col, coreid, i, j, m, n, k;
	e_platform_t platform;
	e_epiphany_t dev;
	//e_mem_t emem;
	//char emsg[_BufSize];
	unsigned flag  = 0x00000000;
	unsigned flag1 = 0x00000000;
	unsigned flag2 = 0x00000000;
	unsigned flag3 = 0x00000000;
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
	//e_alloc(&emem, _BufOffset, _BufSize);	
	
    	// Open a workgroup
	e_open(&dev, 0, 0, platform.rows, platform.cols);
	
	
	// Load the device program onto core (0,0)
	e_load_group("e_dma_chain_test.srec", &dev, 0, 0, platform.rows, platform.cols, E_FALSE);

	// Launch to each core
	for (i=0; i<platform.rows; i++)
	{	
		for(j=0; j<platform.cols; j++)
		{
			row=i;
			col=j;
			coreid = (row + platform.row) * 64 + col + platform.col;
			fprintf(stderr,"%3d: Message from eCore 0x%03x (%2d,%2d) : \n",(row*platform.cols+col),coreid,row,col);
			// Start device
			e_start(&dev, i, j);
			usleep(500000);
			// Wait for core program execution to finish
			// Read message from shared buffer
				
			e_read(&dev, i, j, 0x2250, &flag, sizeof(flag));
			e_read(&dev, i, j, 0x6100, &flag1, sizeof(flag1));
			e_read(&dev, i, j, 0x6104, &flag2, sizeof(flag2));
			e_read(&dev, i, j, 0x6108, &flag3, sizeof(flag3));

			// Print the message and close the workgroup.
			if(flag == 0xffffffff)
			{
				fprintf(stderr, "PASS for word size!\n");
			}else
			{
				fprintf(stderr, "Fail for word size!\n");
			}
	
			if(flag1 == 0xffffffff)
			{
				fprintf(stderr, "PASS for doubleword size!\n");
			}else
			{
				fprintf(stderr, "Fail for doubleword size!\n");
			}
			
			if(flag2 == 0xffffffff)
			{
				fprintf(stderr, "PASS for halfword size!\n");
			}else
			{
				fprintf(stderr, "Fail for halfword size!\n");
			}
		
			if(flag3 == 0xffffffff)
			{
				fprintf(stderr, "PASS for byte size!\n");
			}else
			{
				fprintf(stderr, "Fail for byte size!\n");
			}
		}
	}

	// Close the workgroup
	e_close(&dev);
	
	// Release the allocated buffer and finalize the
	// e-platform connection.
	//e_free(&emem);
	e_finalize();

	return 0;
}
