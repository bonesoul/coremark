/*
	H�seyin Uslu, <shalafiraistlin@gmail.com>
*/

#include "include/console.h"
#include "include/types.h"

typedef struct
{
	uint8  signature[4];
	uint8  checksum;
	uint8  lenght;
	uint8  major_ver;
	uint8  minor_ver;
	uint16 max_structure_size;
	uint8  revision;
	uint8  formatted_area[5];
	uint8  i_sig[5]; // intermediate signature
	uint8  i_checksum; // intermediate checksum
	uint16 table_lenght;
	uint32 table_address;
	uint16 structure_count;
	uint8  bcd_revision; 	
}SMBIOS;

typedef struct {
	uint8	type;
	uint8	lenght;
	uint8	handle[2];
}SMBIOS_HEADER;

typedef uint8	SMBIOS_STRING;

typedef struct {
	SMBIOS_HEADER header;
	SMBIOS_STRING vendor;
	SMBIOS_STRING bios_version;
	uint8	      bios_segment[2];
	SMBIOS_STRING bios_release_date;
	uint8	      bios_size;
	uint8	      bios_characteristics[8];
} SMBIOS_BIOS;	

typedef struct {
	SMBIOS_HEADER header;
	SMBIOS_STRING manufacturer;
	SMBIOS_STRING product_name;
	SMBIOS_STRING version;
	SMBIOS_STRING serial;
} SMBIOS_SYSTEM;

typedef struct {
	SMBIOS_HEADER header;
	SMBIOS_STRING manufacturer;
	SMBIOS_STRING product_name;
	SMBIOS_STRING version;
	SMBIOS_STRING serial;
	SMBIOS_STRING asset_tag;
	uint8	      feature_flags;
} SMBIOS_BASE_BOARD;

char * get_smbios_string(SMBIOS_HEADER *,int);

void read_smbios(void)
{
	/* structure names */
	unsigned char * structure_names[3][20];
	*structure_names[0]=(unsigned char *)"BIOS Info:";
	*structure_names[1]=(unsigned char *)"System Info:";
	*structure_names[2]=(unsigned char *)"Base Board Info:";

	uint32 start_offset,stop_offset;
	uint32 loc,table_loc;	
	
	char * str;
	uint32 str_count;

	char smbios_found=0;
	int i;
	
	printk("Looking for SMBIOS...");
	
	start_offset= 0x000F0000; // search range
	stop_offset = 0x000FFFFF;	

	SMBIOS * sb;
	SMBIOS_HEADER * sh;
	SMBIOS_BIOS * smbios_bios;
	SMBIOS_SYSTEM * smbios_system;
	SMBIOS_BASE_BOARD * smbios_bb;
	
	for(loc=start_offset;loc<stop_offset;loc+=4)
	{
		sb=(SMBIOS *)loc;
		// check for magic string _SM_ (0x5f334d5f)
		if ( (sb->signature[0]=='_') & \
		     (sb->signature[1]=='S') & \
		     (sb->signature[2]=='M') & \
		     (sb->signature[3]=='_') )
		{
			printk("found\n");
			smbios_found=1;
			break;
		}	
	}

	if ( smbios_found == 0 )
		printk("not avaible\n");
	else
	{
		// parse SMBIOS info
		printk("SMBIOS ver: %x.%x rev:%x\n",sb->major_ver,sb->minor_ver,sb->revision);
		//printk("Structure Count: %d\n",sb->structure_count);
		//printk("Table Adress: %x\n",sb->table_address);

		table_loc=sb->table_address;
	
		for(i=0;i<3;i++)
		{	
			sh=(SMBIOS_HEADER *)table_loc;
			str_count=0;

			printk("\n%s\n",*structure_names[sh->type]);
			printk("--------------------------------\n");
			//printk("type: %x lenght: %x handle:%x%x\n",sh->type,sh->lenght,sh->handle[0],sh->handle[1]);

			if( sh->type == 0 )
			{
				// BIOS information struct
				smbios_bios=(SMBIOS_BIOS *)sh;

				/* vendor string */
				str=get_smbios_string(sh,1);
				str_count+=strlen(str)+1;
				printk("vendor : %s\n",str);	
			
				/* version string */
				str=get_smbios_string(sh,2);
				str_count+=strlen(str)+1;
				printk("version: %s\n",str);

				/* release string */
				str=get_smbios_string(sh,3);
				str_count+=strlen(str)+1;
				printk("release: %s\n",str);

				/* relocate table location */
				table_loc=table_loc + sh->lenght + str_count + 1;

			}
			else if ( sh->type == 1)
			{
				/* system info */
				smbios_system=(SMBIOS_SYSTEM*)sh;

				/* manufacturer */
				str=get_smbios_string(sh,1);
				str_count+=strlen(str)+1;
				printk("manufacturer: %s\n",str);

				/* product name */
				str=get_smbios_string(sh,2);
				str_count+=strlen(str)+1;
				printk("product: %s\n",str);

				/* version */
				str=get_smbios_string(sh,3);
				str_count+=strlen(str)+1;
				printk("version: %s\n",str);

				/* serial */
				str=get_smbios_string(sh,4);
				str_count+=strlen(str)+1;
				printk("serial: %s\n",str);

				table_loc=table_loc + sh->lenght + str_count + 1;
			}

			else if ( sh->type == 2)
			{
				/* base board info */
				smbios_bb=(SMBIOS_BASE_BOARD *)sh;
				
				/* manufacturer */
				str=get_smbios_string(sh,1);
				str_count+=strlen(str)+1;
				printk("manufacturer: %s\n",str);
				
				/* product */
				str=get_smbios_string(sh,2);
				str_count+=strlen(str)+1;
				printk("product: %s\n",str);
					
				/* version */
				str=get_smbios_string(sh,3);
				str_count+=strlen(str)+1;
				printk("version: %s\n",str);
				
				/* serial */
				str=get_smbios_string(sh,4);
				str_count+=strlen(str)+1;
				printk("serial: %s\n",str);

			}
		}			
	}
}

int strlen(const char * str)
{
	const char *s;
	for (s=str;*s;++s)
	;
	return (s-str);
}

char * get_smbios_string(SMBIOS_HEADER * sh,int string_no)
{
	int i;
	char *temp_str;
	temp_str=(char *)sh+sh->lenght;
	
	for(i=1;i<string_no;i++)
		temp_str=temp_str+strlen(temp_str)+1;
	return temp_str;
}
