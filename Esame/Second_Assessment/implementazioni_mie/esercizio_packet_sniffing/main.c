#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "stdio.h"
#include "uart.h"
#include "timers.h"
#include "stdlib.h"

#define mainTASK_PRIORITY (tskIDLE_PRIORITY + 2)
#define QUEUE_LENGTH 100

// Queue handles
QueueHandle_t Critical_queue;
QueueHandle_t Normal_queue;
QueueHandle_t Low_queue;

// Timer and semaphore handles
SemaphoreHandle_t Semaphore_Packet;
TimerHandle_t Sniffing_Timer;

// Function declarations
void Task_Sniffing(void *pvParameters);
void Task_Analyzer(void *pvParameters);
void myTimerCallback(TimerHandle_t xTimer);

// Packet structure
typedef struct {
    int ID;
    int priority;
    char content[100];  // Fixed-size array for simplicity
} Packet;

// Global variables
int Packets_ID = 0;

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

void myTimerCallback(TimerHandle_t xTimer) {
    Packet p;

    // Create a new packet
    p.ID = Packets_ID++;
    p.priority = (rand() % 3) + 1;  // Random priority between 1 and 3
    snprintf(p.content, sizeof(p.content), "Packet Content %d", p.ID);

    UART_printf("\nSniffing...\n");
    UART_printf("\nPacket ID: %d Priority: %d\n", p.ID, p.priority);

    // Wait to access the shared queues
    if (xSemaphoreTake(Semaphore_Packet, portMAX_DELAY) == pdTRUE) {
        // Enqueue the packet based on priority
        switch (p.priority) {
            case 1:
                xQueueSend(Critical_queue, &p, portMAX_DELAY);
                break;
            case 2:
                xQueueSend(Normal_queue, &p, portMAX_DELAY);
                break;
            case 3:
                xQueueSend(Low_queue, &p, portMAX_DELAY);
                break;
        }

        // Release the semaphore
        xSemaphoreGive(Semaphore_Packet);
    }
}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

void Task_Sniffing(void *pvParameters) {
    (void)pvParameters;

    // Start the sniffing timer
    xTimerStart(Sniffing_Timer, portMAX_DELAY);

    // Keep the task alive
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

void Task_Analyzer(void *pvParameters) {
    (void)pvParameters;
    Packet p;
    char buffer[150];

    // Allow some time for the system to stabilize
    vTaskDelay(pdMS_TO_TICKS(1000));

    while (1) {
        // Wait to access the shared queues
        if (xSemaphoreTake(Semaphore_Packet, portMAX_DELAY) == pdTRUE) {
            // Check queues in priority order
            if (xQueueReceive(Critical_queue, &p, 0) == pdPASS) {
                snprintf(buffer, sizeof(buffer), "\n[Critical] Packet ID: %d Priority: %d Content: %s\n",
                         p.ID, p.priority, p.content);
                UART_printf(buffer);
            } else if (xQueueReceive(Normal_queue, &p, 0) == pdPASS) {
                snprintf(buffer, sizeof(buffer), "\n[Normal] Packet ID: %d Priority: %d Content: %s\n",
                         p.ID, p.priority, p.content);
                UART_printf(buffer);
            } else if (xQueueReceive(Low_queue, &p, 0) == pdPASS) {
                snprintf(buffer, sizeof(buffer), "\n[Low] Packet ID: %d Priority: %d Content: %s\n",
                         p.ID, p.priority, p.content);
                UART_printf(buffer);
            }

            // Release the semaphore
            xSemaphoreGive(Semaphore_Packet);
        }

        // Delay to simulate processing time
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

void main(void) {
    // Initialize UART for debugging
    UART_init();

    // Create the queues
    Critical_queue = xQueueCreate(QUEUE_LENGTH, sizeof(Packet));
    Normal_queue = xQueueCreate(QUEUE_LENGTH, sizeof(Packet));
    Low_queue = xQueueCreate(QUEUE_LENGTH, sizeof(Packet));

    // Check if queues were created successfully
    if (Critical_queue == NULL || Normal_queue == NULL || Low_queue == NULL) {
        UART_printf("\nFailed to create queues. Halting system.\n");
        while (1);
    }

    // Create the semaphore
    Semaphore_Packet = xSemaphoreCreateBinary();
    if (Semaphore_Packet == NULL) {
        UART_printf("\nFailed to create semaphore. Halting system.\n");
        while (1);
    }

    // Initialize the semaphore to the "given" state
    xSemaphoreGive(Semaphore_Packet);

    // Create the sniffing timer
    Sniffing_Timer = xTimerCreate(
        "Sniffing Timer",
        pdMS_TO_TICKS(2000),  // Period of 2000 ms
        pdTRUE,               // Auto-reload timer
        NULL,                 // Timer ID not used
        myTimerCallback       // Callback function
    );

    // Check if the timer was created successfully
    if (Sniffing_Timer == NULL) {
        UART_printf("\nFailed to create timer. Halting system.\n");
        while (1);
    }

    // Create tasks
    xTaskCreate(Task_Sniffing, "Task_Sniffing", 1000, NULL, mainTASK_PRIORITY, NULL);
    xTaskCreate(Task_Analyzer, "Task_Analyzer", 1000, NULL, mainTASK_PRIORITY, NULL);

    // Start the scheduler
    vTaskStartScheduler();

    // Code should never reach here
    while (1);
}

