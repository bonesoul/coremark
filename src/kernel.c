/* 
	Hüseyin Uslu, <shalafiraistlin@gmail.com>

	After grub boots up the system, and calls the main entrance at boot.S
	boot.S calls the main() in kernel.c as the entrance point

	Contains portions from GNU GRUB Multiboot Specification example code 
	(http://www.gnu.org/software/grub/manual/multiboot/multiboot.html)
    
*/
   
#include "boot/include/multiboot.h" 
#include "kernel/include/console.h" 
#include "x86/include/gdt.h" 
#include "x86/include/idt.h" 
#include "common/include/util.h"
#include "common/include/types.h"
#include "x86/include/timer.h"
#include "x86/include/cpu_info.h"
#include "x86/include/pci.h"

multiboot_info_t *mbi; /* grub multiboot structure */

void main(unsigned long magic_no,unsigned long addr) /* entrance point to kernel */
{                                                    
        /* magic_no and addr parameters supplied to kernel by grub */
  
	init_console(); /* init console */	
	printk("init kernel..\n"); /* kernel banner */

	parse_boot_info(magic_no,addr); /* GRUB provided boot info */
	print_multiboot_memory(); /* print memory amount */
  
    printk("install_gdt()\n");
	install_gdt(); /* global descriptor table */

	cli();
		printk("install_idt()\n");
		install_idt(); /* interrupt descriptor table */
		printk("set_timer()\n");
        set_timer();   /* init. timer */
	sti();
	
	
	printk("Compiled %s %s\n",__DATE__,__TIME__);
	
	
	get_cpuid();
	
	
	scan_pci_devices();
	
	printk("init complete..\n");
	
	
		
	while(1)
	{	
		/* idle loop */
	}
	
}


