#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <e-hal.h>

#define _BufSize   (8192)
#define _BufOffset (0x01000000)
#define _SeqLen    (4)
#define _SeqLen_row  (4)
#define _SeqLen_col  (4)



int main(int argc, char *argv[])
{
	unsigned row, col, coreid, i, j, m, n, k;
	e_platform_t platform;
	e_epiphany_t dev;
	e_mem_t emem;
	char emsg[_BufSize];
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
	e_load_group("e_nested_test.srec", &dev, 0, 0, platform.rows, platform.cols, E_FALSE);

	// Select one core to work 
	for (i=0; i<platform.rows; i++)
	{
		for (j=0; j<platform.cols; j++)
		{
			// Draw to a certain core
			row=i;
			col=j;
			coreid = (row + platform.row) * 64 + col + platform.col;
			fprintf(stderr,"%d: Message from eCore 0x%03x (%2d,%2d): \n",(i*platform.cols+j),coreid,row,col);
		
			e_start(&dev, i, j);			
			usleep(1000000);
			
			// Wait for core program execution to finish
			// Read message from shared buffer
				
			e_read(&emem, 0, 0, 0x0, &emsg, _BufSize);

			// Print the message and close the workgroup.
			fprintf(stderr, "%s\n", emsg);
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

