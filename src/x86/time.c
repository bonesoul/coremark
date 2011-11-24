/*
	Hüseyin Uslu, <shalafiraistlin@gmail.com>
*/

#include "include/time.h"
#include "../kernel/include/console.h"
#include "include/pic.h"

int bcd_2_byte(unsigned char val)
{
	return ((val >> 4)*10)+(val & 0x0F);
}

void read_system_time(void)
{
/* read system time from CMOS */
unsigned char sec=0,min=0,hour=0,day=0,month=0,year=0;

__asm__(
"start:\n\t"
"mov $10,%al\n\t"
"out %al,$0x70\n\t"
"in $0x71,%al\n\t"			
"test $0x80,%al\n\t"
"jne start\n\t"
);

outportb(0x70,0x0);
sec=inportb(0x71);

outportb(0x70,0x02);
min=inportb(0x71);

outportb(0x70,0x04);
hour=inportb(0x71);

outportb(0x70,0x07);
day=inportb(0x71);

outportb(0x70,0x08);
month=inportb(0x71);

outportb(0x70,0x09);
year=inportb(0x71);

printk("%x.%x.200%x %x:%x:%x\n",day,month,year,hour,min,sec);
}
