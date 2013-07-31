#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <e-hal.h>

int main(int argc, char *argv[])
{
	unsigned row, col, coreid, i, j, m, n, k;
	e_platform_t platform;
	e_epiphany_t dev;
	unsigned time_write;
	unsigned time_read;
	float result1;
	float result2;
	srand(1);
	

	// initialize system, read platform params from
	// default HDF. Then, reset the platform and
	e_init(NULL);
	e_reset_system();
	e_get_platform_info(&platform);
	
    	// Open a workgroup
	e_open(&dev, 0, 0, platform.rows, platform.cols);
	
	// Load the device program onto core (0,0)
	e_load_group("e_mesh_bandwidth_near.srec", &dev, 0, 0, platform.rows, platform.cols, E_FALSE);

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
			e_start(&dev, row, col);
			usleep(500000);
			// Wait for core program execution to finish
			// Read message from shared buffer
				
			e_read(&dev, row, col, 0x5000, &time_write, sizeof(time_write));
			e_read(&dev, row, col, 0x5004, &time_read, sizeof(time_read));
			
			// Calculate the bandwidth
			result1 = (32*585937.5)/time_write;
			result2 = (32*585937.5)/time_read;
			// Print the message and close the workgroup
			fprintf(stderr, "%.2fMB\n%.2fMB\n", result1, result2);
			
		}
	}

	// Close the workgroup
	e_close(&dev);
	
	// Finalize the e-platform connection.
	e_finalize();

	return 0;
}
