/*
	$Id: pic.h 73 2007-05-06 14:36:08Z raistlinthewiz $
*/
#ifndef __PIC_H__
#define __PIC_H__

#define PIC1            0x20           /* IO base address for master PIC */
#define PIC2            0xA0           /* IO base address for slave PIC */
#define PIC1_COMMAND    PIC1
#define PIC1_DATA       (PIC1+1)
#define PIC2_COMMAND    PIC2
#define PIC2_DATA       (PIC2+1)
#define PIC_EOI         0x20            /* End - of - interrupt command code */

#define ICW1_ICW4       0x01            /* ICW4 (not) needed */
#define ICW1_SINGLE     0x02            /* Single (cascade) mode */
#define ICW1_INTERVAL4  0x04            /* Call address interval 4 (8) */
#define ICW1_LEVEL      0x08            /* Level triggered (edge) mode */
#define ICW1_INIT       0x10            /* Initialization - required! */

#define ICW4_8086       0x01            /* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO       0x02            /* Auto (normal) EOI */
#define ICW4_BUF_SLAVE  0x08            /* Buffered mode/slave */
#define ICW4_BUF_MASTER 0x0C            /* Buffered mode/master */
#define ICW4_SFNM       0x10            /* Special fully nested (not) */

static __inline__ void outportb(unsigned short port, unsigned char val)
{
  asm volatile("outb %0,%1"::"a"(val),"Nd" (port));
}
  
static __inline__ unsigned char inportb(unsigned short port)
{
  unsigned char ret;
  asm volatile ("inb %1,%0":"=a"(ret):"Nd"(port));
  return ret;
}

static __inline__ unsigned short inportw(unsigned short port)
{
  unsigned short ret;
  asm volatile ("inw %1,%0":"=a"(ret):"Nd"(port));
  return ret;
} 


void remap_pics (int,int);

#endif
