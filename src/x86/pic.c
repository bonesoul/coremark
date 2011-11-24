/*
	Hüseyin Uslu, <shalafiraistlin@gmail.com>
*/

#include "include/pic.h"

/* 
   PIC 8259A 
   Programmable Interrupt Controller
   Each chip has 8 interrupt lines, AT and beyond have 2 chips 
*/

static __inline__ void io_wait(void)
{
  asm volatile("jmp 1f;1:jmp 1f;1:");
}



void remap_pics(int offset1,int offset2)
{
  /* 
    IRQs 0 to 7 are mapped to IDT entries 8 to 15 
    So when IRQ0 fires, IDT's entry 8 will execute the  Double fault handler
    which must not happen.
    So we'll reprogram PIC 8259 to remap IRQs 0-15 to IDT entries 32-47 
  */
    
/*
  arguments:
    offset1 - vector offset for master PIC
      vectors on the master become offset1..offset1+7
    offset2 - same for slave PIC: offset2..offset2+7
 */

        unsigned char   a1, a2;

        a1=inportb(PIC1_DATA);   // save masks
        a2=inportb(PIC2_DATA);

        outportb(PIC1_COMMAND, ICW1_INIT+ICW1_ICW4);  // starts the initialization sequence
        io_wait();
        outportb(PIC2_COMMAND, ICW1_INIT+ICW1_ICW4);
        io_wait();
        outportb(PIC1_DATA, offset1);                    // define the PIC vectors
        io_wait();
        outportb(PIC2_DATA, offset2);
        io_wait();
        outportb(PIC1_DATA, 4);                       // continue initialization sequence
        io_wait();
        outportb(PIC2_DATA, 2);
        io_wait();

        outportb(PIC1_DATA, ICW4_8086);
        io_wait();
        outportb(PIC2_DATA, ICW4_8086);
        io_wait();

        outportb(PIC1_DATA, a1);   // restore saved masks.
        outportb(PIC2_DATA, a2);

}

