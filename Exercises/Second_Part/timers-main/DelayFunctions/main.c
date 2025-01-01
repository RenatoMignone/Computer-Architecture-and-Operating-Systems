#include "FreeRTOS.h"
#include "task.h"
#include "uart.h"
#include <stdio.h>
#include <stdlib.h>
//this one is to gave a constant
#include <limits.h>

//we need to define 2 prios
//
#define mainTask_PRIORITY ( tskIDLE_PRIORITY +2)
#define samplingTask_PRIORITY ( tskIDLE_PRIORITY +3)


void TaskA (void *pvParameters);
void samplingFunction(void *pvParameters);

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv){

  (void) argc;
  (void) argv;


  UART_init();


  xTaskCreate(
    //pointer to the functino to execute
    TaskA,
    "TaskA",
    configMINIMAL_STACK_SIZE,
    NULL,
    mainTask_PRIORITY,
    NULL
  );


  xTaskCreate(
    //pointer to the functino to execute
    samplingFunction,
    "Sampling Function",
    configMINIMAL_STACK_SIZE,
    NULL,
    samplingTask_PRIORITY,
    NULL
  );



  vTaskStartScheduler();
  for( ; ; );
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//it is continuously using the CPU. It has to be an infinite loop and inside it another loop that counts from 0 to a value
void TaskA (void *pvParameters){
  
  (void) pvParameters;
  char message[100];
  //it is basically a long int, so to print we use the long int way.
  TickType_t t;

  for (;;){

    long int i;
    for (i=0;i<LONG_MAX;i++);
    t = xTaskGetTickCount();
    
    sprintf(message, "Busywaited %lu ticks\n",t);
    UART_printf(message);

  }
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void samplingFunction(void *pvParameters){

  (void) pvParameters;

  for (;;){

    UART_printf("Sampled\n");

    //convers the time of ms in ticks, in fact the delay is in ticks.
    vTaskDelay(pdMS_TO_TICKS(220));

  }

}


