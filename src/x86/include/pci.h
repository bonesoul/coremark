/*
	$Id: pci.h 73 2007-05-06 14:36:08Z raistlinthewiz $
*/
#ifndef __PCI_H__
#define __PCI_H__

#include "../../common/include/types.h"

void scan_pci_devices();


/* http://wiki.osdev.org/PCI */

typedef struct
{
uint16 VendorID; 
uint16 DeviceID;

uint16 CommandReg;
uint16 StatusReg;

uint8 ClassCode;
uint8 SubClass;
uint8 RevisionID;
uint8 ProgIF;

uint8 CachelineSize;
uint8 Latency;
uint8 HeaderType;
uint8 BIST;

uint32 BAR0; //Base Address Register
uint32 BAR1;
uint32 BAR2;
uint32 BAR3;
uint32 BAR4;
uint32 BAR5;
uint32 CardbusCISPtr;
uint16 SubVendorID; //Subsystem
uint16 SubDeviceID;
uint32 ExRomAddress; //Expansion ROM
uint32 Reserved1;
uint32 Reserved2;
uint8 IRQ; //IRQ number
uint8 PIN; //IRQ PIN number
uint8 MinGrant;
uint8 MaxLatency;
} pci_device_t;


void outb(uint16 address, uint8 data);
void outw(uint16 address, uint16 data);
void outl(uint16 address, uint32 data);

uint8 inb(uint16 address);
uint16 inw(uint16 address);
uint32 inl(uint16 address);

typedef struct _PCI_VENTABLE
{
	unsigned short	VenId ;
	char *	VenShort ;
	char *	VenFull ;
}  PCI_VENTABLE;
// , *PPCI_VENTABLE 

typedef struct _PCI_DEVTABLE
{
	uint16	VenId ;
	uint16	DevId ;
	char *	Chip ;
	char *	ChipDesc ;
}  PCI_DEVTABLE;
//*PPCI_DEVTABLE ;

// Use this value for loop control during searching:
#define	PCI_VENTABLE_LEN	(sizeof(PciVenTable)/sizeof(PCI_VENTABLE))

// Use this value for loop control during searching:
#define	PCI_DEVTABLE_LEN	(sizeof(PciDevTable)/sizeof(PCI_DEVTABLE))



#endif
