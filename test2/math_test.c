#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <e-hal.h>

#define _BufSize   (4096)
#define _BufOffset (0x01000000)



int main(int argc, char *argv[])
{
	unsigned row, col, coreid, i,j,m,n,k;
	unsigned int mask0, mask1;
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
	{	for(n=0; n< platform.cols;n++)
		{	
			e_reset_core(&dev, m, n);
		}
	}

	// Load the device program onto all the eCores
	// To get the verified values
	//e_load_group("e_math_test.srec", &dev, 0, 0, platform.rows,  platform.cols, E_FALSE);

	// To test 
	e_load_group("e_math_test1.srec", &dev, 0, 0, platform.rows, platform.cols, E_FALSE);
	
	for (i=0; i<platform.rows ; i++)
	{	
		for (j=0; j<platform.cols; j++)
			{

				//Draw to a certain core
				row=i;
				col=j;
				coreid = (row + platform.row) * 64 + col + platform.col;
				fprintf(stderr,"%3d: Message from eCore 0x%03x (%2d,%2d) : \n",(i*platform.cols+j),coreid,row,col);
				e_start(&dev, i, j);
				usleep(100000);
			
				// Wait for core program execution to finish
				// Read message from shared buffer
			
			
				e_read(&emem, 0, 0, 0x0, emsg, _BufSize);

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
