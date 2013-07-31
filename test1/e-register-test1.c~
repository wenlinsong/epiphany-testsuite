#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "e_lib.h"

char outbuf[128] SECTION("shared_dram");

struct ff { int flag0; int flag1 ; };
struct ff check_register(unsigned int a, unsigned int b, unsigned int core_id, unsigned int *p );


int main(void)
{
	e_coreid_t coreid;
	struct ff f_1;
	struct ff f_2;
	unsigned int *po;
	po=0x00002000;

	// Who am I? Query the CoreID from hardware.
	coreid = e_get_coreid();
	
	//First check from 0 to 1
	f_1=check_register((unsigned) 0x00000000, (unsigned) 0xffffffff, coreid, po);
	
	//Second check from 1 to 0
	if(f_1.flag1==1)
	{
		f_2=check_register((unsigned) 0xffffffff, (unsigned) 0x00000000, coreid, po);
	}else
	{
		return EXIT_SUCCESS;
	};
	//print out the result
	if((f_1.flag1==1) && (f_2.flag1==1) )
	{
		sprintf(outbuf, "PASS at core 0x%03x!", coreid);	
	};
	
	return EXIT_SUCCESS;
}


struct ff check_register(unsigned int a, unsigned int b,unsigned int core_id, unsigned int *p)
{
	unsigned int temp;
	struct ff flag ;
	flag.flag0 = 0;
	flag.flag1 = 0;	

	// Write a to the register
	e_sysreg_write(*p,a);
 
	// Read from the register
	temp = e_sysreg_read(*p);
	//temp = (unsigned) 0x11001100;   // set a specific value to see if it real works
	
	// Check if the register is a
	if(temp == a)
	{
		flag.flag0=1;
		// Write b to the register
		e_sysreg_write(*p, b);
	
		// Read from the register
 		temp = e_sysreg_read(*p);

		// Check if the register is b
		if(temp == b)
		{
			flag.flag1=1;			
		}else
		{
			sprintf(outbuf,"At core 0x%03x, We get 0x%x instead of 0x%x", core_id, temp, b);
		};
	}else
	{
		sprintf(outbuf,"At core 0x%03x, We get 0x%x instead of 0x%x", core_id, temp, a);
	};

    return flag;		
}
