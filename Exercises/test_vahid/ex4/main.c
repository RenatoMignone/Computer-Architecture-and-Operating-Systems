#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "stdio.h"
#include "uart.h"

typedef enum {
    SENSOR_TRAFFIC,
    SENSOR_EMERGENCY
} SensorType;

typedef struct {
    SensorType type;
    int data; // Traffic density or emergency vehicle distance
} SensorMessage;

QueueHandle_t xSensorQueue;
SemaphoreHandle_t xPedestrianSemaphore;
SemaphoreHandle_t xMutexLog;

void TrafficLightControllerTask(void *pvParameters) {
    while (1) {
        UART_printf("Traffic Light: Green for vehicles\n");
        vTaskDelay(pdMS_TO_TICKS(5000));

        UART_printf("Traffic Light: Yellow for vehicles\n");
        vTaskDelay(pdMS_TO_TICKS(2000));

        UART_printf("Traffic Light: Red for vehicles\n");

        if (xSemaphoreTake(xPedestrianSemaphore, 0) == pdTRUE) {
            UART_printf("Traffic Light: Pausing for pedestrians\n");
            vTaskDelay(pdMS_TO_TICKS(5000)); // Pedestrian crossing time
        }

        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}

void PedestrianCrossingTask(void *pvParameters) {
    while (1) {
        UART_printf("Pedestrian Crossing: Button pressed\n");
        xSemaphoreGive(xPedestrianSemaphore);
        vTaskDelay(pdMS_TO_TICKS(10000)); // Simulate button press delay
    }
}

void EmergencyVehicleTask(void *pvParameters) {
    SensorMessage message;
    while (1) {
        if (xQueueReceive(xSensorQueue, &message, portMAX_DELAY) == pdPASS) {
            if (message.type == SENSOR_EMERGENCY && message.data == 1) {
                UART_printf("Emergency Vehicle: Clearing traffic\n");
                // Simulate clearing traffic
                vTaskDelay(pdMS_TO_TICKS(7000));
                UART_printf("Emergency Vehicle: Traffic restored\n");
            }
        }
    }
}

void CentralMonitorTask(void *pvParameters) {
    while (1) {
        if (xSemaphoreTake(xMutexLog, portMAX_DELAY) == pdTRUE) {
            UART_printf("Central Monitor: System running smoothly\n");
            xSemaphoreGive(xMutexLog);
        }
        vTaskDelay(pdMS_TO_TICKS(3000));
    }
}

void SensorTask(void *pvParameters) {
    SensorMessage message;
    while (1) {
        // Simulate random sensor updates
        message.type = rand() % 2 ? SENSOR_TRAFFIC : SENSOR_EMERGENCY;
        message.data = rand() % 2; // Random data (0 or 1)

        if (xQueueSend(xSensorQueue, &message, portMAX_DELAY) == pdPASS) {
            UART_printf("Sensor Task: Sent update - Type: %d, Data: %d\n", message.type, message.data);
        }

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

int main(void) {

     UART_init();

    xSensorQueue = xQueueCreate(10, sizeof(SensorMessage));
    xPedestrianSemaphore = xSemaphoreCreateBinary();
    xMutexLog = xSemaphoreCreateMutex();

    if (xSensorQueue == NULL || xPedestrianSemaphore == NULL || xMutexLog == NULL) {
        UART_printf("Failed to create queues or semaphores\n");
        return -1;
    }

    xTaskCreate(TrafficLightControllerTask, "TrafficLightController", 1000, NULL, 2, NULL);
    xTaskCreate(PedestrianCrossingTask, "PedestrianCrossing", 1000, NULL, 2, NULL);
    xTaskCreate(EmergencyVehicleTask, "EmergencyVehicle", 1000, NULL, 3, NULL);
    xTaskCreate(CentralMonitorTask, "CentralMonitor", 1000, NULL, 1, NULL);
    xTaskCreate(SensorTask, "Sensor", 1000, NULL, 2, NULL);

    vTaskStartScheduler();
    while (1);
}
