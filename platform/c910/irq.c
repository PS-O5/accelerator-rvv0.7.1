#include "FreeRTOS.h"
#include "task.h"

/* * This function is called by portASM.S when an external interrupt occurs.
 * cause: The value of the mcause register.
 */
void external_interrupt_handler( uint32_t cause )
{
    /* TODO: Parse the PLIC (Platform Level Interrupt Controller) here 
       to see which peripheral (UART, DMA, etc.) fired. */
    
    ( void ) cause; // Prevent unused variable warning
}
