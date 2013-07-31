#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "e_lib.h"

void int_isr(int signum);
unsigned *mailbox, *mailbox1;

int main(void)
{
	mailbox = (unsigned *)0x00006000;
	mailbox1 = (unsigned *)0x00006100;

	// Initialize the mailbox
	mailbox[0]  = 0x00000000;
	mailbox1[0] = 0x00000000;

	// Preparing for the interrupt
	// Attach the ISR with this interrupt
	e_irq_attach(E_MESSAGE_INT, int_isr);

	// Clear the IMASK
	e_irq_mask(E_MESSAGE_INT, E_FALSE);
	
	// Enable the global interrupt
	e_irq_global_mask(E_FALSE);
	
	// Making slave_core into idle and wait for the finishing transfer interrupt
	__asm__ __volatile__("idle");

	// Showing coming back from the interrupt
	mailbox1[0] = 0xffffffff;

	// Set the IMASK
	e_irq_mask(E_MESSAGE_INT, E_TRUE);
	
	// Disable the global interrupt
	e_irq_global_mask(E_TRUE);
	
	return 0;
}

void __attribute__((interrupt)) int_isr(int signum)
{
	mailbox[0] = 0xdeadbeef;
	return;
}	

	
	 
