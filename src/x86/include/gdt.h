/*
	$Id: gdt.h 17 2006-12-02 22:31:25Z raistlinthewiz $
   
	See src/gdt.c for global descriptor tables explanation
*/

#ifndef __GDT_H__
#define __GDT_H__

struct gdt_entry
{
  unsigned short limit;
  unsigned short base;
  unsigned char base_middle;
  unsigned char access;
  unsigned char granularity;
  unsigned char base_high;
} __attribute__((packed)); /* prevents compiler any "optimization" on structure */

struct gdt_ptr
{
  unsigned short limit;
  unsigned int base;
} __attribute__((packed));

struct gdt_entry gdt[3]; /* allocation for 3 gdt entries */
struct gdt_ptr gp; /* descriptor table's pointer */

extern void gdt_flush();

void set_gdt_entry(int, unsigned long, unsigned long,unsigned char, unsigned char);
void install_gdt();


#endif
