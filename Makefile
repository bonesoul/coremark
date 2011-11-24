# Huseyin Uslu, <shalafiraistlin@gmail.com>

# Compiler options
CC = gcc
CFLAGS = -c -g -ffreestanding -nostdlib -fno-builtin -fno-exceptions -Wall -fno-stack-protector
COMPILE_C = $(CC) $(CFLAGS)
ASM=nasm
AFLAGS=-f elf 
ASSEMBLER=$(ASM) $(AFLAGS)
LFLAGS = -T linker.ld -Map kernel.map -o $(OUTPUT) --oformat elf32-i386
LINKER = ld
LINK = $(LINKER) $(LFLAGS)


# Output options
OUTPUT=kernel.bin
IMAGE = kernel.img
ISO = kernel.iso
TEMP_FLOPPY = tmp_floppy
LOOP_DEV = /dev/loop8
GRUB_ZIP=grub.zip
GRUB_IMG=grub.img

# Subsystems
COMMON=src/common/misc.o
CONSOLE=src/kernel/console.o
BOOT=src/boot/boot.o src/boot/multiboot.o
CPU=src/x86/cpuid.o src/x86/cpu_info.o
EXCEPTION=src/x86/exception.o
GDT=src/x86/gdt.o src/x86/gdt_flush.o
IDT=src/x86/idt.o src/x86/load_idt.o
IRQ=src/x86/irq.o src/x86/irq_handler.o
ISR=src/x86/isr.o
PIC=src/x86/pic.o src/x86/time.o
PCI=src/x86/pci.o
KERNEL=src/kernel.o

OBJS=$(COMMON) $(CONSOLE) $(BOOT) $(CPU) $(EXCEPTION) $(GDT) $(IDT) $(IRQ) $(ISR) $(PIC) $(PCI) $(KERNEL)


compile:
		@make $(OUTPUT)


$(OUTPUT): $(OBJS)
		@echo -e "\t[\x1B[1m$(LINKER)\x1B[0m]\t$(IMAGE)"
		@$(LINKER) $(LFLAGS) $(OBJS)
		@echo -e "\t[\x1B[1mmbchk\x1B[0m]\t$(OUTPUT)"
		@mbchk $(OUTPUT)

.a.o:
		@echo -e "\t[\x1B[1m$(ASM)\x1B[0m]\t$*.a"
		@$(ASSEMBLER) $(AFLAGS) $*.a -o $*.o
			
.c.o:
			@echo -e "\t[\x1B[1m$(CC)\x1B[0m]\t$*.c"
			@$(COMPILE_C) $(CFLAGS) $*.c -o $*.o
			
.S.o:
			@echo -e "\t[\x1B[1m$(CC)\x1B[0m]\t$*.S"
			@$(COMPILE_C) $(CFLAGS) $*.S -o $*.o
			 
clean:
			@echo -e "\t[\x1B[1mcleaning..\x1B[0m]\t$(OBJS) $(OUTPUT) $(IMAGE) $(ISO)"
			@rm -f -R $(OBJS) $(OUTPUT) $(IMAGE) $(ISO)
			
image:
			@rm -f -R $(TEMP_FLOPPY)
			@rm -f $(IMAGE)
			@rm -f $(GRUB_IMG)
			@echo -e "\t[\x1B[1mdd\x1B[0m]\t$(IMAGE)"
			@dd if=/dev/zero of=$(IMAGE) bs=512 count=2880
			@echo -e "\t[\x1B[1mmknod\x1B[0m]\t$(LOOP_DEV)"
			@-mknod -m660 $(LOOP_DEV) b 7 8
			@echo -e "\t[\x1B[1mlosetup\x1B[0m]\t$(LOOP_DEV)"
			@-/sbin/losetup -d $(LOOP_DEV)
			@/sbin/losetup $(LOOP_DEV) $(IMAGE)
			@echo -e "\t[\x1B[1mmkdosfs\x1B[0m]\t$(LOOP_DEV)"
			@mkdosfs $(LOOP_DEV)
			@mkdir $(TEMP_FLOPPY)			
			@echo -e "\t[\x1B[1mmount\x1B[0m]\t$(LOOP_DEV)->$(TEMP_FLOPPY)"
			@mount -t msdos $(LOOP_DEV) $(TEMP_FLOPPY)
			@unzip -q $(GRUB_ZIP)
			@echo -e "\t[\x1B[1mcopy\x1B[0m]\t$(GRUB_IMG) $(OUTPUT)->(LOOP_DEV)"
			@cat $(GRUB_IMG)>$(LOOP_DEV)
			@cp $(OUTPUT) $(TEMP_FLOPPY)
			@umount $(LOOP_DEV)
			
			
iso:
			@make image
		  @mkisofs -r -b $(IMAGE) -c boot.catalog -o $(ISO) .
		  