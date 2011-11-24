/* 
	Hüseyin Uslu, <shalafiraistlin@gmail.com>

	Console driver
	TODO: add debug levels for printk() function
*/

#include "include/console.h"
#include "../common/include/misc.h"

void init_console(void) /* initialize the console driver */
{
	/* must be called before any printk or screen related function */
	/* TODO: put a check if the init_console is called before any other console related function */
	int i,j;
	
	cls();
	vga_buffer_x_offset=0; // vga buffer's x cordinate
	vga_buffer_y_offset=0; // vga buffer's y cordinate
	
	/* loop through the entire vga buffer array */
	for(i=0;i<80;i++)	
	{
		for(j=0;j<24;j++)
		{
			vga_buffer[i][j]='\0'; /* init the vga_buffer_array */
		}
	}
}

// Write a byte out to the specified port.
void outb(uint16 port, uint8 value)
{
    asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}

uint8 inb(uint16 port)
{
   uint8 ret;
   asm volatile("inb %1, %0" : "=a" (ret) : "dN" (port));
   return ret;
}

uint16 inw(uint16 port)
{
   uint ret;
   asm volatile ("inw %1, %0" : "=a" (ret) : "dN" (port));
   return ret;
} 

void move_cursor()
{
	uint16 loc=y_pos * 80 + x_pos;
	outb(0x3D4, 14);                  // Tell the VGA board we are setting the high cursor byte.
  outb(0x3D5, cursorLocation >> 8); // Send the high cursor byte.
  outb(0x3D4, 15);                  // Tell the VGA board we are setting the low cursor byte.
  outb(0x3D5, cursorLocation);      // Send the low cursor byte.
	
}

void putchar_vga_buffer (int c) /* puts the character to vga buffer */
{
	int i;
	int j;
	
	
	if(vga_buffer_x_offset>=24)	/* if the vga buffer is full */
	{
		/* move i. line to i-1 position (-the first line in screen is so deleted from buffer-) */
		for(i=1;i<24;i++)
		{
			for(j=0;j<80;j++)
			{
				vga_buffer[i-1][j]=vga_buffer[i][j]; 
			}
		}
		
		vga_buffer_x_offset=23;	/* as the vga buffer was full, reposition the x_offset to last line */
		vga_buffer_y_offset=0;
		
		for(j=0;j<80;j++) /* the last line also needs to be cleared */
			vga_buffer[vga_buffer_x_offset][j]='\0';
	}

	vga_buffer[vga_buffer_x_offset][vga_buffer_y_offset]=c; /* insert the given character to vga_buffer */
	
	if ((c == '\n' || c == '\r' || vga_buffer_y_offset>=79)) /* if the line width exceeds 80 characters, we have to continue over the next line */
	{
		vga_buffer_x_offset++; /* increment the line counter */
		vga_buffer_y_offset=0; /* go to the beginning of the line */
	}
	else
	{
		vga_buffer_y_offset++;	/* advance unit position in the line */
	}
	
	
	
}

void putchar_loc(int c,unsigned char x_cor,unsigned char y_cor)
{
	/* puts char to vga buffer at specified location */
	vga_buffer[x_cor][y_cor]=c;
	update_screen();
}
    
void update_screen(void) /* updates screen from the vga_buffer */
{
	int x=0;
	int y=0;
	
	
	cls(); // we first need a fresh screen 
	
	/* loop through the vga buffer */
	for(x=0;x<24;x++)
	{
		for(y=0;y<80;y++)
		{
			putchar(vga_buffer[x][y]); /* put character to screen */
			if(vga_buffer[x][y]=='\n' || vga_buffer[x][y]=='\r' ) 
			{
				/* if we hit a new line character - no need to read rest of the line */
				break;
			}
			
		}
	}
	
	move_cursor();
	
} 

void printk (const char *format, ...) /* main kernel console-printing function */
{
	char **arg = (char **) &format;
	int c;
	char buf[20];

	arg++;

	while ((c = *format++) != 0)
	{
		if (c != '%')
			putchar_vga_buffer (c);
		else/* main kernel console-printing function */
		{
			char *p;

			c = *format++;
			switch (c)
			{
				case 'd':
				case 'u':
				case 'x':
				itoa (buf, c, *((int *) arg++));
				p = buf;
				goto string;
				break;

			case 's':
				p = *arg++;
				if (! p)
				p = "(null)";

				string:
				while (*p)
				putchar_vga_buffer (*p++);
				break;/* main kernel console-printing function */

			default:
				putchar_vga_buffer (*((int *) arg++));
				break;
			}
		}
	}
	
	update_screen();
	
}

/* Clear the screen and initialize VIDEO, XPOS and YPOS. */
      void cls (void)
     {
       int i;
     
       video = (unsigned char *) VIDEO;
     
       for (i = 0; i < COLUMNS * LINES * 2; i+=2)
       {
         video[i] = ' ';
     		 video[i+1] = ATTRIBUTE;
     	 }
     
       xpos = 0;
       ypos = 0;
     }


     
      /* Put the character C on the screen. */
     void
     putchar (int c)
     {
       if (c == '\n' || c == '\r')
         {
         newline:
           xpos = 0;
           ypos++;
           if (ypos >= LINES)
             ypos = 0;
           return;
         }
     
       *(video + (xpos + ypos * COLUMNS) * 2) = c & 0xFF;
       *(video + (xpos + ypos * COLUMNS) * 2 + 1) = ATTRIBUTE;
     
       xpos++;
       if (xpos >= COLUMNS)
         goto newline;
     }
