#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
//------------------
#include "uart.h"
#include <stdio.h> 
//------------------

#define mainTASK_PRIORITY    ( tskIDLE_PRIORITY + 2 )

typedef struct {
    int orderID;
    char itemDetails[50];
} Order;

QueueHandle_t xOrderQueue;
QueueHandle_t xShippingQueue;

void OrderPickerTask(void *pvParameters) {
    int orderID = 1; // Simulated order IDs
    while (1) {
        Order newOrder;
        newOrder.orderID = orderID++;
        snprintf(newOrder.itemDetails, sizeof(newOrder.itemDetails), "Item %d", newOrder.orderID);

        if (xQueueSend(xOrderQueue, &newOrder, portMAX_DELAY) == pdPASS) {
            UART_printf("Order Picker: Picked Order ID %d\n", newOrder.orderID);
        } else {
            UART_printf("Order Picker: Queue Full, Unable to Pick Order\n");
        }

        vTaskDelay(pdMS_TO_TICKS(2000)); // Simulate picking time
    }
}

void PackingTask(void *pvParameters) {
    Order receivedOrder;
    while (1) {
        if (xQueueReceive(xOrderQueue, &receivedOrder, portMAX_DELAY) == pdPASS) {
            UART_printf("Packing Station: Packing Order ID %d\n", receivedOrder.orderID);
            vTaskDelay(pdMS_TO_TICKS(1500)); // Simulate packing time

            if (xQueueSend(xShippingQueue, &receivedOrder, portMAX_DELAY) == pdPASS) {
                UART_printf("Packing Station: Packed Order ID %d\n", receivedOrder.orderID);
            } else {
                UART_printf("Packing Station: Shipping Queue Full, Unable to Send Order\n");
            }
        } else {
            UART_printf("Packing Station: No Orders to Pack\n");
        }
    }
}

void ShippingTask(void *pvParameters) {
    Order packedOrder;
    while (1) {
        if (xQueueReceive(xShippingQueue, &packedOrder, portMAX_DELAY) == pdPASS) {
            UART_printf("Shipping Controller: Shipping Order ID %d\n", packedOrder.orderID);
            vTaskDelay(pdMS_TO_TICKS(2000)); // Simulate shipping time
        } else {
            UART_printf("Shipping Controller: No Orders to Ship\n");
        }
    }
}

void LoggerTask(void *pvParameters) {
    while (1) {
        UART_printf("Logger: System is running smoothly\n");
        vTaskDelay(pdMS_TO_TICKS(5000)); // Periodic logging
    }
}

int main(int argc, char **argv){

	(void) argc;
	(void) argv;

    UART_init();

	xOrderQueue = xQueueCreate(5, sizeof(Order)); // Queue for 5 orders
    xShippingQueue = xQueueCreate(5, sizeof(Order)); // Queue for 5 packed orders

    if (xOrderQueue == NULL || xShippingQueue == NULL) {
        UART_printf("Failed to create queues\n");
        return -1;
    }

    xTaskCreate(OrderPickerTask, "OrderPicker", 1000, NULL, mainTASK_PRIORITY+1, NULL);
    xTaskCreate(PackingTask, "PackingStation", 1000, NULL, mainTASK_PRIORITY+1, NULL);
    xTaskCreate(ShippingTask, "ShippingController", 1000, NULL, mainTASK_PRIORITY+1, NULL);
    xTaskCreate(LoggerTask, "Logger", 1000, NULL, mainTASK_PRIORITY, NULL);

    vTaskStartScheduler();
    while (1);
}
