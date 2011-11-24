/*
	
	Hüseyin Uslu, <shalafiraistlin@gmail.com>

	global descriptor table
	gdt (global descriptor table) is protection mecanism for 386 based systems.
	With mechanism kernel defines priviles for memory access.  
  
	GDT prevents processes from accessing memory locations which is not supposed 
	to do so grub already installs a gdt, overwriting the grubs loaded memory will also
	trash the GDT, so it's best setup a new gdt

	references:
	http://www.osdever.net/bkerndev/index.php?the_id=90
	http://www.mega-tokyo.com/osfaq/BabyStep
	http://www.mega-tokyo.com/osfaq/GdtForDummies
  
*/

#include "include/gdt.h"

void set_gdt_entry(int num, unsigned long base,unsigned long limit, unsigned char access, unsigned char gran)
{
  gdt[num].base=(base & 0xFFFF); /* lowest 2 bytes of base */
  gdt[num].base_middle=(base >> 16) & 0xFF; /* third third byte */
  gdt[num].base_high=(base >> 24 ) & 0xFF; /* highest byte */
  
  gdt[num].limit=(limit & 0xFFFF); /* lowest 2 bytes of limit */
  
  gdt[num].granularity=((limit >> 16) & 0x0F); /* 1 byte */
  gdt[num].granularity|=(gran & 0xF0); /* 1 byte */
  gdt[num].access=access;

}

void install_gdt()
{
 
  gp.base=(unsigned int)&gdt; /* set base address to our descriptor table */
  gp.limit=(sizeof(struct gdt_entry) * 3)-1; /* (size of our gdt table)-1  */
  
  /* NULL descriptor table - CPU needs it */
  set_gdt_entry(0,0,0,0,0);
  
  /* Code Segment */
  set_gdt_entry(1,0x0,0xFFFFF,0x9A,0xC0);
  /*
    cs:0 will be located at 4.MB of memory
    access; 0x9A (1 - 00 - 1 - 1010b)
                  |   |    |   |
                  P   DPL  S   TYPE
                
    P: 1 ( segment present )
    DPL: 0 (Ring 0)
    S: 1 (For CS/DS/SS segments it has to be 1)
    TYPE: 1010
          bit3: (1) if S=1 and bit3=1 then its Code Segment
                    else S=1 and bit3=0 then Data Segment
          bit2: (0) (TODO: write description)
          bit1:	(1) For Code Segment 0 means you can't read segment,
                    1 means you can
          bit0; (0) The segment has been accessed(1) or not(0)                                                        
  
  granularity: 0xC0 (1 - 1 - 0 - 0 - 0000)
                     |   |   |   |   |
                     G  D/B  0  AVL  Size
  
          G:1   segment size specified in bytes(0) or in 4K pages(1)
          D/B:1 For Code Segment, this field states default address/operand size
                as 32bit(1) or 16bit(0)    
          0: 	Reserved bit always 0
          AVL:  Avaible bit for programmers own use
          Size: (TODO: write description)      
  */
  
  
  /* Data Segment */
 
  set_gdt_entry(2,0x0,0xFFFFF,0x92,0xC0);
  /* 
    ds:0 will be in 8.MB of memory
    access: 0x92 (10010010)
    P: 1 present segment
    DPL: privilege level ring0
    S: 1 for CS/DS/SS
    Type:
      bit3 (0): if S=1 bit3=0 then Data Segment
      bit2 (0): (TODO)
      bit1 (1): For Data Segment, 0 means read-only, 1 means r/w
      bit0 (0): Segment has been already accessed?
    
   granularity: same as above CS's granularity */

  gdt_flush(); 
}

  
  
