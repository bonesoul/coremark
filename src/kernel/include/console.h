/*
	$Id: console.h 65 2007-04-27 22:53:17Z raistlinthewiz $
*/     

#ifndef __CONSOLE_H__
#define __CONSOLE_H__

/* TODO: merge below definitions with newly written console definitions */

     /* Some screen stuff. */
     /* The number of columns. */
     #define COLUMNS                 80
     /* The number of lines. */
     #define LINES                   24
     /* The attribute of an character. */
     #define ATTRIBUTE               0x1F
     /* The video memory address. */
     #define VIDEO                   0xB8000
     
     /* Variables. */
     /* Save the X position. */
     int xpos;
     /* Save the Y position. */
     int ypos;
     /* Point to the video memory. */
     volatile unsigned char *video;
     
     
char vga_buffer[24][80]; /* vga_buffer - 2 dimensional vga buffer array for holding 24 x 80 text scren */
int vga_buffer_x_offset; /* vga_buffers vertical offset */
int vga_buffer_y_offset; /* vga_buffers horizantal offset */

void printk (const char *format, ...); /* main kernel console-printing function */
void putchar_vga_buffer (int c); /* puts the character to vga buffer */
void init_console(void); /* initialize the console driver */
void update_screen(void); /* updates screen from the vga_buffer */
void cls(void);
void putchar (int c);
void putchar_loc(int c,unsigned char x_cor,unsigned char y_cor);

     
#endif

