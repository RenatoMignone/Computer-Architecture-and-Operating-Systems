#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"
#include <stdio.h>
#include <string.h>
#include "uart.h"

TimerHandle_t xPeriodicTimerHandle;
TimerHandle_t xOneShotTimerHandle;


void vTaskA(void *pvParameters) {
    (void) pvParameters;
    int activationCount = 0;
    char buffer[100];
    while (1) {
        activationCount++;
        sprintf(buffer,"TaskA activated %d times\n", activationCount);
        UART_printf(buffer);
        vTaskDelay(pdMS_TO_TICKS(1000)); 
    }
}

void vHighPriorityTask(void *pvParameters) {
    (void) pvParameters;
    UART_printf("High priority task activated\n");
    vTaskDelete(NULL);
}


void vOneShotTimerCallback(void) {
    UART_printf("GAME OVER\n");
}

void vPeriodicTimerCallback(void) {
    xTaskCreate(vHighPriorityTask, "HighPriorityTask", configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES - 1, NULL);
}

int main(void) {

    UART_init();

    xTaskCreate(vTaskA, "TaskA", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);

    xPeriodicTimerHandle = xTimerCreate("PeriodicTimer", pdMS_TO_TICKS(2000), pdTRUE, NULL, vPeriodicTimerCallback);
    if (xPeriodicTimerHandle != NULL) {
        xTimerStart(xPeriodicTimerHandle, 0);
    }

    xOneShotTimerHandle = xTimerCreate("OneShotTimer", pdMS_TO_TICKS(2000), pdFALSE, NULL, vOneShotTimerCallback);
    if (xOneShotTimerHandle != NULL) {
        xTimerStart(xOneShotTimerHandle, 0);
    }

    vTaskStartScheduler();


    for (;;){

    }
}
