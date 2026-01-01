MEMORY
{
MEM1(RWX)  : ORIGIN = 0x00000000,  LENGTH = 0x40000
MEM2(RWX)  : ORIGIN = 0x00040000,  LENGTH = 0xc0000
}
__kernel_stack = 0xee000 ;

ENTRY(__start)

SECTIONS {
    .text :
    {
        crt0.o (.text)
        *(.text*)
    } >MEM1

    .rodata :  {   *( .rodata* )
                   *(.srodata)
                   *(.srodata.*)
                   *(.srodata.cst4*)
		   *(.srodata.cst8*)
		  } >MEM1

    .data :
    {
        *(.data*)
        *(.sdata*)
    } >MEM2
    .bss :
    {
        *(.bss) *.(COMMON) *(.sbss)
    } >MEM2
    end = .;
}
