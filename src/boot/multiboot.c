/*
	Hüseyin Uslu, <shalafiraistlin@gmail.com>
	
	Grub Multiboot parser

*/

#include "include/multiboot.h"
#include "../kernel/include/console.h"

/* internal macro */
#define CHECK_FLAG(flags,bit)   ((flags) & (1 << (bit))) /* Check if the bit BIT in FLAGS is set. */

/* global kernel multiboot structure variable */
extern multiboot_info_t *mbi;

/* grub multiboot parser */
void parse_boot_info(unsigned long magic_number, unsigned long addr)
{
	/*
		For more information about the Multiboot Specification see 
		http://www.gnu.org/software/grub/manual/multiboot/multiboot.html
		and also check multiboot.h	
	
		TODO: Flags are not yet fully parsed
        
		multiboot header:

		offset 	type	field		note
		0	u32	magicno		required
		4	u32	flags		required
		8	u32	checksum	required
		12	u32	header_addr	if flag[16] is set 
		16	u32	load_addr	if flag[16] is set 
		20	u32	load_end_addr	if flag[16] is set cls();
		24	u32	bss_end_addr	if flag[16]v set 
		28	u32	entry_addr	if flag[16] is set 
		32	u32	mode_type	if flag[2] is set 
		36	u32	width		if flag[2] is set 
		40	u32	height		if flag[2] is set 
		44	u32	depth		if flag[2] is set 
	*/

	printk("[GRUB] ");
	if(magic_number!=MULTIBOOT_BOOTLOADER_MAGIC)
	  printk(" Expected magic number do not match!\n");
	else
	  printk(" 0x%x..\n",magic_number);

	mbi=(multiboot_info_t *)addr;
	//printk("FLAGS = 0x%x\n",(unsigned)mbi->flags);

}

void print_multiboot_memory(void)
{

  /* do mem_lower and mem_upper info supplied? */
  if (CHECK_FLAG (mbi->flags, 0)) {
    printk ("[GRUB] Total Memory = %uKB..\n",(unsigned)mbi->mem_lower + (unsigned)mbi->mem_upper);
  }

}

void print_multiboot_mmap(void)
{
  /* Are mmap_* valid? */

  if (CHECK_FLAG (mbi->flags, 6))
    {
      memory_map_t *mmap;
      
      printk("[ memory map ]\n");
                  
      for (mmap = (memory_map_t *) mbi->mmap_addr;
	   (unsigned long) mmap < mbi->mmap_addr + mbi->mmap_length;
	   mmap = (memory_map_t *) ((unsigned long) mmap
				    + mmap->size + sizeof (mmap->size)))
	printk ("* base_addr = 0x%x%x,"
		" length = 0x%x%x, type = 0x%x\n",
		(unsigned) mmap->base_addr_high,
		(unsigned) mmap->base_addr_low,
		(unsigned) mmap->length_high,
		(unsigned) mmap->length_low,
		(unsigned) mmap->type); 
    }
  // size = 0x%x,                      //(unsigned) mmap->size,
}

void print_multiboot_boot_info(void)
{
  /* boot_device info avaible?*/
    if (CHECK_FLAG (mbi->flags, 1)) 
      {
	//printk ("boot device = 0x%x\n", (unsigned) mbi->boot_device);
	switch (((mbi->boot_device) & 0xFF000000))
	  {
	  case	0x00: 	printk("Boot Device: floppy\n");
	    break;
	  case 	0x80: 	printk("Boot Device: disc\n");
	    break;
	    /* partition info */
	    //	printk("part1= 0x%x\n",(unsigned)((mbi->boot_device) & 0x00FF0000));
	    //	printk("part2= 0x%x\n",(unsigned)((mbi->boot_device) & 0x0000FF00));
	    //	printk("part3= 0x%x\n",(unsigned)((mbi->boot_device) & 0x000000FF));
	  default:	printk("Boot Device: unknown boot device\n");
	  }	
	
      }
    
     	/* Command line passed? */
    if (CHECK_FLAG (mbi->flags, 2))
      printk ("Grub command line = %s\n", (char *) mbi->cmdline);
    else
      printk("Grub command line, empty\n");
    
    
    
    /* Are mods_* valid? */
    if (CHECK_FLAG (mbi->flags, 3))
      {
      	module_t *mod;
	int i;
	
	printk ("mods_count = %d, mods_addr = 0x%x\n",
            	(int) mbi->mods_count, (int) mbi->mods_addr);
	for (i = 0, mod = (module_t *) mbi->mods_addr;
	     i < mbi->mods_count;
	            i++, mod += sizeof (module_t))
	  printk ("mod_start = 0x%x, mod_end = 0x%x, string = %s\n",(unsigned) mod->mod_start,(unsigned) mod->mod_end,(char *) mod->string);
      
      }

    /* FIXME: this is not an information message but an error condition! */
    /* Bits 4 and 5 are mutually exclusive! */
    if (CHECK_FLAG (mbi->flags, 4) && CHECK_FLAG (mbi->flags, 5))
      {
           printk ("Error:Both bits 4 and 5 are set.\n");
           return;
      }


    /* Is the symbol table of a.out valid? */
    if (CHECK_FLAG (mbi->flags, 4))
      {
	aout_symbol_table_t *aout_sym = &(mbi->u.aout_sym);
	
           printk ("aout_symbol_table: tabsize = 0x%0x, "
                   "strsize = 0x%x, addr = 0x%x\n",
                   (unsigned) aout_sym->tabsize,
                   (unsigned) aout_sym->strsize,
                   (unsigned) aout_sym->addr);
      }
    
    /* Is the section header table of ELF valid? */
    if (CHECK_FLAG (mbi->flags, 5))
      {
	elf_section_header_table_t *elf_sec = &(mbi->u.elf_sec);
	
	printk ("elf_sec: num = %u, size = 0x%x,"
		" addr = 0x%x, shndx = 0x%x\n",
		(unsigned) elf_sec->num, (unsigned) elf_sec->size,
		(unsigned) elf_sec->addr, (unsigned) elf_sec->shndx);
      }

    
	if (CHECK_FLAG (mbi->flags,7))
	  printk("Drive info avaible..\n");

	if (CHECK_FLAG (mbi->flags,8))
	  printk("romconfig avible..\n");

	if (CHECK_FLAG (mbi->flags,9))
	  printk("bootloader info avaible..\n");

}



