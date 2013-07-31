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
	e_ctimer_set(E_CTIMER_0, E_CTIMER_MAX) ;
	e_ctimer_start(E_CTIMER_0, E_CTIMER_CLK);
	time_p = e_ctimer_get(E_CTIMER_0);
	time_c = e_ctimer_get(E_CTIMER_0);
	e_ctimer_stop(E_CTIMER_0);
	time_compare = time_p - time_c ;

	// Addition
	// E_CTIMER0
	e_ctimer_set(E_CTIMER_0, E_CTIMER_MAX) ;
	e_ctimer_start(E_CTIMER_0, E_CTIMER_CLK);
	time_p = e_ctimer_get(E_CTIMER_0);
		
		ref = bf + af ;

	
	time_c = e_ctimer_get(E_CTIMER_0);
	e_ctimer_stop(E_CTIMER_0);
			
	temp = time_p - time_c - time_compare;

	if (!((ref > 6.299f)&&(ref < 6.301f)))
	{
		sprintf(outbuf, "\n Addition is wrong!\n");
	}else
	{
		sprintf(outbuf , "\nE_CTIMER0: The clock cycle of addition is %d.\n", temp);
	}
	
	// Subtraction
	// E_CTIMER0
	e_ctimer_set(E_CTIMER_0, E_CTIMER_MAX) ;
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
		sprintf(outbuf+strlen(outbuf) , "\n E_CTIMER0: The clock cycle of subtraction is %d.\n", temp);
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
		sprintf(outbuf+strlen(outbuf) , "\nE_CTIMER0: The clock cycle of multiplication is %d.\n", temp);
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
		sprintf(outbuf+strlen(outbuf) , "\nE_CTIMER0: The clock cycle of division is %d.\n", temp);
	}

	// Mod
	// E_CTIMER0
	e_ctimer_set(E_CTIMER_0,  E_CTIMER_MAX) ;
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
		sprintf(outbuf+strlen(outbuf) , "\nE_CTIMER0: The clock cycle of mod is %d.\n", temp);
	}
	
	// Sin
	// E_CTIMER0
	e_ctimer_set(E_CTIMER_0,  E_CTIMER_MAX) ;
	e_ctimer_start(E_CTIMER_0, E_CTIMER_CLK);
	time_p = e_ctimer_get(E_CTIMER_0);	
		 
	re_f0 = sinf(in_sin);

	time_c = e_ctimer_get(E_CTIMER_0);
	e_ctimer_stop(E_CTIMER_0);
			
	temp = time_p - time_c - time_compare;

	if ( (re_f0 > 0.499) && (re_f0 < 0.501) )
	{
		sprintf(outbuf+strlen(outbuf) , "\nE_CTIMER0: The clock cycle of sin is %d.\n", temp);		
	}else
	{
		sprintf(outbuf+strlen(outbuf), "\n Sin is wrong!\n");
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
	
	if (  (re_f1 > 0.499) && (re_f1 < 0.501))
	{
		sprintf(outbuf+strlen(outbuf) , "\nE_CTIMER0: The clock cycle of cos is %d.\n", temp);
	}else
	{
		sprintf(outbuf+strlen(outbuf), "\n Cos is wrong!\n");
	}
	

	// Sqrt
	e_ctimer_set(E_CTIMER_0,  E_CTIMER_MAX) ;
	e_ctimer_start(E_CTIMER_0, E_CTIMER_CLK);
	time_p = e_ctimer_get(E_CTIMER_0);	
		 
	re_f2 = sqrtf(in_sqt);

	time_c = e_ctimer_get(E_CTIMER_0);
	e_ctimer_stop(E_CTIMER_0);
			
	temp = time_p - time_c - time_compare;

	if ( (re_f2 > 0.499) && (re_f2 < 0.501) )
	{
		sprintf(outbuf+strlen(outbuf) , "\nE_CTIMER0: The clock cycle of sqrt is %d.\n", temp);		
	}else
	{
		sprintf(outbuf+strlen(outbuf), "\n Sqrt is wrong!\n");
	}
	
	// Ceil
	e_ctimer_set(E_CTIMER_0,  E_CTIMER_MAX) ;
	e_ctimer_start(E_CTIMER_0, E_CTIMER_CLK);
	time_p = e_ctimer_get(E_CTIMER_0);	
		 
	re_f3 = ceilf(in_ceil);

	time_c = e_ctimer_get(E_CTIMER_0);
	e_ctimer_stop(E_CTIMER_0);
			
	temp = time_p - time_c - time_compare;

	if ( (re_f3 > 2.99) && (re_f3 < 3.01) )
	{
		sprintf(outbuf+strlen(outbuf) , "\nE_CTIMER0: The clock cycle of ceil is %d.\n", temp);		
	}else
	{
		sprintf(outbuf+strlen(outbuf), "\n Ceil is wrong!\n");
	}

	// Floor
	e_ctimer_set(E_CTIMER_0,  E_CTIMER_MAX) ;
	e_ctimer_start(E_CTIMER_0, E_CTIMER_CLK);
	time_p = e_ctimer_get(E_CTIMER_0);	
		 
	re_f5 = floorf(in_ceil);

	time_c = e_ctimer_get(E_CTIMER_0);
	e_ctimer_stop(E_CTIMER_0);
			
	temp = time_p - time_c - time_compare;

	if ( (re_f5 > 1.99f) && (re_f5 < 2.01f)  )
	{
		sprintf(outbuf+strlen(outbuf) , "\nE_CTIMER0: The clock cycle of floor is %d.\n", temp);
	}else
	{
		sprintf(outbuf+strlen(outbuf), "\n Floor is wrong!\n");
	}
	

	// Log10
	e_ctimer_set(E_CTIMER_0, E_CTIMER_MAX) ;
	e_ctimer_start(E_CTIMER_0, E_CTIMER_CLK);
	time_p = e_ctimer_get(E_CTIMER_0);	
		 
	re_f4 = log10f(in_log);

	time_c = e_ctimer_get(E_CTIMER_0);
	e_ctimer_stop(E_CTIMER_0);
			
	temp = time_p - time_c - time_compare;

	if ( (re_f4 > 1.99f) && (re_f4 < 2.01f) )
	{
		sprintf(outbuf+strlen(outbuf) , "\nE_CTIMER0: The clock cycle of log10 is %d.\n", temp);
	}else
	{
		sprintf(outbuf+strlen(outbuf), "\n Log10 is wrong!\n");
	}

	
	return EXIT_SUCCESS;

}
