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
	unsigned flag;
	unsigned flag1;
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
	e_load_group("e_dma_int_test.srec", &dev, 0, 0, platform.rows, platform.cols, E_FALSE);

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
			e_read(&dev, i, j, 0x3000, &flag1, sizeof(flag1));
			
			// Print the message and close the workgroup.
			if((flag==(unsigned)0xdeadbeef)&&(flag1==(unsigned)0xdeadbeef))
			{
				fprintf(stderr, "PASS!\n");
			}else
			{
				fprintf(stderr,"Fail!\nThe interrupt output is 0x%08x!\nThe return output is 0x%08x!\n",flag,flag1);
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
