#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

//------------------
#include "uart.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
//------------------    

#define mainTASK_PRIORITY    ( tskIDLE_PRIORITY + 2 )
#define QUEUE_LENGTH 5


///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
void OrderPickerTask(void *pvParameters);
void OrderPackerTask(void *pvParameters);
void OrderShipperTask(void *pvParameters);
void OrderLoggerTask(void *pvParameters);


///////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
QueueHandle_t xQueue_Item1;
QueueHandle_t xQueue_Item2;
QueueHandle_t xQueue_Item3;
QueueHandle_t xMessageQueue1;
QueueHandle_t xMessageQueue2;

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
typedef struct{
        int index;
	    char *details;
	    //int priority;
        }Item;

typedef struct{
    int index;
    char message[100];
}Log_Message;

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
int main(int argc, char **argv){
	(void) argc;
	(void) argv;
    UART_init();
    xQueue_Item1 = xQueueCreate(QUEUE_LENGTH, sizeof(Item));
    xQueue_Item2 = xQueueCreate(QUEUE_LENGTH, sizeof(Item));
    xQueue_Item3 = xQueueCreate(QUEUE_LENGTH, sizeof(Item));
    xMessageQueue1 = xQueueCreate(100,sizeof(Log_Message));
    xMessageQueue2 = xQueueCreate(100,sizeof(Log_Message));

    for(int i=0;i<QUEUE_LENGTH;i++){
        Item tmp;
        tmp.index = i;
        xQueueSend(xQueue_Item1, &tmp, portMAX_DELAY);
    }
        
    xTaskCreate(OrderPickerTask, "OrderPicker", 1000, NULL, mainTASK_PRIORITY+1, NULL);
    xTaskCreate(OrderPackerTask, "OrderPacker", 1000, NULL, mainTASK_PRIORITY+1, NULL);
    xTaskCreate(OrderShipperTask, "OrderShipper", 1000, NULL, mainTASK_PRIORITY+1, NULL);
    xTaskCreate(OrderLoggerTask, "OrderLogger", 1000, NULL, mainTASK_PRIORITY+1, NULL);


    vTaskStartScheduler();
    while (1);
}


///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
void OrderPickerTask(void *pvParameters){
	(void) pvParameters;
    Item tmp;
    Log_Message message;
    while(1){
            xQueueReceive(xQueue_Item1,&tmp,portMAX_DELAY);
            xQueueSend(xQueue_Item2,&tmp,portMAX_DELAY);

            sprintf(message.message, "Order Picker: taking the packet %d",tmp.index);
            xQueueSend(xMessageQueue1,&message,portMAX_DELAY);

            //UART_printf("\nOrder Picker: taking the packet %d...\n",tmp.index);
            //UART_printf("Order Picker: Processing the packet %d...\n",tmp.index);
        
            vTaskDelay(pdMS_TO_TICKS(1000));

            sprintf(message.message, "Order Picker: sending the packet %d",tmp.index);
            xQueueSend(xMessageQueue1,&message,portMAX_DELAY);

            sprintf(message.message, "Order Picker endend his task...");
            xQueueSend(xMessageQueue1,&message,portMAX_DELAY);

            //UART_printf("Order Picker: sending the packet %d...\n",tmp.index);
            //UART_printf("Order Picker endend his task...\n\n",tmp.index);
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
}

void OrderPackerTask(void *pvParameters){
	(void) pvParameters;
    Item tmp;
    Log_Message message;
    while(1){
            //UART_printf("\nOrder Packer: taking the item %d...\n",tmp.index);
            xQueueReceive(xQueue_Item2,&tmp,portMAX_DELAY);
            xQueueSend(xQueue_Item3,&tmp,portMAX_DELAY);

            sprintf(message.message, "Order Packer: Packing the packet %d",tmp.index);
            xQueueSend(xMessageQueue1,&message,portMAX_DELAY);

            //UART_printf("Order Packer: Packing the packet %d...\n",tmp.index);
            vTaskDelay(pdMS_TO_TICKS(1000));
            sprintf(message.message,"Order Packer: sending the packet %d",tmp.index);
            xQueueSend(xMessageQueue1,&message,portMAX_DELAY);

            sprintf(message.message, "Order Packer endend his task...");
            xQueueSend(xMessageQueue1,&message,portMAX_DELAY);
            //UART_printf("Order Packer: sending the packet %d...\n",tmp.index);
            //UART_printf("Order Packer endend his task...\n\n",tmp.index);
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
}

void OrderShipperTask(void *pvParameters){
	(void) pvParameters;
    Item tmp;
    Log_Message message;
    while(1){

            xQueueReceive(xMessageQueue1,&message,portMAX_DELAY);
            xQueueSend(xMessageQueue2,&message,portMAX_DELAY);

            xQueueReceive(xQueue_Item3,&tmp,portMAX_DELAY);
            sprintf(message.message, "Order Scipper: taking the item %d",tmp.index);
            xQueueSend(xMessageQueue2,&message,portMAX_DELAY);
            
            sprintf(message.message, "Order Scipper ended the task ...");
            xQueueSend(xMessageQueue2,&message,portMAX_DELAY);


        }
}


void OrderLoggerTask(void *pvParameters){
	(void) pvParameters;
    Log_Message message;
    while(1){
        xQueueReceive(xMessageQueue2,&message,portMAX_DELAY);
        UART_printf("\nLogger message:");
        UART_printf(message.message);
        UART_printf("\n");
    }

}
