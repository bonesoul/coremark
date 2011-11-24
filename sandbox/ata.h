#ifndef __ATA_H__
#define __ATA_H__

#include "types.h"

/* 
	IDE Controllers -compability mode
	Channel   Command Block Registers Control Block Registers IRQ
	Primary   1F0h - 1F7h     	  3F6h		          14
	Secondary 170h - 177h     	  376h		          15
*/

/* controller check */
#define MAGIC_NO 0x99 /* magic number for controller testing */

/* controllers */
#define PRIMARY_BASE	  0x1f0
#define SECONDARY_BASE	  0x170
#define CB_PRIMARY_BASE   0x3f6
#define CB_SECONDARY_BASE 0x376

/* command block registers */
/* read */
#define DATA_PORT        0X0
#define ERROR_REGISTER   0x1
#define FEATURE_REGISTER 0x1
#define SECTOR_COUNT   0x2
#define SECTOR_NUMBER  0x3
#define LBA_LOW        0x3
#define CYLINDER_LOW   0x4
#define LBA_MID        0x4
#define CYLINDER_HIGH  0x5
#define LBA_HIGH       0x5
#define DRIVE_SELECT   0x6
#define DRIVE_HEAD     0x6
#define STATUS	       0x7
#define COMMAND	       0x7

/* control block registers - read */
#define ALTERNATIVE_STATUS 0x0 /* alt. status register to one in 0x1f7 or 0x177 */
#define DRIVE_ADDRESS      0x1



/* status register bits 
   bit7 - busy bit 
   bit3 - data request bit
*/
#define BUSY_BIT 0x80
#define  DRQ_BIT 0x8

#define CMD_IDENTIFY_DRIVE 0xEC

/* device types */
#define ATADEV_NODEVICE 0x0
#define ATADEV_PATA     0x1
#define ATADEV_PATAPI   0x2
#define ATADEV_SATA     0x3
#define ATADEV_SATAPI   0x4
#define ATADEV_UNKNOWN  0x5


typedef struct ata_device_identification
{
	uint16 dev_type;
	uint16 cylinders;
	uint16 reserved_1;
	uint16 heads;
	uint16 bytes_per_track;
	uint16 bytes_per_sector;
	uint16 sectors_per_track;
	uint16 gaps;
	uint16 reserved_2;
	uint16 vendor_id;
	uint8  serial_number[20];
	uint16 controller_type;
	uint16 buffer_size;
	uint16 ecc_bytes;
	uint8  firmware_revision[8];
	uint8  model[40];
	uint32 reserved_3;
	uint32 reserved_4;
	uint32 reserved_5;
	uint16 reserved_6;
}ata_device;

void check_ata_devices(void);

void test_io_devices(void);

#endif
