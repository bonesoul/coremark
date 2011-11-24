ENTRY (start) /* kernel entrance point function; start in boot.S */

SECTIONS
{
    . = 0x00100000;

    .text :
    {
        *(.text)
        *(.rodata)
    }

    .data ALIGN (0x1000) :
    {
        *(.data)
    }

    .bss :
    {
        _sbss = .;
        *(COMMON)
        *(.bss)
        _ebss = .;
    }
}

/* check http://www.gnu.org/software/binutils/manual/ld-2.9.1/html_mono/ld.html 
   for more information on linker scripts */
