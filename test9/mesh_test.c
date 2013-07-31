#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <e-hal.h>
#define _BufOffset (0x01000000)
#define _BufSize (0x2000)
#define mas_row (1)
#define mas_col (1)

int main(int argc, char *argv[])
{
	unsigned row, col, coreid, i, j, k;
	e_platform_t platform;
	e_epiphany_t dev;
	e_mem_t emem;
	unsigned time[13];
	unsigned *p;
	p = &time[0];
	srand(1);
	

	// initialize system, read platform params from
	// default HDF. Then, reset the platform and
	// get the actual system parameters.
	e_init(NULL);
	e_reset_system();
	e_get_platform_info(&platform);

	// Allocate a buffer in shared external memory
	// for message passing from eCore to host.
	e_alloc(&emem, _BufOffset, _BufSize);	
	
    	// Open a workgroup
	e_open(&dev, 0, 0, platform.rows, platform.cols);
	

	// Load the device program onto core (0,0)
	e_load("e_mesh_test.srec", &dev, mas_row, mas_col, E_FALSE);
	
	coreid = (mas_row + platform.row) * 64 + mas_col + platform.col;
	fprintf(stderr,"%3d: Message from eCore 0x%03x (%2d,%2d) : \n",(mas_row*platform.cols+mas_col),coreid,mas_row,mas_col);
	
	// Start device
	e_start(&dev, mas_row, mas_col);
	
	// Wait for core program execution to finish
	
	usleep(10000000);
	
	// Read message from shared buffer
				
	e_read(&dev, mas_row, mas_col, 0x6500, p, sizeof(time));

	// Print the message and close the workgroup.
	for (k=0; k<13; k++)
	fprintf(stderr, "0x%08x\n", p[k]);
			
	// Close the workgroup
	e_close(&dev);
	
	// Release the allocated buffer and finalize the
	// e-platform connection.
	e_free(&emem);
	e_finalize();

	return 0;
}
