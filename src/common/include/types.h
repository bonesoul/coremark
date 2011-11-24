/*
	$Id: types.h 64 2007-04-27 21:11:54Z raistlinthewiz $
*/
  
/* type definitions for kernel */


#ifndef __TYPES_H__
#define __TYPES_H__

typedef unsigned char      uint8;
typedef unsigned short     uint16;
typedef unsigned int   	   uint32;
typedef unsigned long long uint64;


#ifndef _SIZE_T
#define _SIZE_T
typedef unsigned int size_t;
#endif

typedef unsigned short ushort_t;
typedef unsigned long ulong_t;
typedef unsigned char byte;
typedef enum {false, true} bool;

#define FALSE (unsigned int)0
#define TRUE  (unsigned int)1
#define NULL ((void *)0)

#endif
