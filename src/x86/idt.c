/* 
	Hüseyin Uslu, <shalafiraistlin@gmail.com>
	
	contains interrupt descriptor table, interrup & exception handling functions.
	IDT is similar to gdt, check gdt.c
*/ 

#include "include/idt.h"
#include "include/irq_handler.h"
#include "../common/include/util.h"
#include "include/pic.h"



/* interrupt service request - exception handlers in ISR.asm */
extern void isr0();  // Division By Zero Exception
extern void isr1();  // Debug Exception
extern void isr2();  // Non Maskable Interrupt Exception
extern void isr3();  // Breakpoint Exception
extern void isr4();  // Into Detected Overflow Exception
extern void isr5();  // Out of Bounds Exception
extern void isr6();  // Invalid Opcode Exception
extern void isr7();  // No Coprocessor Exception
extern void isr8();  // Double Fault Exception
extern void isr9();  // Coprocessor Segment Overrun Exception
extern void isr10(); // Bad TSS Exception
extern void isr11(); // Segment Not Present Exception
extern void isr12(); // Stack Fault Exception
extern void isr13(); // General Protection Fault Exception
extern void isr14(); // Page Fault Exception
extern void isr15(); // Unknown Interrupt Exception
extern void isr16(); // Coprocessor Fault Exception
extern void isr17(); // Aligment Check Exception (486+)
extern void isr18(); // Machine Check Exception (Pentium/586+)
extern void isr19(); // Reserved Exception
extern void isr20(); // Reserved Exception
extern void isr21(); // Reserved Exception
extern void isr22(); // Reserved Exception
extern void isr23(); // Reserved Exception
extern void isr24(); // Reserved Exception
extern void isr25(); // Reserved Exception
extern void isr26(); // Reserved Exception
extern void isr27(); // Reserved Exception
extern void isr28(); // Reserved Exception
extern void isr29(); // Reserved Exception
extern void isr30(); // Reserved Exception
extern void isr31(); // Reserved Exception

extern void IRQ0();
extern void IRQ1();
extern void IRQ14();
extern void IRQ15();

void set_idt_entry(unsigned char num, unsigned long base, unsigned short seg, unsigned char flags)
{
  idt[num].base_low=(base & 0xFFFF);
  idt[num].base_high=(base >> 16) & 0xFFFF;
  
  idt[num].segment=seg;
  idt[num].reserved=0;
  idt[num].flags=flags;
}


