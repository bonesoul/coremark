/* 
	$Id: idt.h 17 2006-12-02 22:31:25Z raistlinthewiz $

	idt ( interrupt descriptor table )
*/

#ifndef __IDT_H__
#define __IDT_H__

#define PRESENT 0x80
#define NOT_PRESENT 0x00
#define KERNEL_LEVEL 0x00
#define USER_LEVEL 0x60

struct idt_entry
{
  unsigned short base_low;
  unsigned short segment;
  unsigned char reserved;
  unsigned char flags;
  unsigned short base_high;
} __attribute__((packed));

struct idt_ptr
{
  unsigned short limit;
  unsigned int base;
} __attribute__((packed));

/* table will consist of 256 entries for ISRs */
struct idt_entry idt[256];
struct idt_ptr idtp;

extern void load_idt();



void set_idt_entry(unsigned char,unsigned long,unsigned short,unsigned char);
void install_idt();

/* This defines what the stack looks like after an ISR was running */
struct regs
{
    unsigned int gs, fs, es, ds;      /* pushed the segs last */
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* pushed by 'pusha' */
    unsigned int int_no, err_code;    /* our 'push byte #' and ecodes do this */
    unsigned int eip, cs, eflags, useresp, ss;   /* pushed by the processor automatically */ 
};

#endif

  
