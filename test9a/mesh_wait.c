#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <e-hal.h>
#define mas_row (0)
#define mas_col (0)

int main(int argc, char *argv[])
{
	unsigned row, col, coreid, i, j, k;
	e_platform_t platform;
	e_epiphany_t dev;
	e_mem_t emem;
	unsigned time;
	unsigned time1;
	unsigned signal = 0xdeadbeef;
	row = mas_row;
	col = mas_col;
	srand(1);
	

	// initialize system, read platform params from
	// default HDF. Then, reset the platform and
	// get the actual system parameters.
	e_init(NULL);
	e_reset_system();
	e_get_platform_info(&platform);

    	// Open a workgroup
	e_open(&dev, 0, 0, platform.rows, platform.cols);
	
	
	// Load the device program onto core 	

	e_load("e_mesh_wait_r.srec", &dev, mas_row, mas_col, E_TRUE);
	
	usleep(10000);
	
	// Let other cores know the core id of the specific core
	for(i=0; i<platform.rows; i++)
	{
		for(j=0; j<platform.cols; j++)
		{
			e_write(&dev, i, j, 0x6000, &row, sizeof(row));
			e_write(&dev, i, j, 0x6004, &col, sizeof(col));
	
		}
	}
	
	//for (i=0; i<(platform.rows-2); i++)
	//{
	//	for(j=0; j<(platform.cols-2); j++)
	//	{
	//		if((i!=mas_row)|(j!=mas_col))
	//		{
				e_load("e_mesh_wait.srec",&dev, mas_row, mas_col+1, E_TRUE);
				e_load("e_mesh_wait.srec",&dev, mas_row+1, mas_col, E_TRUE);
	//		}
	//	}
	//}
	
	usleep(10000);
	
	// Sent the signal to start transfer

	e_write(&dev, mas_row, mas_col, 0x6100, &signal, sizeof(signal));
	
	
	usleep(1000000);
			
	// Read message from shared buffer
	
	e_read(&dev, mas_row, mas_col, 0x5000, &time, sizeof(time));
	e_read(&dev, mas_row, mas_col+1, 0x7000, &time1, sizeof(time1));
	
	// Print the message and close the workgroup.

	fprintf(stderr, "0x%08x\n", time);
	fprintf(stderr, "0x%08x\n", time1);
			
	// Close the workgroup
	e_close(&dev);
	
	// Release the allocated buffer and finalize the
	// e-platform connection.
	e_free(&emem);
	e_finalize();

	return 0;
}
