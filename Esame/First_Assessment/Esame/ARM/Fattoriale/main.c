#include "FreeRTOS.h"
#include "task.h"

#include "uart.h"

#define mainTASK_PRIORITY    ( tskIDLE_PRIORITY + 2 )

void Fattoriale();
void Fattoriale_asm();

int main(int argc, char **argv){

  (void) argc;
  (void) argv;

  UART_init();

  	xTaskCreate(
		// Function which implements the task
		Fattoriale,
		// Name of the task (debug purposes, not used by the kernel)
		"Task_Fattoriale",
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
		Fattoriale_asm,
		// Name of the task (debug purposes, not used by the kernel)
		"Task_Fattoriale_asm",
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


//////////////////////////////////////////////////////////////////////////////////////////
//Function to convert an integer to a string
/* Support function */
void intToString(int num, char* str) {
    if (num == 0) {
        str[0] = '0';
        str[1] = '\0';
        return;
    }
    int isNegative = 0;
    if (num < 0) {
        isNegative = 1;
        num = -num; 
  }
    char temp[12]; 
    int index = 0;
    while (num > 0) {
        temp[index++] = (num % 10) + '0'; 
        num /= 10; 
    }
    if (isNegative) {

        temp[index++] = '-';
    }
    temp[index] = '\0';
    for (int i = 0; i < index; i++) {
        str[i] = temp[index - 1 - i];
    }
    str[index] = '\0'; 
}





//////////////////////////////////////////////////////////////////////////////////////////
//Function to compute the factorial in asm
void Fattoriale(){
  UART_printf("\nFactorial in C\n");
  int result = 1;
  int numero = 10;

  char buffer[10];

  for(int i=numero;i>0;i--){
    result=result*i;
  }

  intToString(result,buffer);
  UART_printf(buffer); 
}


//////////////////////////////////////////////////////////////////////////////////////////
//Function to compute the factorial in asm
void Fattoriale_asm(){
  UART_printf("\nFactorial in ASM\n");
  int number = 10;
  int result;
  char buffer[12];

  __asm__ volatile(
    
    "mov r0,%1\n"
    "mov r1,#1\n"
    "fact_loop:\n"
    "mul r1,r1,r0\n"
    "sub r0,r0,#1\n"
    "cmp r0,#0\n"
    "bne fact_loop\n"
    "mov %0,r1\n"
    : "=r" (result)
    : "r"  (number)
  );
  
  intToString(result,buffer);
  UART_printf(buffer); 
}

