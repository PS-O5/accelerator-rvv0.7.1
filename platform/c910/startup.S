.section .text
.global _start
.global _stack_top

_start:
    /* Disable interrupts globally */
    csrw mie, zero

    /* Initialize Stack Pointer */
    la sp, _stack_top

    /* Initialize Global Pointer (optional but recommended) */
    .option push
    .option norelax
    la gp, __global_pointer$
    .option pop

    /* Jump to main */
    call main

    /* Hang if main returns */
loop:
    j loop
