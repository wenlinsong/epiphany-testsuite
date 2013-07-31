#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <e-hal.h>

#define _BufSize   (1024)
#define _BufOffset (0x01000000)
#define r_row (1)
#define r_col (1)
#define t_row (2)
#define t_col (1)


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
	e_load("e_dma_slave_test.srec", &dev, r_row, r_col, E_FALSE);
	e_load("e_dma_slave_test1.srec", &dev, t_row, t_col, E_FALSE);

	// Launch to each core
	
	row=r_row;
	col=r_col;
	coreid = (row + platform.row) * 64 + col + platform.col;
	fprintf(stderr,"%3d: Message from eCore 0x%03x (%2d,%2d) : \n",(row*platform.cols+col),coreid,row,col);
	// Start device
	e_start(&dev, r_row, r_col);
	usleep(10000);
	
	e_write(&dev, t_row, t_col, 0x6500, &row, sizeof(row));
	e_write(&dev, t_row, t_col, 0x6504, &col, sizeof(col));
	
	usleep(1000);
	
	e_start(&dev, t_row, t_col);
	usleep(300000);
	
	// Wait for core program execution to finish
	// Read message from shared buffer
				
	e_read(&dev, t_row, t_col, 0x6100, &flag, sizeof(flag));
			
	// Print the message and close the workgroup.
	if(flag==(unsigned)0xffffffff)
	{
		fprintf(stderr, "PASS!\n");
	}else
	{
		fprintf(stderr,"Fail!\n");
	}

	// Close the workgroup
	e_close(&dev);
	
	// Release the allocated buffer and finalize the
	// e-platform connection.
	//e_free(&emem);
	e_finalize();

	return 0;
}
