#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "e_lib.h"

#define pi (3.141592653);

char outbuf[4096] SECTION("shared_dram");
int main(void)
{
	e_coreid_t coreid;
	unsigned int i;
	unsigned int num;
	unsigned int time_p;
	unsigned int time_c;
	unsigned int time_compare;
	unsigned int list[11] = {4, 3, 3, 145472, 68208, 26401, 55529, 115553, 18528, 16964, 236431};
	float volatile af;
	float volatile bf;
	float volatile cf;
	float volatile df;
	float ref;	
	unsigned int temp;

	float volatile in_sin;
	float volatile in_cos;
	float volatile in_sqt;
	float volatile in_ceil;
	float volatile in_log;
	float re_f0, re_f1, re_f2, re_f3, re_f4, re_f5;

	af = 3.5f;
	bf = 2.8f;	
	cf = 8.0f;
	df = 3.0f;
	in_sin = (float) pi;
	in_sin = in_sin / 6 ;
	in_cos = (float) pi;
	in_cos = in_cos / 3 ;
	in_sqt = 0.25f;
	in_ceil = 2.5f;
	in_log = 100.0f;

	// Who am I? Query the CoreID from hardware.
	coreid = e_get_coreid();
	sprintf(outbuf, "");

	// Get time waste on functions
	e_ctimer_set(E_CTIMER_0,  E_CTIMER_MAX) ;
	e_ctimer_start(E_CTIMER_0, E_CTIMER_CLK);
	time_p = e_ctimer_get(E_CTIMER_0);
	time_c = e_ctimer_get(E_CTIMER_0);
	e_ctimer_stop(E_CTIMER_0);
	time_compare = time_p - time_c ;
	
	// Addition
	// E_CTIMER0
	e_ctimer_set(E_CTIMER_0,  E_CTIMER_MAX) ;
	e_ctimer_start(E_CTIMER_0, E_CTIMER_CLK);
	time_p = e_ctimer_get(E_CTIMER_0);
		
		ref = bf + af ;

	
	time_c = e_ctimer_get(E_CTIMER_0);
	e_ctimer_stop(E_CTIMER_0);
			
	temp = time_p - time_c - time_compare;

	if (!((ref > 6.299f)&&(ref < 6.301f)) )
	{
		sprintf(outbuf, "\n Addition is wrong!\n");
	}else
	{
		if ( (temp >=(list[0] - 2)) && (temp <=(list[0] + 2)) )
			{
				sprintf(outbuf , "\nPASS for addition!\n");
			}else
			{
				sprintf(outbuf , "\nThe clock cycle for addition is %d instead of %d.\n", temp, list[0]);
			}
	}
	
	// Subtraction
	// E_CTIMER0
	e_ctimer_set(E_CTIMER_0,  E_CTIMER_MAX) ;
	e_ctimer_start(E_CTIMER_0, E_CTIMER_CLK);
	time_p = e_ctimer_get(E_CTIMER_0);
		
		ref =  af - bf;

	
	time_c = e_ctimer_get(E_CTIMER_0);
	e_ctimer_stop(E_CTIMER_0);
			
	temp = time_p - time_c - time_compare;

	if ( !((ref > 0.699f)&&(ref < 0.701f)))
	{
		sprintf(outbuf+strlen(outbuf), "\n Subtraction is wrong!\n");
	}else
	{
		if ((temp >= (list[1] - 2)) && (temp <= (list[1] + 2)) )
			{
				sprintf(outbuf +strlen(outbuf) , "\nPASS for substraction!\n");
			}else
			{
				sprintf(outbuf +strlen(outbuf), "\nThe clock cycle for subtraction is %d instead of %d.\n", temp, list[1]);
			}
	}
	
	// Mul
	//E_CTIMER0
	e_ctimer_set(E_CTIMER_0, E_CTIMER_MAX) ;
	e_ctimer_start(E_CTIMER_0, E_CTIMER_CLK);
	time_p = e_ctimer_get(E_CTIMER_0);
		
		ref = af * bf;
	
	time_c = e_ctimer_get(E_CTIMER_0);
	e_ctimer_stop(E_CTIMER_0);
			
	temp = time_p - time_c - time_compare;

	if (!((ref > 9.799f)&&(ref < 9.801f)))
	{
		sprintf(outbuf+strlen(outbuf), "\n Multiplication is wrong!\n");
	}else
	{
		if ((temp > (unsigned) (list[2] -2)) && (temp < (unsigned) (list[2]+2)) )
			{
				sprintf(outbuf +strlen(outbuf), "\nPASS for multiplication!\n");
			}else
			{
				sprintf(outbuf +strlen(outbuf), "\nThe clock cycle for multiplication is %d instead of %d.\n", temp, list[2]);
			}
	}
	
	// Div
	//E_CTIMER0
	e_ctimer_set(E_CTIMER_0, E_CTIMER_MAX) ;
	e_ctimer_start(E_CTIMER_0, E_CTIMER_CLK);
	time_p = e_ctimer_get(E_CTIMER_0);
	
		ref = ( af / bf);
	
	time_c = e_ctimer_get(E_CTIMER_0);
	e_ctimer_stop(E_CTIMER_0);
			
	temp = time_p - time_c - time_compare;

	if (!((ref > 1.2499f)&&(ref < 1.2501f)))
	{
		sprintf(outbuf+strlen(outbuf), "\n Division is wrong!\n");
	}else
	{
		if ( (temp > (unsigned) (list[3] * 0.8)) && (temp < (unsigned) (list[3] * 1.2)) )
			{
				sprintf(outbuf +strlen(outbuf), "\nPASS for division!\n");
			}else
			{
				sprintf(outbuf +strlen(outbuf), "\nThe clock cycle for division is %d instead of %d.\n", temp, list[3]);
			}
	}

	// Mod
	// E_CTIMER0
	e_ctimer_set(E_CTIMER_0, E_CTIMER_MAX) ;
	e_ctimer_start(E_CTIMER_0, E_CTIMER_CLK);
	time_p = e_ctimer_get(E_CTIMER_0);
	
		ref = fmodf(cf,df);

	
	time_c = e_ctimer_get(E_CTIMER_0);
	e_ctimer_stop(E_CTIMER_0);
			
	temp = time_p - time_c - time_compare;

	if (!((ref > 1.99f)&&(ref < 2.01f)))
	{
		sprintf(outbuf+strlen(outbuf), "\n Mod is wrong!\n");
	}else
	{
		if ( (temp > (unsigned)(list[4] * 0.8)) && (temp < (unsigned)(list[4] * 1.2)) )
			{
				sprintf(outbuf +strlen(outbuf), "\nPASS for mod!\n");
			}else
			{
				sprintf(outbuf +strlen(outbuf), "\nThe clock cycle for mod is %d instead of %d.\n", temp, list[4]);
			}
	}
	
	// Sin
	// E_CTIMER0
	e_ctimer_set(E_CTIMER_0, E_CTIMER_MAX) ;
	e_ctimer_start(E_CTIMER_0, E_CTIMER_CLK);
	time_p = e_ctimer_get(E_CTIMER_0);	
		 
	re_f0 = sinf(in_sin);

	time_c = e_ctimer_get(E_CTIMER_0);
	e_ctimer_stop(E_CTIMER_0);
			
	temp = time_p - time_c - time_compare;

	if ( !((re_f0 > 0.499) && (re_f0 < 0.501)) )
	{
		sprintf(outbuf+strlen(outbuf), "\n Sin is wrong!\n");		
	}else
	{
		if ( (temp > (unsigned)(list[5] * 0.8)) && (temp < (unsigned)(list[5] * 1.2)) )
			{
				sprintf(outbuf +strlen(outbuf), "\nPASS for sin!\n");
			}else
			{
				sprintf(outbuf +strlen(outbuf), "\nThe clock cycle for sin is %d instead of %d.\n", temp, list[5]);
			}
	}
	
	// Cos
	// E_CTIMER0
	e_ctimer_set(E_CTIMER_0, E_CTIMER_MAX) ;
	e_ctimer_start(E_CTIMER_0, E_CTIMER_CLK);
	time_p = e_ctimer_get(E_CTIMER_0);	
		 
	re_f1 = cosf(in_cos);

	time_c = e_ctimer_get(E_CTIMER_0);
	e_ctimer_stop(E_CTIMER_0);
			
	temp = time_p - time_c - time_compare;
	
	if (  !((re_f1 > 0.499) && (re_f1 < 0.501)))
	{
		sprintf(outbuf+strlen(outbuf), "\n Cos is wrong!\n");
	}else
	{
		if ( (temp > (unsigned)(list[6] * 0.8)) && (temp < (unsigned)(list[6] * 1.2)) )
			{
				sprintf(outbuf +strlen(outbuf), "\nPASS for cos!\n");
			}else
			{
				sprintf(outbuf +strlen(outbuf), "\nThe clock cycle for cos is %d instead of %d.\n", temp, list[6]);
			}
	}
	

	// Sqrt
	e_ctimer_set(E_CTIMER_0,  E_CTIMER_MAX) ;
	e_ctimer_start(E_CTIMER_0, E_CTIMER_CLK);
	time_p = e_ctimer_get(E_CTIMER_0);	
		 
	re_f2 = sqrtf(in_sqt);

	time_c = e_ctimer_get(E_CTIMER_0);
	e_ctimer_stop(E_CTIMER_0);
			
	temp = time_p - time_c - time_compare;

	if ( !((re_f2 > 0.499) && (re_f2 < 0.501)) )
	{
		sprintf(outbuf+strlen(outbuf), "\n Sqrt is wrong!\n");		
	}else
	{
		if ( (temp > (list[7] * 0.8)) && (temp < (list[7] * 1.2)) )
			{
				sprintf(outbuf +strlen(outbuf), "\nPASS for square root!\n");
			}else
			{
				sprintf(outbuf +strlen(outbuf), "\nThe clock cycle for sqrt is %d instead of %d.\n", temp, list[7]);
			}
	}
	
	// Ceil
	e_ctimer_set(E_CTIMER_0,  E_CTIMER_MAX) ;
	e_ctimer_start(E_CTIMER_0, E_CTIMER_CLK);
	time_p = e_ctimer_get(E_CTIMER_0);	
		 
	re_f3 = ceilf(in_ceil);

	time_c = e_ctimer_get(E_CTIMER_0);
	e_ctimer_stop(E_CTIMER_0);
			
	temp = time_p - time_c - time_compare;

	if ( !((re_f3 > 2.99) && (re_f3 < 3.01)) )
	{
		sprintf(outbuf+strlen(outbuf), "\n Ceil is wrong!\n");	
	}else
	{
		if ( (temp > (list[8] * 0.8)) && (temp < (list[8] * 1.2)) )
			{
				sprintf(outbuf +strlen(outbuf), "\nPASS for ceil!\n");
			}else
			{
				sprintf(outbuf +strlen(outbuf), "\nThe clock cycle for ceil is %d instead of %d.\n", temp, list[8]);
			}
	}

	// Floor
	e_ctimer_set(E_CTIMER_0,  E_CTIMER_MAX) ;
	e_ctimer_start(E_CTIMER_0, E_CTIMER_CLK);
	time_p = e_ctimer_get(E_CTIMER_0);	
		 
	re_f5 = floorf(in_ceil);

	time_c = e_ctimer_get(E_CTIMER_0);
	e_ctimer_stop(E_CTIMER_0);
			
	temp = time_p - time_c - time_compare;

	if ( !((re_f5 > 1.99f) && (re_f5 < 2.01f))  )
	{
		sprintf(outbuf+strlen(outbuf), "\n Floor is wrong!\n");
	}else
	{
		if ( (temp > (list[9] * 0.8)) && (temp < (list[9] * 1.2)) )
			{
				sprintf(outbuf +strlen(outbuf), "\nPASS for floor!\n");
			}else
			{
				sprintf(outbuf +strlen(outbuf), "\nThe clock cycle for floor is %d instead of %d.\n", temp, list[9]);
			}
	}
	

	// Log10
	e_ctimer_set(E_CTIMER_0, E_CTIMER_MAX) ;
	e_ctimer_start(E_CTIMER_0, E_CTIMER_CLK);
	time_p = e_ctimer_get(E_CTIMER_0);	
		 
	re_f4 = log10f(in_log);

	time_c = e_ctimer_get(E_CTIMER_0);
	e_ctimer_stop(E_CTIMER_0);
			
	temp = time_p - time_c - time_compare;

	if (!( (re_f4 > 1.99f) && (re_f4 < 2.01f)) )
	{
		sprintf(outbuf+strlen(outbuf), "\n Log10 is wrong!\n");
	}else
	{ 
		if ( (temp > (list[10] * 0.8)) && (temp < (list[10] * 1.2)) )
			{
				sprintf(outbuf +strlen(outbuf), "\nPASS for log10!\n");
			}else
			{
				sprintf(outbuf +strlen(outbuf), "\nThe clock cycle for log10 is %d instead of %d.\n", temp, list[10]);
			}
	}

	
	return EXIT_SUCCESS;

}
