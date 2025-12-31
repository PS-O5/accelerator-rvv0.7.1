#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/* --- Basic Setup --- */
#define configUSE_PREEMPTION                    1
#define configUSE_IDLE_HOOK                     0
#define configUSE_TICK_HOOK                     0
#define configCPU_CLOCK_HZ                      ( 10000000 ) /* Set to your actual Core Freq */
#define configTICK_RATE_HZ                      ( ( TickType_t ) 1000 )
#define configMAX_PRIORITIES                    ( 5 )
#define configMINIMAL_STACK_SIZE                ( ( unsigned short ) 256 ) /* Higher for 64-bit */
#define configTOTAL_HEAP_SIZE                   ( ( size_t ) ( 64 * 1024 ) )
#define configMAX_TASK_NAME_LEN                 ( 16 )
#define configUSE_16_BIT_TICKS                  0
#define configIDLE_SHOULD_YIELD                 0

/* --- RISC-V Specifics (CRITICAL) --- */
/* The C910 usually has CLINT. You must find the address of the CLINT 
   MTIME register for your specific hardware map. 
   Example below is QEMU 'virt' machine default. CHANGE THIS. */
#define configMTIME_BASE_ADDRESS                ( 0x0200bff8UL )
#define configMTIMECMP_BASE_ADDRESS             ( 0x02004000UL )

/* Map the FreeRTOS trap handler to the standard RISC-V names */
#define vPortSVCHandler                         freertos_risc_v_trap_handler
#define xPortPendSVHandler                      freertos_risc_v_trap_handler
#define xPortSysTickHandler                     freertos_risc_v_trap_handler

#endif /* FREERTOS_CONFIG_H */
