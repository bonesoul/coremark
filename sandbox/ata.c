/*
	Hüseyin Uslu, <shalafiraistlin@gmail.com>
*/

#include "include/ata.h"
#include "include/console.h"
#include "include/pic.h"
#include "include/types.h"
#include "include/timer.h"
#include "include/types.h"

void identify_ata_device(uint8, uint8);
int find_device_type(uint8,uint8);

ata_device devices[4];
char device_exists[4];


void check_ata_devices(void)
{
	unsigned char val;
	int ctrl;
	int ctrl_base;
	int drive;
	int drive_no;
	int i;

	for(i=0;i<4;i++)
		device_exists[i]=FALSE;

	for(ctrl=0;ctrl<2;ctrl++)
	{
		if(ctrl==0) ctrl_base=PRIMARY_BASE; /* primary ide controller */
		else ctrl_base=SECONDARY_BASE;
		
		/* check ide controller if exists */
		outportb(ctrl_base + LBA_LOW,MAGIC_NO); /* magic no to low lba port */
		val=inportb(ctrl_base + LBA_LOW); /* re-read it */
		if(val==MAGIC_NO) /* if equal, controller exists */
		{
			/* now check drivers by a drive-select command */
			/* first device=0xA0, second=0xB0 */
			for(drive=0;drive<2;drive++)
			{
				if(drive==0) drive_no=0xA0;
				else drive_no=0xB0;
				outportb(ctrl_base + DRIVE_SELECT,drive_no); /* issue drive select */
				val=inportb(ctrl_base + STATUS); 
				if(val & 0x40) /* if busy bit is set, device exists */
				{
					switch(find_device_type(ctrl,drive))
					{
						case ATADEV_NODEVICE:
							break;
						case ATADEV_UNKNOWN:
							printk("found unknown device on controller-%d channel-%d\n",ctrl,drive);
							break;
						case ATADEV_PATA:
							printk("found ATA device on controller-%d channel-%d\n",ctrl,drive);
							identify_ata_device(ctrl,drive);
							break;
						case ATADEV_PATAPI:
							printk("found ATAPI device on controller-%d channel-%d\n",ctrl,drive);
							break;
						case ATADEV_SATA:
							printk("found SATA device on controller-%d channel-%d\n",ctrl,drive);
							break;
						case ATADEV_SATAPI:
							printk("found SATAPI device on controller-%d channel-%d\n",ctrl,drive);
							break;				
					}
				}
			}
		}
	}
}

int find_device_type(uint8 controller,uint8 device)
{
int command_base;
uint8 sector_count;
uint8 sector_number;
uint8 status;
uint8 cyl_low;
uint8 cyl_high;

if (controller==0)
	command_base=PRIMARY_BASE;
else if (controller==1)
	command_base=SECONDARY_BASE;

sector_count=inportb(command_base + SECTOR_COUNT);
sector_number=inportb(command_base + SECTOR_NUMBER);

if(sector_count==1) /* !also sector_number==1 */
{
	status=inportb(command_base + STATUS);
	cyl_low=inportb(command_base + CYLINDER_LOW);
	cyl_high=inportb(command_base + CYLINDER_HIGH);
	
	/* differentiate device */
	if(cyl_low==0x14 && cyl_high==0xEB) return ATADEV_PATAPI;
	else if(cyl_low==0x69 && cyl_high==0x96) return ATADEV_SATAPI;
	else if(cyl_low==0 && cyl_high==0) return ATADEV_PATA;
	else if(cyl_low==0x3c &&  cyl_high==0xc3) return ATADEV_SATA;
	else return ATADEV_UNKNOWN;
}
else
return ATADEV_NODEVICE;

}

void identify_ata_device(uint8 controller, uint8 device)
{
/* 
   controller; 0, ata0
			   1, ata1
   device;     0, channel-0 device,
			   1, channel-1 device 
*/

int controller_base;
int command_base;
int i;
uint16 * buffer;


if(controller==0)
{
	command_base=PRIMARY_BASE;
	controller_base=CB_PRIMARY_BASE;
}
else if(controller==1)
{
	command_base=SECONDARY_BASE;
	controller_base=CB_SECONDARY_BASE;
}
while( inportb(controller_base + ALTERNATIVE_STATUS) & BUSY_BIT);/* wait drive to be ready */
outportb(command_base + COMMAND,CMD_IDENTIFY_DRIVE); /* send identify command to command port */
while(inportb(controller_base + ALTERNATIVE_STATUS) & DRQ_BIT !=0 ); /* wait data request bit */
inportb(command_base + COMMAND); /* harddisk interrupt will fire, clear it */

buffer=(uint16 *)&devices[controller * 2 + device];

for(i=0;i<128;i++)
	buffer[i]=inportw(command_base + DATA_PORT);

device_exists[controller * 2 + device]=TRUE;
}

uint32 test_io_device(uint8 drive,uint16 size)
{
	int command_base=PRIMARY_BASE;
	/* lba-24 bit mode */
	uint32 addr;
	uint16 i;
	uint8 buffer[512];
	uint16 tmp;
	uint32 elapsed_time;

	start_timer();

	outportb(command_base + 0x1,0x00);
	outportb(command_base + SECTOR_COUNT, 0x01);
	outportb(command_base + SECTOR_NUMBER, (unsigned char)addr);
	outportb(command_base + 0x4,(unsigned char)(addr >> 8));
	outportb(command_base + 0x5,(unsigned char)(addr >> 16));
	outportb(command_base + DRIVE_HEAD, 0xED | (drive << 4) | ((addr>>24) & 0x0f));
	outportb(command_base + COMMAND, 0x20);
	
	while(!(inportb(command_base + STATUS) & 0x08))

		for(i=0;i<size;i++)
		{
			tmp=inportw(command_base);
			buffer[i*2]=(unsigned char)tmp;
			buffer[i*2+1]=(unsigned char)(tmp>>8);
		}

	stop_timer();
	elapsed_time=stop_time;
	return elapsed_time;
}

void test_io_devices()
{
	int j;

	uint32 disk_size;
	uint16 block_size;

	uint8 device=0;

	if(device_exists[device]) /* if device exists */
	{
		printk("Device: (ata0,0) ");
		for(j=0;j<20;j+=2)	
			printk("%c%c",(unsigned char)devices[device].model[j+1],(unsigned char)devices[device].model[j]);
		printk("\n");

		printk("Serial: ");
		for(j=0;j<10;j+=2)	
			printk("%c%c",(unsigned char)devices[device].serial_number[j+1],(unsigned char)devices[device].serial_number[j]);

		printk(" Firmware: ");
		for(j=0;j<4;j+=2)	
			printk("%c%c",(unsigned char)devices[device].firmware_revision[j+1],(unsigned char)devices[device].firmware_revision[j]);
		printk("\n");

		printk("%d cylinders, %d heads, %d sectors per track\n",devices[device].cylinders,devices[device].heads,devices[device].sectors_per_track);
		printk("%d bytes per sector, %d bytes per track\n",devices[device].bytes_per_sector,devices[device].bytes_per_track);
		printk("Buffer size: %d bytes\n",devices[device].buffer_size);

		disk_size=devices[device].cylinders * devices[device].heads * devices[device].sectors_per_track * devices[device].bytes_per_sector / 1024;
		printk("Size: %d KB\n",disk_size);

		block_size=devices[device].bytes_per_sector;
		if(block_size==0)
			block_size=512;

		printk("[Test Result - Total Time: %d miliseconds]\n",test_io_device(0,block_size));
	}	
}



