/* Scheduler includes. */
#include "FreeRTOS.h"
#include "semphr.h"

/* Demo includes. */
#include "IntTimer.h"
/* Library includes. */
#include "SMM_MPS2.h"

/* Timer frequencies are slightly offset so they nest. 
 * The frequency is defined here in Hz.
 *
 * Timer0 has a period of 1/2000 sec 
 * Timer1 has a period of 1/2001 sec
 *
 *
 * */
#define tmrTIMER_0_FREQUENCY	( 2000UL )
#define tmrTIMER_1_FREQUENCY	( 2001UL )


volatile uint32_t ulNest, ulNestCount;
extern SemaphoreHandle_t xBinarySemaphore;

void TIMER0_Handler( void )
{
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

	/* Clear interrupt. */
	CMSDK_TIMER0->INTCLEAR = ( 1ul <<  0 );


	if( ulNest > 0 )
	{
		/* This interrupt occurred in between the nesting count being incremented
		and decremented in the TIMER1_Handler.  Keep a count of the number of
		times this happens as its printed out by the check task in main_full.c.*/
		ulNestCount++;
	}

	/* 
	 * portEND_SWITCHING_ISR is a macro used in FreeRTOS to request a context switch from 
	 * within an Interrupt Service Routine (ISR) if needed. 
	 *
	 * The macro is specific to the hardware platform (port) being used and is part of 
	 * FreeRTOS's low-level scheduler support.
	 *
	 * Context Switching in ISRs:
	 *
	 * When an interrupt occurs, it can unblock a higher-priority task. If this happens, 
	 * the system needs to switch from the currently running task to the newly unblocked task.
	 *
	 * This is where portEND_SWITCHING_ISR comes in. It allows the kernel to efficiently request 
	 * a context switch if a higher-priority task is now ready to execute.
	 *
	 * Parameters:
	 *
	 *  portEND_SWITCHING_ISR usually takes one argument, a flag that indicates whether a context i
	 *  switch is needed.  
	 *
	 *  Typically, this flag is a variable like xHigherPriorityTaskWoken, which is set by FreeRTOS i
	 *  interrupt-safe API functions (e.g., xSemaphoreGiveFromISR) to pdTRUE if a higher-priority 
	 *  task was unblocked, or pdFALSE if no task change is required.
	 *
	 *  If the interrupt service routine has caused a higher-priority task to become ready to run 
	 *  (e.g., by unblocking a waiting task), xHigherPriorityTaskWoken is set to pdTRUE.
	 *
	 *  portEND_SWITCHING_ISR checks this flag, and if pdTRUE, it triggers a context switch. 
	 *  This ensures that the highest-priority ready task gets executed right after the ISR finishes.
	 */


    	xSemaphoreGiveFromISR(xBinarySemaphore, &xHigherPriorityTaskWoken);
	portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
}
/*-----------------------------------------------------------*/

void TIMER1_Handler( void )
{
	/* Increment the nest count while inside this ISR as a crude way of the
	higher priority timer interrupt knowing if it interrupted the execution of
	this ISR. */
	ulNest++;
	/* Clear interrupt. */
	CMSDK_TIMER1->INTCLEAR = ( 1ul <<  0 );
	ulNest--;
}
/*-----------------------------------------------------------*/

void vInitialiseTimers( void )
{
	/* Clear interrupt. 
	 * INTCLEAR: This is the interrupt clear register timer 0. 
	 * Writing to INTCLEAR clears the interrupt flag for the timer, 
	 * acknowledging that the interrupt has been handled.
	 * */
	CMSDK_TIMER0->INTCLEAR = ( 1ul <<  0 );

	 /* Reload value is slightly offset from the other timer. */

	/* RELOAD holds the reload value for the timer 0. This value determines 
	 * the countdown starting point for the timer each time it resets.
	 *
	 * Our CPU runs at 25MH therefore configCPU_CLOCK_HZ defined in FreeRTOSConfig.h
	 * is 25000000. 
	 * */
	CMSDK_TIMER0->RELOAD   = ( configCPU_CLOCK_HZ / tmrTIMER_0_FREQUENCY ) + 1UL;

	/* CTRL is the control register for Timer 0. This register configures and controls various 
	 * aspects of the timer’s operation, including enabling the timer, setting the timer mode, 
	 * and configuring the timer’s interrupt.
	 *
	 * In our emulated board bit 3 enables the interrupts and bit 0 starts the timer.
	*/


	CMSDK_TIMER0->CTRL     = ( ( 1ul <<  3 ) | /* Enable Timer interrupt. */
						     ( 1ul <<  0 ) );  /* Enable Timer. */

	CMSDK_TIMER1->INTCLEAR = ( 1ul <<  0 );
	CMSDK_TIMER1->RELOAD   = ( configCPU_CLOCK_HZ / tmrTIMER_1_FREQUENCY ) + 1UL;
	CMSDK_TIMER1->CTRL     = ( ( 1ul <<  3 ) |
						     ( 1ul <<  0 ) );

	/* NVIC_SetPriority: is an ARM CMSIS function that configures the priority of a specific interrupt 
	 * in the Nested Vectored Interrupt Controller (NVIC).  
	 * It takes two parameters: the interrupt number and the priority level to assign.*/
	NVIC_SetPriority( TIMER0_IRQn, configMAX_SYSCALL_INTERRUPT_PRIORITY );
	NVIC_SetPriority( TIMER1_IRQn, configMAX_SYSCALL_INTERRUPT_PRIORITY + 1 );
	
	
	/* NVIC_EnableIRQ is a function in the ARM CMSIS used to enable interrupts for a specific interrupt 
	 * source. It configures the Nested Vectored Interrupt Controller (NVIC) to allow the processor 
	 * to respond to a particular interrupt. 
	*/

	NVIC_EnableIRQ( TIMER0_IRQn );
	NVIC_EnableIRQ( TIMER1_IRQn );
}

