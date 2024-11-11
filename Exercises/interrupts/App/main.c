/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/* Standard includes. */
#include <stdio.h>
#include <string.h>

/* Application includes. */
#include "IntTimer.h"

/* Task priorities */

#define mainTASKA_STACK_SIZE ( configMINIMAL_STACK_SIZE + ( configMINIMAL_STACK_SIZE >> 1 ) )
#define mainTASKB_STACK_SIZE ( configMINIMAL_STACK_SIZE  )

/* printf() output uses the UART.  These constants define the addresses of the
 * required UART registers. */

#define UART0_ADDRESS                         ( 0x40004000UL )
#define UART0_DATA                            ( *( ( ( volatile uint32_t * ) ( UART0_ADDRESS + 0UL ) ) ) )
#define UART0_STATE                           ( *( ( ( volatile uint32_t * ) ( UART0_ADDRESS + 4UL ) ) ) )
#define UART0_CTRL                            ( *( ( ( volatile uint32_t * ) ( UART0_ADDRESS + 8UL ) ) ) )
#define UART0_BAUDDIV                         ( *( ( ( volatile uint32_t * ) ( UART0_ADDRESS + 16UL ) ) ) )
#define TX_BUFFER_MASK                        ( 1UL )


/* GLOBAL VARIABLES */

/* Semaphore used to implement the deferred interrupt */
SemaphoreHandle_t xBinarySemaphore;
/* Seed used to generate pseudo random numbers */
static uint32_t seed = 14536;
/* Global counter to count when nested interrupts occur */
extern uint32_t ulNestCount;


static void prvUARTInit( void )
{
    UART0_BAUDDIV = 16;
    UART0_CTRL = 1;
}

/* Custom function to generate a pseudo-random number */
uint32_t simpleRandom() {
    // Linear Congruential Generator (LCG) parameters
    seed = (seed * 1664525 + 1013904223); // Modulus is implicitly 2^32
    return seed;
}

/* Funcion to generate CPU intensive activity  */
uint32_t cpuIntensiveFibonacci(uint32_t n) {
    uint32_t a = 0, b = 1, temp;

    if (n == 0) {
        return a;
    }
    if (n == 1) {
        return b;
    }

    // Calculate Fibonacci iteratively
    for (uint32_t i = 2; i <= n; i++) {
        temp = a + b;
        a = b;
        b = temp;
    }

    return b;
}

/* Deferred interrupt task called on Timer 0 */
void TaskB( void * pvParameters ) {
    ( void ) pvParameters;
    for (;;) {
        // Wait for the semaphore from the ISR
        if (xSemaphoreTake(xBinarySemaphore, portMAX_DELAY) == pdTRUE) {
            // The semaphore was successfully taken, meaning the ISR occurred
            // Perform the deferred processing here
	    	if (ulNestCount > 0) {
			printf ("Deferred task executed and detected %u nested interrupts \n",(unsigned) ulNestCount);
			ulNestCount = 0;
		}
        }
    }

}
	
/* Main task */
void TaskA( void * pvParameters )
{
    /* Avoid warning about unused parameter. */
    ( void ) pvParameters;

    int fib;
    
    const TickType_t xTaskPeriod = pdMS_TO_TICKS( 5000UL );
    TickType_t xPreviousWakeTime;


    xPreviousWakeTime = xTaskGetTickCount();

    for( ; ; )
    {

	/* In FreeRTOS, vTaskDelayUntil() is a function that allows a task to enter a timed waiting state 
	 * in a way that maintains a fixed, periodic execution rate. It is particularly useful for tasks 
	 * that need to run at precise intervals, as it helps prevent "drift" in timing. 
	 *
	 * Unlike vTaskDelay(), which simply delays a task for a set amount of time from the moment 
	 * it is called, vTaskDelayUntil() calculates the delay relative to a previous "last wake time."
	 *
	 * This allows the task to run at precise intervals, even if the task's execution is delayed by 
	 * other tasks or interrupts.
	 *
	 * vTaskDelayUntil() compensates for any delays or time spent while the task was running. 
	 * It ensures that the task resumes on a consistent schedule rather than accumulating 
	 * delays over time.
	 */

        vTaskDelayUntil( &xPreviousWakeTime, xTaskPeriod );
	fib = (simpleRandom() % (20 - 1 + 1)) + 1;
        printf( "Fibonacci %d = %u - TickCount %d \r\n", 
			fib, 
			(unsigned) cpuIntensiveFibonacci(fib), 
			( int ) xTaskGetTickCount() 
			);
    }
}

void main( void )
{

    xBinarySemaphore = xSemaphoreCreateBinary();
    if (xBinarySemaphore != NULL) {
        // Create the deferred task with a priority that suits your application
        xTaskCreate(TaskB, "TaskB", mainTASKB_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL);
    }

    /* Hardware initialisation.   */
    prvUARTInit();
    vInitialiseTimers();


    xTaskCreate( TaskA, "TaskA", mainTASKA_STACK_SIZE, NULL, tskIDLE_PRIORITY+1, NULL );

    /* Start the scheduler. */
    vTaskStartScheduler();

    for( ; ; )
    {
    }

}


