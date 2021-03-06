#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "e_lib.h"


char outbuf[128] SECTION("shared_dram");
unsigned int clear_value(unsigned int prev,unsigned int mask, unsigned int *p);
unsigned int set_value(unsigned int prev,unsigned int mask, unsigned int *p);

int main(void)
{
	e_coreid_t coreid;
	unsigned int *po;
	unsigned int reg;
	unsigned int p_value;
	unsigned int flag3, flag4, flag5;
	unsigned int temp;		
	unsigned int k;	
	po=(unsigned) 0x0000600c;
	po[0] = 0x00000000;

	while(1)
	{
		while(po[0] == (unsigned) 0x00000000)
		{};
		
		sprintf(outbuf,"");
		// Store the previous value of the register	
		p_value = e_reg_read(*po);
		// Disable interrupts

		e_irq_global_mask(E_TRUE);	

		//For register EILAT
			// Clear the EILAT
			e_reg_write(E_REG_ILATCL, (unsigned)0xffffffff);
			temp = 	e_reg_read(*po);
			if (temp == (unsigned)0x00000000)
			{
				flag3=1;
			}else{
				flag3=0;
				sprintf(outbuf,"We get 0x%08x instead of 0x%08x\n", temp, 0x00000000);
			}
			
			// Set the EILAT
			e_reg_write(E_REG_ILATST, (unsigned)0xffffffff);
			temp = 	e_reg_read(*po);
			if (temp == (unsigned)0x000003ff)
			{
				flag4=1;
			}else{
				flag4=0;
				sprintf(outbuf + strlen(outbuf),"We get 0x%08x instead of 0x%08x\n", temp, 0x000003ff);
			}
			
			// Clear the EILAT
			
			e_reg_write(E_REG_ILATCL, (unsigned)0xffffffff);
			temp = 	e_reg_read(*po);
			if (temp == (unsigned)0x00000000)
			{
				flag5=1;
			}else{
				flag5=0;
				sprintf(outbuf + strlen(outbuf),"We get 0x%08x instead of 0x%08x\n", temp, 0x00000000);
			}
			
			if ((flag3 == 1) && (flag4 == 1) && (flag5 == 1))
			{
				sprintf(outbuf , "PASS!");
			}
			
			// Set the register to the previous value
			e_reg_write(*po, p_value);
			po[0] = (unsigned) 0x00000000;

			// Enable the interrupts
			e_irq_global_mask(E_FALSE);
	}

			return EXIT_SUCCESS;
}
			
