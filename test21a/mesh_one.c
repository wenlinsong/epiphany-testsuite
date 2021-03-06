#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <e-hal.h>
#define mas_row (3)
#define mas_col (3)

int main(int argc, char *argv[])
{
	unsigned row, col, coreid, i, j, m, n, k;
	e_platform_t platform;
	e_epiphany_t dev;
	unsigned time;
	unsigned signal = 0xdeadbeef;
	unsigned clr = 0x00000000;
	unsigned master_row, master_col;
	master_row = mas_row;
	master_col = mas_col;
	float result1;
	srand(1);
	

	// initialize system, read platform params from
	// default HDF. Then, reset the platform and
	e_init(NULL);
	e_reset_system();
	e_get_platform_info(&platform);
	
    	// Open a workgroup
	e_open(&dev, 0, 0, platform.rows, platform.cols);
	
	// Load the device program onto core (0,0)
	e_load("e_mesh_one.srec", &dev, mas_row, mas_col, E_TRUE);
	
	usleep(10000);
	
	// Let other cores know the core id of the specific core
	for(i=0; i<platform.rows; i++)
	{
		for(j=0; j<platform.cols; j++)
		{
			e_write(&dev, i, j, 0x6000, &master_row, sizeof(master_row));
			e_write(&dev, i, j, 0x6004, &master_col, sizeof(master_col));
	
		}
	}
	
	for (i=0; i<platform.rows; i++)
	{
		for(j=0; j<platform.cols; j++)
		{
			if((i!=mas_row)|(j!=mas_col))
			{
				e_load("e_mesh_one1.srec",&dev, i, j, E_TRUE);
			}
		}
	}
	
	usleep(10000);
	
	// Sent the signal to start transfer

	e_write(&dev, mas_row, mas_col, 0x6100, &signal, sizeof(signal));
	
	
	usleep(1000000);		
	// Read message from shared buffer
	
	e_read(&dev, mas_row, mas_col, 0x5000, &time, sizeof(time));
		
			
	// Calculate the bandwidth
	result1 = (120*585938)/(time);
	
	// Print the message and close the workgroup
	
	fprintf(stderr, "0x%08x!\n", time);
	
	fprintf(stderr, "The bandwidth of all-to-one is %.2fMB/s!\n", result1);		
	
	// Close the workgroup
	e_close(&dev);
	
	// Finalize the e-platform connection.
	e_finalize();

	return 0;
}
