#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

//------------------
#include "uart.h"
#include <stdio.h> 
//------------------

SemaphoreHandle_t xMutex;
SemaphoreHandle_t xInspectionSignal;

void ConveyorTask(void *pvParameters) {
    int conveyorID = (int)pvParameters;
    while (1) {
        if (xSemaphoreTake(xMutex, portMAX_DELAY)) {
            UART_printf("Conveyor %d Running\n", conveyorID);
            vTaskDelay(pdMS_TO_TICKS(2000)); // Simulate operation
            UART_printf("Conveyor %d Stopped\n", conveyorID);
            xSemaphoreGive(xMutex);
            xSemaphoreGive(xInspectionSignal); // Signal inspection
            vTaskDelay(pdMS_TO_TICKS(1000)); // Pause before next cycle
        }
    }
}

void InspectionTask(void *pvParameters) {
    while (1) {
        if (xSemaphoreTake(xInspectionSignal, portMAX_DELAY)) {
            UART_printf("Inspecting Product\n");
            vTaskDelay(pdMS_TO_TICKS(1500)); // Simulate inspection
            UART_printf("Inspection Complete\n");
        }
    }
}

void LoggerTask(void *pvParameters) {
    while (1) {
        // Simulate logging
        UART_printf("Logger: System running\n");
        vTaskDelay(pdMS_TO_TICKS(5000)); // Periodic logging
    }
}

int main(void) {
    
    UART_init();

    xMutex = xSemaphoreCreateMutex();
    xInspectionSignal = xSemaphoreCreateBinary();

    xTaskCreate(ConveyorTask, "Conveyor1", 1000, (void *)1, 2, NULL);
    xTaskCreate(ConveyorTask, "Conveyor2", 1000, (void *)2, 2, NULL);
    xTaskCreate(InspectionTask, "Inspection", 1000, NULL, 3, NULL);
    xTaskCreate(LoggerTask, "Logger", 1000, NULL, 1, NULL);

    vTaskStartScheduler();
    while (1);
}
