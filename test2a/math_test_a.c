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
	unsigned int emsg[11];


	srand(1);

	// initialize system, read platform params from
	// default HDF. Then, reset the platform and
	// get the actual system parameters.

	e_init(NULL);
	e_reset_system();
	e_get_platform_info(&platform);

	
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
	// To test 
	e_load_group("e_math_test_a.srec", &dev, 0, 0, platform.rows, platform.cols, E_FALSE);
	
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
				usleep(500000);
			
				// Wait for core program execution to finish
				// Read message from shared buffer
			
			
				e_read(&dev, i, j, 0x6000, &emsg, sizeof(emsg));

				// Print the message and close the workgroup.
				fprintf(stderr, "Clock cycle for add is :");
				fprintf(stderr, "%6d\n", emsg[0]);
				fprintf(stderr, "Clock cycle for sub is :");
				fprintf(stderr, "%6d\n", emsg[1]);
				fprintf(stderr, "Clock cycle for mul is :");
				fprintf(stderr, "%6d\n", emsg[2]);
				fprintf(stderr, "Clock cycle for div is :");
				fprintf(stderr, "%6d\n", emsg[3]);
				fprintf(stderr, "Clock cycle for mod is :");
				fprintf(stderr, "%6d\n", emsg[4]);
				fprintf(stderr, "Clock cycle for sin is :");
				fprintf(stderr, "%6d\n", emsg[5]);
				fprintf(stderr, "Clock cycle for cos is :");
				fprintf(stderr, "%6d\n", emsg[6]);
				fprintf(stderr, "Clock cycle for sqrt is :");
				fprintf(stderr, "%6d\n", emsg[7]);
				fprintf(stderr, "Clock cycle for ceil is :");
				fprintf(stderr, "%6d\n", emsg[8]);
				fprintf(stderr, "Clock cycle for floor is :");
				fprintf(stderr, "%6d\n", emsg[9]);
				fprintf(stderr, "Clock cycle for log10 is :");
				fprintf(stderr, "%6d\n", emsg[10]);
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
