/*
	Hüseyin Uslu, <shalafiraistlin@gmail.com>
*/

#include "include/memtest.h"
#include "include/types.h"
#include "include/irq_handler.h"
#include "include/console.h"
#include "include/timer.h"

/* based on concepts of: http://www.cs.utexas.edu/users/djimenez/utsa/cs3343/lecture15.html */

/* 
	TODO: 
		* add stride-based memory read-tests
		* add L1, L2 increased strides
		* implement stride argument to memtest_read function
*/

uint32 memtest_write(register uint32 test_block_size)
{
	/* define the memory range */
	uint32 start_offset=0x01000000; /* start from a safe spot */
	uint32 end_offset=start_offset + test_block_size;

	uint32 elapsed_time;

	register uint8 * data;
	register int i;

	data=(uint8 *)start_offset;
	i=0;
	
	start_timer();
	while (i < test_block_size)
	{
		data[i]=0;
		i++;
	}
	stop_timer();

	elapsed_time=(stop_time * 1000000) / test_block_size; /* 1milisec = 1000000 nanosec */
	return elapsed_time;
}

uint32 memtest_read(register uint32 test_block_size)
{
	/* define the memory range */
	uint32 start_offset=0x000FFFFF;
	uint32 end_offset=start_offset + test_block_size;

	uint32 elapsed_time;

	register uint8 * data;
	register uint8 tmp;
	register int i;

	data=(uint8 *)start_offset;
	i=0;
	
	start_timer();
	while (i < test_block_size)
	{
		tmp=data[i];
		i++;
	}
	stop_timer();

	elapsed_time=(stop_time * 1000000) / test_block_size; /* 1milisec = 1000000 nanosec */
	return elapsed_time;
}

void memtest(void)
{	
	uint32 size;
	uint32 read_interval=0;
	uint32 write_interval=0;
	int i=0;

	size=(1<<24)/100;
	
	printk("Starting SINGLE MEMORY CELL READ test;  78 probes - using %d KB data\n",size/1024);
	printk("[");
	for(i=0;i<78;i++)
	{
		read_interval+=memtest_read(size);
		printk(".");
	}
	printk("]\n");

	printk("Starting SINGLE MEMORY CELL WRITE test; 78 probes - using %d KB data\n",size/1024);
	printk("[");
	for(i=0;i<78;i++)
	{
		write_interval+=memtest_write(size);
		printk(".");
	}
	printk("]\n");

	printk("Read  single memory cell: %d nanoseconds\n",read_interval/78);
	printk("Write single memory cell: %d nanoseconds\n",write_interval/78);
}

