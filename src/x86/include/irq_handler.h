/*
	$Id: irq_handler.h 73 2007-05-06 14:36:08Z raistlinthewiz $
*/
#ifndef __IRQ_H__
#define __IRQ_H__

//void IRQ0();   /* timer interrupt */
//void IRQ1();   /* keyboard */
//void IRQ2();   /* cascade interrupt (see doc/remapping_pics.txt) */
//void IRQ3();   /* COM2 */
//void IRQ4();   /* COM1 */
//void IRQ5();   /* Sound Card */
//void IRQ6();   /* Floppy Disk controller */
//void IRQ7();   /* Paralel port */
//void IRQ8();   /* Real time clock */
//void IRQ9();   /* Free interrupt */
//void IRQ10();  /* Free interrupt */
//void IRQ11();  /* Free interrupt */
//void IRQ12();  /* PS/2 Mouse */
//void IRQ13();  /* FPU coprocessor */
//void IRQ14();  /* Primary IDE channel */
//void IRQ15();  /* Secondary IDE channel */


#include "../../common/include/types.h"

void keyboard_handler();
void timer_handler();
void ata0_handler();
void ata1_handler();

void IRQ_NOT_IMPLEMENTED(); /* not yet implemented IRQ handlers */

void sleep(uint32);

#endif
