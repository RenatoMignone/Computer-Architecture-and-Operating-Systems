#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
//------------------
#include "uart.h"
#include <stdio.h>
#include "stdbool.h"
//------------------

#define mainTASK_PRIORITY    ( tskIDLE_PRIORITY + 2 )
#define QUEUE_LEN 50


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
typedef struct{
    int intex;
}Data;
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
bool traffic_light_street = false;
//////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
void TrafficLight(void *pvParameters);
void PedestrianCrossing(void *pvParameters);
void EmergencyVehicle(void *pvParameters);
void CentralMonitoring(void *pvParameters);

//////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
QueueHandle_t xDataQueue;

//////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
SemaphoreHandle_t xMutex;

//////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
int main(int argc, char **argv){

        (void) argc;
        (void) argv;

    UART_init();

    xDataQueue = xQueueCreate(QUEUE_LEN,sizeof(Data));
    xMutex = xSemaphoreCreateMutex();
    xSemaphoreGive(xMutex);

    xTaskCreate(TrafficLight, "TrafficLight", 1000, NULL, mainTASK_PRIORITY+1, NULL);
    xTaskCreate(PedestrianCrossing, "PedestrianCrossing", 1000, NULL, mainTASK_PRIORITY+1, NULL);
    xTaskCreate(EmergencyVehicle, "EmergencyVehicle", 1000, NULL, mainTASK_PRIORITY+1, NULL);
    xTaskCreate(CentralMonitoring, "CentralMonitoring", 1000, NULL, mainTASK_PRIORITY+1, NULL);


    vTaskStartScheduler();
    while (1);
}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
void TrafficLight(void *pvParameters){
    (void) pvParameters;
    while(1){
        xSemaphoreTake(xMutex,portMAX_DELAY);
        traffic_light_street = true;
        UART_printf("\nTraffic Light: GREEN\n");
        vTaskDelay(pdMS_TO_TICKS(2000));
        xSemaphoreGive(xMutex);

        vTaskDelay(pdMS_TO_TICKS(500));

    }
}
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
void PedestrianCrossing(void *pvParameters){
    (void) pvParameters;

    while(1){
        vTaskDelay(pdMS_TO_TICKS(10000));
        UART_printf("\nButton Pressing\n");
        xSemaphoreTake(xMutex,portMAX_DELAY);
        traffic_light_street = false;
        UART_printf("\nTraffic Light: RED\n");
        vTaskDelay(pdMS_TO_TICKS(2000));
        xSemaphoreGive(xMutex);
        vTaskDelay(pdMS_TO_TICKS(500));

    }
}
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
void EmergencyVehicle(void *pvParameters){
    (void) pvParameters;

    while(1){
        
    }
}
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
void CentralMonitoring(void *pvParameters){
    (void) pvParameters;

    while(1){
        
    } 
}




