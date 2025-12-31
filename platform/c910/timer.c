#include "FreeRTOS.h"
#include "task.h"

/* Addresses for C910/RISC-V Timer - ADJUST THESE FOR YOUR SPECIFIC HARDWARE */
/* These defaults are common for QEMU/Virt, but C910 hardware might differ */
volatile uint64_t * const pullMachineTimerCompareRegister = ( volatile uint64_t * const ) configMTIMECMP_BASE_ADDRESS;
volatile uint64_t * const pullMachineTimerCounterRegister = ( volatile uint64_t * const ) configMTIME_BASE_ADDRESS;

/* * This is called by the FreeRTOS RISC-V port to setup the tick timer.
 */
void vPortSetupTimerInterrupt( void )
{
    uint32_t ulCurrentTimeHigh, ulCurrentTimeLow;
    volatile uint32_t * const pulTimeHigh = ( volatile uint32_t * const ) ( ( uint32_t ) pullMachineTimerCounterRegister + 1UL );
    volatile uint32_t * const pulTimeLow = ( volatile uint32_t * const ) ( ( uint32_t ) pullMachineTimerCounterRegister );
    volatile uint32_t ulCompareMatchValue;

    /* Get current time */
    do
    {
        ulCurrentTimeHigh = *pulTimeHigh;
        ulCurrentTimeLow = *pulTimeLow;
    } while( ulCurrentTimeHigh != *pulTimeHigh );

    /* Add the tick period to current time to set the next interrupt */
    ulCompareMatchValue = ulCurrentTimeLow + ( configCPU_CLOCK_HZ / configTICK_RATE_HZ );

    /* Set the compare register */
    *pullMachineTimerCompareRegister = ( uint64_t ) ulCompareMatchValue; // simplified for 32-bit tick logic, works for startup
    
    /* Enable Machine Timer Interrupt (mie.mtie) */
    __asm volatile( "csrs mie, %0" :: "r"(0x80) );
}
