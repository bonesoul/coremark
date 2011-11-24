/*
	Hüseyin Uslu, <shalafiraistlin@gmail.com>
*/

#include "include/keyboard.h"
#include "include/console.h"
#include "include/command.h"
#include "include/pic.h"



/* US keyboard mapping */
unsigned char kbdus[128] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
  '9', '0', '-', '=', '\b',	/* Backspace */
  '\t',			/* Tab */
  'q', 'w', 'e', 'r',	/* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',		/* Enter key */
    0,			/* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
 '\'', '`',   0,		/* Left shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
  'm', ',', '.', '/',   0,					/* Right shift */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};


void init_keyboard()
{
	input_offset=0;
}

void clear_buffer();

void read_keyboard(unsigned char scancode)
{
	unsigned char chr;
	if(!(scancode & 0x80))
	{


		chr=kbdus[scancode];
	        
		/* check special characters */
		if (chr=='\b') /* backspace */
		{
		  input_offset--; 
		  input_buffer[input_offset]='\0';
		  return;
		}

		printk("%c", chr);
		
		input_buffer[input_offset]=chr;
		input_offset++;
		
		if (chr=='\n') /* enter */
		{
			/* console command needs to be parsed */
		        parse_cmd((unsigned char *)&input_buffer); /* parse the command */
			printk(">"); 
			clear_buffer(); /* clear the keyboard buffer */			
		}
	       
		
		
	}
	else
	{
		/* a key just has been released */
	}
		
}

void clear_buffer()
{
	int i;
	for(i=0;i<100;i++)
	  input_buffer[i]='\0';
	input_offset=0;
	
}
