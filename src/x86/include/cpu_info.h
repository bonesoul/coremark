#ifndef __CPU_INFO_H__
#define __CPU_INFO_H__



void get_cpuid(void);
char * get_vendor(void); /* defined in cpuid.asm */
char * get_brand_string(void);  /* defined in cpuid.asm */

#endif
