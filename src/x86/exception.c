/*
	Hüseyin Uslu, <shalafiraistlin@gmail.com>
*/

#include "include/exception.h"
#include "include/idt.h"
#include "../kernel/include/console.h"

/* exception handler */
void fault_handler(struct regs *r)
{
	/*unsigned char * exceptions[] =
	{
    		"Division By Zero",
    		"Debug",
    		"Non Maskable Interrupt",
    		"Breakpoint",
    		"Into Detected Overflow",
    		"Out of Bounds",
    		"Invalid Opcode",
    		"No Coprocessor",		
    		"Double Fault",
    		"Coprocessor Segment Overrun",
    		"Bad TSS",
    		"Segment Not Present",
    		"Stack Fault",
    		"General Protection Fault",
    		"Page Fault",
    		"Unknown Interrupt",
    		"Coprocessor Fault",
    		"Alignment Check",
    		"Machine Check",
    		"Reserved",
    		"Reserved",
		"Reserved",
    		"Reserved",
    		"Reserved",
    		"Reserved",
    		"Reserved",
    		"Reserved",
    		"Reserved",
    		"Reserved",
    		"Reserved",
    		"Reserved",
    		"Reserved"
	};*/
	
	
	
	/* Some sort of bug or incompatibility issue happened with
	* GCC 4.1. GCC tries to use memset and memcpy to initialize
	* dynamic array above but we force gcc to not to use standart
	* libraries and optimizations so GCC exits with error.
	* Declaring array with dimensions first than appending the values
	* solved this issue.
	*/
	
	unsigned char * exceptions[32][28]; // there are 32 exceptions, and max length for an exceptions is 27
	*exceptions[0] =  (unsigned char *)"Division By Zero";
	*exceptions[1] =  (unsigned char *)"Debug";
	*exceptions[2] =  (unsigned char *)"Non Maskable Interrupt";
	*exceptions[3] =  (unsigned char *)"Breakpoint";
	*exceptions[4] =  (unsigned char *)"Into Detected Overflow";
	*exceptions[5] =  (unsigned char *)"Out of Bounds";
	*exceptions[6] =  (unsigned char *)"Invalid Opcode";
	*exceptions[7] =  (unsigned char *)"No Coprocessor";
	*exceptions[8] =  (unsigned char *)"Double Fault";
	*exceptions[9] =  (unsigned char *)"Coprocessor Segment Overrun";
	*exceptions[10] = (unsigned char *)"Bad TSS";
	*exceptions[11] = (unsigned char *)"Segment Not Present";
	*exceptions[12] = (unsigned char *)"Stack Fault";
	*exceptions[13] = (unsigned char *)"General Protection Fault";
	*exceptions[14] = (unsigned char *)"Page Fault";
	*exceptions[15] = (unsigned char *)"Unknown Interrupt";
	*exceptions[16] = (unsigned char *)"Coprocessor Fault";
	*exceptions[17] = (unsigned char *)"Alignment Check";
	*exceptions[18] = (unsigned char *)"Machine Check";
	*exceptions[19] = (unsigned char *)"Reserved";
	*exceptions[20] = (unsigned char *)"Reserved";
	*exceptions[21] = (unsigned char *)"Reserved";
	*exceptions[22] = (unsigned char *)"Reserved";
	*exceptions[23] = (unsigned char *)"Reserved";
	*exceptions[24] = (unsigned char *)"Reserved";
	*exceptions[25] = (unsigned char *)"Reserved";
	*exceptions[26] = (unsigned char *)"Reserved";
	*exceptions[27] = (unsigned char *)"Reserved";
	*exceptions[28] = (unsigned char *)"Reserved";
	*exceptions[29] = (unsigned char *)"Reserved";
	*exceptions[30] = (unsigned char *)"Reserved";
	*exceptions[31] = (unsigned char *)"Reserved";
	
	/* print CPU state */
	printk("\nCPU Exception: %s [%d]\n",*exceptions[r->int_no],r->int_no);
	printk("error code: %d\n",r->err_code);
	printk("  CS:EIP: 0x%x:0x%x\n",r->cs,r->eip);
	printk("  EFLAGS: 0x%x\n",r->eflags);
	printk(" USERESP: 0x%x\n",r->useresp);
	printk("      SS: 0x%x\n",r->ss);
	printk("     EAX: 0x%x\n",r->eax);
	printk("     EBX: 0x%x\n",r->ebx);
	printk("     ECX: 0x%x\n",r->ecx);
	printk("     EDX: 0x%x\n",r->edx);
	printk("     ESP: 0x%x\n",r->esp);
	printk("     EBP: 0x%x\n",r->ebp);
	printk("     ESI: 0x%x\n",r->esi);
	printk("     EDI: 0x%x\n",r->edi);
	printk("      ES: 0x%x\n",r->es);
	printk("      DS: 0x%x\n",r->ds);
	printk("      GS: 0x%x\n",r->gs);
	printk("      FS: 0x%x\n",r->fs);
	printk("Kernel panic...\n");
	while(1);
	
}

void reserved()
{
  printk("RESERVED EXCEPTION!\n");
  while(1);
}