void install_idt()
{
  idtp.limit=(sizeof(struct idt_entry)* 47)-1; /* 255 ISRs */
  idtp.base=(unsigned int)&idt;

  
  remap_pics(0x20,0x28); /* remap interrups to;
			    master pic interrups -> 0x20 (32)
			    slave pic interrupts -> 0x28 (40)
			 */

  /* assing handlers to interrups & exceptions */
  set_idt_entry(0 ,(unsigned long)&isr0,0x08 , 0xF | PRESENT | KERNEL_LEVEL);
  set_idt_entry(1 ,(unsigned long)&isr1,0x08 ,  0xF | PRESENT | KERNEL_LEVEL);
  set_idt_entry(2 ,(unsigned long)&isr2,0x08 ,  0xF | PRESENT | KERNEL_LEVEL);
  set_idt_entry(3 ,(unsigned long)&isr3,0x08 ,  0xF | PRESENT | KERNEL_LEVEL);
  set_idt_entry(4 ,(unsigned long)&isr4,0x08 ,  0xF | PRESENT | KERNEL_LEVEL);
  set_idt_entry(5 ,(unsigned long)&isr5,0x08 ,  0xF | PRESENT | KERNEL_LEVEL);
  set_idt_entry(6 ,(unsigned long)&isr6,0x08 ,  0xF | PRESENT | KERNEL_LEVEL);
  set_idt_entry(7 ,(unsigned long)&isr7,0x08 ,  0xF | PRESENT | KERNEL_LEVEL);
  set_idt_entry(8 ,(unsigned long)&isr8,0x08 ,  0xF | PRESENT | KERNEL_LEVEL);
  set_idt_entry(9 ,(unsigned long)&isr9,0x08 ,  0xF | PRESENT | KERNEL_LEVEL);
  set_idt_entry(10,(unsigned long)&isr10,0x08,  0xF | PRESENT | KERNEL_LEVEL);
  set_idt_entry(11,(unsigned long)&isr11,0x08,  0xF | PRESENT | KERNEL_LEVEL);
  set_idt_entry(12,(unsigned long)&isr12,0x08,  0xF | PRESENT | KERNEL_LEVEL);
  set_idt_entry(13,(unsigned long)&isr13,0x08,  0xF | PRESENT | KERNEL_LEVEL);
  set_idt_entry(14,(unsigned long)&isr14,0x08,  0xF | PRESENT | KERNEL_LEVEL);
  set_idt_entry(15,(unsigned long)&isr15,0x08,  0xF | PRESENT | KERNEL_LEVEL);
  set_idt_entry(16,(unsigned long)&isr16,0x08,  0xF | PRESENT | KERNEL_LEVEL);
  set_idt_entry(17,(unsigned long)&isr17,0x08,  0xF | PRESENT | KERNEL_LEVEL);
  set_idt_entry(18,(unsigned long)&isr18,0x08,  0xF | PRESENT | KERNEL_LEVEL);
  set_idt_entry(19,(unsigned long)&isr19,0x08,  0xF | PRESENT | KERNEL_LEVEL);
  set_idt_entry(20,(unsigned long)&isr20,0x08,  0xF | PRESENT | KERNEL_LEVEL);
  set_idt_entry(21,(unsigned long)&isr21,0x08,  0xF | PRESENT | KERNEL_LEVEL);
  set_idt_entry(22,(unsigned long)&isr22,0x08,  0xF | PRESENT | KERNEL_LEVEL);
  set_idt_entry(23,(unsigned long)&isr23,0x08,  0xF | PRESENT | KERNEL_LEVEL);
  set_idt_entry(24,(unsigned long)&isr24,0x08,  0xF | PRESENT | KERNEL_LEVEL);
  set_idt_entry(25,(unsigned long)&isr25,0x08,  0xF | PRESENT | KERNEL_LEVEL);
  set_idt_entry(26,(unsigned long)&isr26,0x08,  0xF | PRESENT | KERNEL_LEVEL);
  set_idt_entry(27,(unsigned long)&isr27,0x08,  0xF | PRESENT | KERNEL_LEVEL);
  set_idt_entry(28,(unsigned long)&isr28,0x08,  0xF | PRESENT | KERNEL_LEVEL);
  set_idt_entry(29,(unsigned long)&isr29,0x08,  0xF | PRESENT | KERNEL_LEVEL);
  set_idt_entry(30,(unsigned long)&isr30,0x08,  0xF | PRESENT | KERNEL_LEVEL);
  set_idt_entry(31,(unsigned long)&isr31,0x08,  0xF | PRESENT | KERNEL_LEVEL);
  
  
  /* hardware IRQ handlers */
  set_idt_entry(32,(unsigned long)&IRQ0,0x08, 0xF | PRESENT | KERNEL_LEVEL);  // timer IRQ
  set_idt_entry(33,(unsigned long)&IRQ1,0x08, 0xF | PRESENT | KERNEL_LEVEL);   // keyboard IRQ
  set_idt_entry(34,(unsigned long)&IRQ_NOT_IMPLEMENTED,0x08, 0xF | NOT_PRESENT | KERNEL_LEVEL);
  set_idt_entry(35,(unsigned long)&IRQ_NOT_IMPLEMENTED,0x08, 0xF | NOT_PRESENT | KERNEL_LEVEL);
  set_idt_entry(36,(unsigned long)&IRQ_NOT_IMPLEMENTED,0x08, 0xF | NOT_PRESENT | KERNEL_LEVEL);
  set_idt_entry(37,(unsigned long)&IRQ_NOT_IMPLEMENTED,0x08, 0xF | NOT_PRESENT | KERNEL_LEVEL);
  set_idt_entry(38,(unsigned long)&IRQ_NOT_IMPLEMENTED,0x08, 0xF | NOT_PRESENT | KERNEL_LEVEL);
  set_idt_entry(39,(unsigned long)&IRQ_NOT_IMPLEMENTED,0x08, 0xF | NOT_PRESENT | KERNEL_LEVEL);
  set_idt_entry(40,(unsigned long)&IRQ_NOT_IMPLEMENTED,0x08, 0xF | NOT_PRESENT | KERNEL_LEVEL);
  set_idt_entry(41,(unsigned long)&IRQ_NOT_IMPLEMENTED,0x08, 0xF | NOT_PRESENT | KERNEL_LEVEL);
  set_idt_entry(42,(unsigned long)&IRQ_NOT_IMPLEMENTED,0x08, 0xF | NOT_PRESENT | KERNEL_LEVEL);
  set_idt_entry(43,(unsigned long)&IRQ_NOT_IMPLEMENTED,0x08, 0xF | NOT_PRESENT | KERNEL_LEVEL);
  set_idt_entry(44,(unsigned long)&IRQ_NOT_IMPLEMENTED,0x08, 0xF | NOT_PRESENT | KERNEL_LEVEL);
  set_idt_entry(45,(unsigned long)&IRQ_NOT_IMPLEMENTED,0x08, 0xF | NOT_PRESENT | KERNEL_LEVEL);
  set_idt_entry(46,(unsigned long)&IRQ14,0x08, 0xF | PRESENT | KERNEL_LEVEL); // ata-0 handler
  set_idt_entry(47,(unsigned long)&IRQ15,0x08, 0xF | PRESENT | KERNEL_LEVEL); // ata-1 handler

  load_idt();
}









