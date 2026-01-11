#include "uart.h"
#include <stdint.h>

extern t_ck_uart_device g_console_uart;

void print_hex(uint64_t val) {
    char hex[] = "0123456789ABCDEF";
    uart_puts("0x");
    for (int i = 15; i >= 0; i--) {
        int nibble = (val >> (i * 4)) & 0xF;
        ck_uart_putc(&g_console_uart, hex[nibble]);
    }
    uart_puts("\r\n");
}

/* Trap Handler */
__attribute__((used, aligned(4)))
void trap_handler(void) {
    uint64_t cause, epc;
    __asm__ volatile (
        "csrr %0, mcause\n"
        "csrr %1, mepc\n"
        : "=r"(cause), "=r"(epc)
    );
    uart_puts("\n=== HARDWARE TRAP ===\n");
    uart_puts("Cause: "); print_hex(cause);
    uart_puts("Addr:  "); print_hex(epc);
    
    if (cause == 2) {
        uart_puts("Type: Illegal Instruction (Hardware Refused Access)\n");
    }
    
    uart_puts("System Halted.\n");
    while (1);
}

int main(void) {
    console_init(); 
    uart_puts("\n=== Lichee Pi 4A: Vector Presence Probe ===\n");

    /* 1. CHECK MSTATUS */
    uint64_t mstatus_start, mstatus_end;
    
    // Read Initial
    __asm__ volatile("csrr %0, mstatus" : "=r"(mstatus_start));
    uart_puts("Initial mstatus: "); print_hex(mstatus_start);

    // Try to Enable Vectors (Set Bits 24:23 to '01' or '11')
    uart_puts("Attempting to set VS bits...\n");
    __asm__ volatile (
        "li t0, 0x01806000 \n"  // Set VS (24:23) and FS (14:13)
        "csrs mstatus, t0 \n"
        "fence.i \n"
        : : : "t0"
    );

    // Read Back
    __asm__ volatile("csrr %0, mstatus" : "=r"(mstatus_end));
    uart_puts("Final mstatus:   "); print_hex(mstatus_end);

    // Verify
    if (((mstatus_end >> 23) & 0x3) == 0) {
        uart_puts("\n*** FAILURE: VS Bits stuck at 0. Vectors not supported on this core. ***\n");
        return 0;
    }
    uart_puts("Status: VS Bits Enabled.\n");

    /* 2. PROBE VECTOR CSR (vstart) */
    uart_puts("Probing Vector CSR 'vstart' (0x008)...\n");
    
    uint64_t vstart_val = 0xDEADBEEF; // Garbage init
    
    // If Vectors are missing/locked, THIS instruction will Trap (MCAUSE 2)
    __asm__ volatile (
        "csrw vstart, zero \n"      // Try to write
        "csrr %0, vstart \n"        // Try to read back
        : "=r"(vstart_val)
    );

    uart_puts("vstart value:    "); print_hex(vstart_val);

    if (vstart_val == 0) {
        uart_puts("\n*** SUCCESS: VECTOR HARDWARE DETECTED ***\n");
        uart_puts("The CPU acknowledged the vector register access.\n");
        uart_puts("Any previous failures were due to complex setup/memory issues.\n");
    } else {
        uart_puts("\n*** FAILURE: vstart read garbage ***\n");
    }

    while (1);
    return 0;
}
