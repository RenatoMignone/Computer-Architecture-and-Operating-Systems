#include "FreeRTOS.h"
#include "task.h"

#include "uart.h"

#define mainTASK_PRIORITY    ( tskIDLE_PRIORITY + 2 )

void vTaskFunction(void *pvParameters);
void vTaskFunction2(void *pvParameters);
void vTaskFunction_Fibonacci1(void *pvParameters);
void vTaskFunction_Fibonacci2(void *pvParameters);
void intToString(int num, char* str);

int main(int argc, char **argv){

	(void) argc;
	(void) argv;

  UART_init();
	/*
	xTaskCreate(
		// Function which implements the task
		vTaskFunction,
		// Name of the task (debug purposes, not used by the kernel)
		"Task1",
		// Stack to allocate to the task
		configMINIMAL_STACK_SIZE,
		// Parameter passed to the task. Not needed for Hello World example
		NULL,
		// Priority assigned to the task
		mainTASK_PRIORITY,
		// Task handle. Not required
		NULL
	);

	
	xTaskCreate(
		// Function which implements the task
		vTaskFunction2,
		// Name of the task (debug purposes, not used by the kernel)
		"Task2",
		// Stack to allocate to the task
		configMINIMAL_STACK_SIZE,
		// Parameter passed to the task. Not needed for Hello World example
		NULL,
		// Priority assigned to the task
		mainTASK_PRIORITY-1,
		// Task handle. Not required
		NULL
	);
	*/
	
	xTaskCreate(
		// Function which implements the task
		vTaskFunction_Fibonacci1,
		// Name of the task (debug purposes, not used by the kernel)
		"Task_Fibonacci1",
		// Stack to allocate to the task
		configMINIMAL_STACK_SIZE,
		// Parameter passed to the task. Not needed for Hello World example
		NULL,
		// Priority assigned to the task
		mainTASK_PRIORITY,
		// Task handle. Not required
		NULL
	);
	
	xTaskCreate(
		// Function which implements the task
		vTaskFunction_Fibonacci2,
		// Name of the task (debug purposes, not used by the kernel)
		"Task_Fibonacci2",
		// Stack to allocate to the task
		configMINIMAL_STACK_SIZE,
		// Parameter passed to the task. Not needed for Hello World example
		NULL,
		// Priority assigned to the task
		mainTASK_PRIORITY,
		// Task handle. Not required
		NULL
	);

	// Give control to the scheduler
	vTaskStartScheduler();

	// If everything ok should never reach here
  for( ; ; );
}











/* Task Function */
void vTaskFunction(void *pvParameters) {

	// Avoid warning about unused pvParameters
	(void) pvParameters;

    for (;;) {

        // Task code: print a message
        UART_printf("Hello, World!\n");

        // Delay for 1 second
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

/* Task Function */
void vTaskFunction2(void *pvParameters) {

	// Avoid warning about unused pvParameters
	(void) pvParameters;

    for (;;) {

        // Task code: print a message
        UART_printf("Hello, World! from task 2\n");

        // Delay for 1 second
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}



/* Support function */
void intToString(int num, char* str) {
    // Handle the case of zero explicitly
    if (num == 0) {
        str[0] = '0';
        str[1] = '\0'; // Null-terminate the string
        return;
    }

    // Determine if the number is negative
    int isNegative = 0;
    if (num < 0) {
        isNegative = 1;
        num = -num; // Make the number positive for easier handling
    }

    // Temporary variable to hold the string in reverse
    char temp[12]; // Enough to hold all digits of int including sign and null
    int index = 0;

    // Extract digits and store them in reverse order
    while (num > 0) {
        temp[index++] = (num % 10) + '0'; // Convert digit to character
        num /= 10; // Reduce num by a factor of 10
    }

    // Add the negative sign if the number was negative
    if (isNegative) {
        temp[index++] = '-';
    }

    // Null-terminate the string
    temp[index] = '\0';

    // Reverse the string into the output buffer
    for (int i = 0; i < index; i++) {
        str[i] = temp[index - 1 - i];
    }
    str[index] = '\0'; // Null-terminate the output string
}












/* Task Function */
void vTaskFunction_Fibonacci1(void *pvParameters) {

	// Avoid warning about unused pvParameters
	(void) pvParameters;
	
    char buffer0[20];
    char buffer1[20];
	
    int N = 10;
    int n2 = 0;
    int n1 = 1;
    int n = 0;	
    int i = 0;

    for (;;) {

        // Task code: print a message
        if (i < N) {
        	// Fibonacci
	    	n = n2 + n1;
		
		    // Call the function to convert integer to string
		    intToString(i, buffer0);
    		intToString(n, buffer1);
    		
    		// Print
          UART_printf(buffer0);
	       	UART_printf(" - Fibonacci 1: ");
	       	UART_printf(buffer1);
          UART_printf("\n");

	       	
	       	// Update values
	       	n2 = n1;
		n1 = n;
		i++;
    }
    else{
      vTaskDelete(NULL);
    }

        // Delay for 1 second
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}











/* Task Function */
void vTaskFunction_Fibonacci2(void *pvParameters) {

	// Avoid warning about unused pvParameters
	(void) pvParameters;
	
    char buffer0[20];
    char buffer1[20];

    int N = 10;
    int n2 = 0;
    int n1 = 1;
    int n = 0;	
    int i = 0;

    for (;;) {

        // Task code: print a message
      if (i < N) {
        	// Fibonacci
		        n = n2 + n1;
		
		        // Call the function to convert integer to string
		        intToString(i, buffer0);
    		    intToString(n, buffer1);
    		
    		    // Print
            UART_printf(buffer0);
	       	  UART_printf(" - Fibonacci 2: ");
	       	  UART_printf(buffer1);
            UART_printf("\n");
	       	
	       	// Update values
	       	  n2 = n1;
		        n1 = n;
		        i++;
	}
  
    else{
      vTaskDelete(NULL);
    }


        // Delay for 1 second
        vTaskDelay(pdMS_TO_TICKS(1000));
  }
}
