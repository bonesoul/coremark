/* 
	Hüseyin Uslu, <shalafiraistlin@gmail.com>

	hardware IRQ handles
*/

#include "include/irq_handler.h"
#include "../common/include/util.h"
#include "include/pic.h"
#include "../kernel/include/console.h"
#include "../common/include/types.h"
#include "include/timer.h"

uint32 timer_ticks=0; /* probably we will need uint64 later */
uint32 stop_time=0;

void ata0_handler(void)
{
	//printk("ata0_int()\n");
}

void ata1_handler(void)
{
	//printk("ata1_int()\n");
}

void set_timer(void)
{	
	/* 
	   program the PIT - (8253-8254)
           Programmable Interval Timer, has 3 channels
           Channel 0; interval timer
	   Channel 1; system specific channel
           Channel 2; system speaker channel for beeps  
	   TIMER_CLOCK, defined in timer.h, is input clock of PIT
	   TIMER_FREQ, defined in timer.h, frequency of timer ticks		
	*/

	int div=TIMER_CLOCK / TIMER_FREQ; /* calculate divisor */
	outportb(PIT_CHAN_CMD,PIT_CMD);

	/* 
	   program the PIT command channel 
	   PIT_CMD=0x36= 00110110b
           
           [7 6][5 4][3 2 1][0]
           CNTR RW   Mode   BCD            
           00   11   011    0
 
            CNTR=0; counter=channel 0 - timer channel
            RW    ; Read-Write=3 - first LSB then MSB
            Mode=3; Square wave mode
            BCD=0 ; use 16-bit counter, not BCD
             
	*/
	outportb(PIT_CHAN1,div & 0xFF); /* set low byte of divisor */
	outportb(PIT_CHAN1,div >> 8);   /* set high byte of divisor */
	

}

void sleep(uint32 duration)
{
	timer_ticks=0;
	
	while(timer_ticks<duration);
	
}

void timer_handler()   /* timer interrupt */
{
	timer_ticks++; /* increment tick counter */

	//if (timer_ticks % TIMER_FREQ == 0) /* if tick=timer_freq => 1secpassed */
	//{	
	//}
	
 	outportb(PIC1,PIC_EOI); /* handled the interrupt, move on PIC */
}

void start_timer()
{
	timer_ticks=0;
}

void stop_timer()
{
	stop_time=timer_ticks;
}

void keyboard_handler()   /* keyboard */
{

	unsigned char scancode;
	
 	scancode=inportb(0x60);  /* read keyboard port */
 	//read_keyboard(scancode); /* find corresponding ascii character */
  
	outportb(PIC1,PIC_EOI);  /* tell master PIC that we're complete with 
			      last keyboard interrupt. So it can continue
			      to send us new ones */
	
}

void IRQ_NOT_IMPLEMENTED()
{
//	printk("IRQ_NOT_IMPLEMENTED!\n");
}




