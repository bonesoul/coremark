/*
	Hüseyin Uslu, <shalafiraistlin@gmail.com>
*/


#include "include/command.h"
#include "include/types.h"
#include "include/console.h"
#include "include/version.h"
#include "include/cpu_info.h"
#include "include/smbios.h"
#include "include/time.h"
#include "include/multiboot.h"
#include "include/memtest.h"
#include "include/ata.h"

extern uint32 timer_ticks;

/* internal functions */
unsigned char compare_command(const unsigned char *,const unsigned char*);

/* command handlers */
void cmd_mem_info();
void cmd_boot_info();
void cmd_banner();
void cmd_time();
void cmd_cpuinfo();
void cmd_smbios();
void cmd_cls();
void cmd_help();
void cmd_memtest();
void cmd_iotest();

/* keyboard interrupt calls this function for parsing commands */
void parse_cmd(const unsigned char* cmd)
{
  if(compare_command(cmd,(unsigned char *)"meminfo"))
    cmd_mem_info();
  else if (compare_command(cmd,(unsigned char *)"bootinfo"))
    cmd_boot_info();
  else if (compare_command(cmd,(unsigned char *)"banner"))
    cmd_banner();
  else if (compare_command(cmd,(unsigned char *)"time"))
    cmd_time();
  else if (compare_command(cmd,(unsigned char *)"cpuinfo"))
    cmd_cpuinfo();
  else if (compare_command(cmd,(unsigned char *)"smbios"))
    cmd_smbios();
  else if (compare_command(cmd,(unsigned char *)"help"))
    cmd_help();
  else if (compare_command(cmd,(unsigned char *)"memtest"))
    cmd_memtest();
  else if (compare_command(cmd,(unsigned char *)"iotest"))
    cmd_iotest();
  else
    {
      printk("command not found: %s",(char *)cmd);
    }

}

/* compares given commands, returns 1 if equal, 0 if not */
unsigned char compare_command(const unsigned char * cmd,const unsigned char * cmd2)
{
  /* 
     cmd is supplied command (consists \0 at the end) 
     cmd2 is supplied comparision command 
  */

  unsigned char cmd_len,cmd2_len,i;

  cmd_len=strlen((char *)cmd)-1; /* decrement 1 for \0 */
  cmd2_len=strlen((char *)cmd2);
  
  if (cmd_len!=cmd2_len)
    return 0; /* comparision failed */
  else
  {
    for (i=0;i<cmd2_len;i++)
      {
	if(cmd[i]!=cmd2[i])
	  return 0; /* comparision failed */
      }
  } 

  return 1; /* commands matches */

}

/* command handlers */

void cmd_mem_info()
{
  print_multiboot_memory();
  print_multiboot_mmap();
}

void cmd_boot_info()
{
  print_multiboot_boot_info();
}

void cmd_banner()
{
  /* printk banner */
  printk("%s\n",VERSION);
  printk("%s\n",AUTHORS);
}

void cmd_time()
{
  read_system_time();
  printk("Timer ticks: %dmsec\n",timer_ticks);
}

void cmd_cpuinfo()
{
  /* print CPUID info */
  get_cpuid(); /* read cpu info */
}

void cmd_smbios()
{
  read_smbios();
}

void cmd_help()
{
  printk("Avaible commands: banner, meminfo, bootinfo, cpuinfo, time, smbios, memtest, iotest\n");
}

void cmd_memtest()
{
  memtest();  
}

void cmd_iotest()
{
 /*test_io_devices();*/
}
