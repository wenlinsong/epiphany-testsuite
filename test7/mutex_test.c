#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <e-hal.h>

#define _BufSize   (4096)
#define _BufOffset (0x01000000)




int main(int argc, char *argv[])
{
	unsigned row, col, coreid, i, j, m, n, k;
	e_platform_t platform;
	e_epiphany_t dev;
	e_mem_t emem;
	char emsg[_BufSize];
	unsigned num;
	unsigned counter = 0;
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
	
	
	// Load the device program onto core (0,0)
	e_load("e_mutex_test0.srec", &dev, 0, 0, E_TRUE);

	usleep(10000);
	// Load the device program onto all the other eCores
	e_load_group("e_mutex_test.srec", &dev, 0, 1, 1, 3, E_TRUE);
	e_load_group("e_mutex_test.srec", &dev, 1, 0, 3, 4, E_TRUE);
	
	usleep(100000);
	

			
		// Wait for core program execution to finish
		// Read message from shared buffer
				
		e_read(&dev, 0, 0, 0x6200, &num, sizeof(num));
		e_read(&dev, 0, 0, 0x6300, &counter, sizeof(counter));

		// Print the message and close the workgroup.
		fprintf(stderr, "The counter now is %d!\n", counter);
		fprintf(stderr, "The clock cycle is %d!\n", num);
		
	// Close the workgroup
	e_close(&dev);
	
	// Release the allocated buffer and finalize the
	// e-platform connection.
	e_free(&emem);
	e_finalize();

	return 0;
}

