/*
	$Id: keyboard.h 65 2007-04-27 22:53:17Z raistlinthewiz $
*/
#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

unsigned char input_buffer[100]; /* used for keyboard input buffer */
unsigned char input_offset;


void init_keyboard();
void read_keyboard(unsigned char);
#endif

