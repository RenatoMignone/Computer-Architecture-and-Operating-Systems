#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
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
bool traffic_light_street = false; //red for pedoni
//////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
void TrafficLight(void *pvParameters);
void PedestrianCrossing(void *pvParameters);
void EmergencyVehicle(void *pvParameters);
void CentralMonitoring(void *pvParameters);
void myTimerCallback(TimerHandle_t t);
void EmergencyCallback(TimerHandle_t t);

//////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
bool button = false;
TimerHandle_t TrafficTimer;
TimerHandle_t EmergencyTimer;
SemaphoreHandle_t xMutex;
//////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
void myTimerCallback(TimerHandle_t t){
    if(xSemaphoreTake(xMutex,portMAX_DELAY) == pdTRUE){
        traffic_light_street = true; //Green for pedoni
        UART_printf("\nThe light is now GREEN\n");
        vTaskDelay(pdMS_TO_TICKS(2000));
        traffic_light_street = false;
        UART_printf("\nThe light is now RED\n");
    }
    xSemaphoreGive(xMutex);
}

void EmergencyCallback(TimerHandle_t t){


}

//////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
int main(int argc, char **argv){

        (void) argc;
        (void) argv;

    UART_init();

    EmergencyTimer = xTimerCreate(
        "EmergencyTimer",
        pdMS_TO_TICKS(7000),
        pdTRUE,
        (void *)0,
        EmergencyCallback
    );

    TrafficTimer = xTimerCreate(
        "TrafficTimer",
        pdMS_TO_TICKS(3000),
        pdTRUE,
        (void *)1,
        myTimerCallback
    );


    xMutex = xSemaphoreCreateBinary();
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
    
    xTimerStart(TrafficTimer,portMAX_DELAY);

    while(1){

    }
}
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
void PedestrianCrossing(void *pvParameters){
    (void) pvParameters;

    while(1){

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




