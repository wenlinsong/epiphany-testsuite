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
	unsigned time[8];
	unsigned signal = 0xdeadbeef;
	unsigned clr = 0x00000000;
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
	e_load("e_mesh_bi00.srec", &dev, 0, 0, E_TRUE);
	e_load_group("e_mesh_bi.srec", &dev, 1, 0, (platform.rows-1), 1, E_TRUE);
	e_load_group("e_mesh_bi1.srec", &dev, 0, 1, platform.rows, 1, E_TRUE);
	e_load_group("e_mesh_rec.srec", &dev, 0, 2, platform.rows, 1, E_TRUE);
	e_load_group("e_mesh_rec1.srec", &dev, 0, 3, platform.rows, 1, E_TRUE);
	

	usleep(10000);
	
	// Sent the signal to start transfer
	e_write(&dev, 0, 0, 0x6100, &signal, sizeof(signal));	
	
	
	usleep(1000000);		
	// Read message from shared buffer
	for(i=0; i<platform.rows; i++)
	{
		for(j=(platform.cols/2); j<(platform.cols); j++)
		{
			e_read(&dev, i, j, 0x5000, &time[i*2+j-2], sizeof(time[0]));
		}
	}		
			
	// Calculate the bandwidth
	result1 = (37500000)/(time[0]+time[1]+time[2]+time[3]+time[4]+time[5]+time[6]+time[7]);
	//result2 = (32*585937.5)/time_read;
	// Print the message and close the workgroup
	for(k=0; k<8; k++)
	{
		fprintf(stderr, "0x%08x!\n", time[k]);
	}
	
	fprintf(stderr, "The bandwidth of bisection is %.2fMB/s!\n", result1);		
	
	// Close the workgroup
	e_close(&dev);
	
	// Finalize the e-platform connection.
	e_finalize();

	return 0;
}
