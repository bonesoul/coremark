/*
	$Id: util.h 17 2006-12-02 22:31:25Z raistlinthewiz $
*/
#ifndef __UTIL_H__
#define __UTIL_H__

#define sti() __asm__ __volatile__ ("sti":::"memory")
#define cli() __asm__ __volatile__ ("cli":::"memory")


#endif

