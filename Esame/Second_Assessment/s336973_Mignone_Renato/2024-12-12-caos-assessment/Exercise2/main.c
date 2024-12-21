
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "uart.h"

#include <stdio.h>
#include <string.h>

TickType_t xDelay = pdMS_TO_TICKS( 20 );

static void vTaskA(void *pvParameters);
static void vTaskC(void *pvParameters);
static void vTaskB(void *pvParameters);
static void vTaskD(void *pvParameters);

#define TASK_A_PRIORITY    ( tskIDLE_PRIORITY + 3 )
#define TASK_BC_PRIORITY   ( tskIDLE_PRIORITY + 2 )
#define TASK_D_PRIORITY    ( tskIDLE_PRIORITY + 1 )

SemaphoreHandle_t xSemaphoreBC;
SemaphoreHandle_t xSemaphoreD;
SemaphoreHandle_t xMutex;


void main( void )
{
    UART_init();
    xMutex = xSemaphoreCreateMutex();
    xSemaphoreBC = xSemaphoreCreateBinary();
    xSemaphoreD = xSemaphoreCreateBinary();

    if( xMutex != NULL && xSemaphoreBC != NULL && xSemaphoreD != NULL )
    {
        xTaskCreate( vTaskA, "Task A", configMINIMAL_STACK_SIZE, NULL, TASK_A_PRIORITY, NULL );
        xTaskCreate( vTaskB, "Task B", configMINIMAL_STACK_SIZE, NULL, TASK_BC_PRIORITY, NULL );
        xTaskCreate( vTaskC, "Task C", configMINIMAL_STACK_SIZE, NULL, TASK_BC_PRIORITY, NULL );
        xTaskCreate( vTaskD, "Task D", configMINIMAL_STACK_SIZE, NULL, TASK_D_PRIORITY, NULL );

        vTaskStartScheduler();
    }
    for( ; ; )
    {
    }
}

static void vTaskA( void *pvParameters )
{
    for( ;; )
    {
        if( xSemaphoreTake( xMutex, portMAX_DELAY ) == pdTRUE )
        {
            UART_printf( "Task A\n" );
            xSemaphoreGive( xSemaphoreBC );
            xSemaphoreGive( xMutex );
        }
        vTaskDelay( xDelay );
    }
}

static void vTaskB( void *pvParameters )
{

    for( ;; )
    {
        if( xSemaphoreTake( xSemaphoreBC, portMAX_DELAY ) == pdTRUE )
        {
            if( xSemaphoreTake( xMutex, portMAX_DELAY ) == pdTRUE )
            {
                UART_printf( "Task B\n" );

                xSemaphoreGive( xSemaphoreD );

                xSemaphoreGive( xMutex );
            }
        }

        vTaskDelay( xDelay );
    }
}

static void vTaskC( void *pvParameters )
{

    for( ;; )
    {
        if( xSemaphoreTake( xSemaphoreBC, portMAX_DELAY ) == pdTRUE )
        {
            
            if( xSemaphoreTake( xMutex, portMAX_DELAY ) == pdTRUE )
            {
                UART_printf( "Task C\n" );

                xSemaphoreGive( xSemaphoreD );

                xSemaphoreGive( xMutex );
            }
        }

        vTaskDelay( xDelay );
    }
}

static void vTaskD( void *pvParameters )
{

    for( ;; )
    {
        if( xSemaphoreTake( xSemaphoreD, portMAX_DELAY ) == pdTRUE )
        {
            if( xSemaphoreTake( xMutex, portMAX_DELAY ) == pdTRUE )
            {
                UART_printf( "Task D\n" );

                xSemaphoreGive( xMutex );
            }
        }
        vTaskDelay( xDelay );
    }
}
