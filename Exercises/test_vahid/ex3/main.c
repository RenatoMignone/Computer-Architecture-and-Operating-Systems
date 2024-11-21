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

// Data Structures
typedef struct {
    int packetID;
    char packetData[256]; // Raw packet data
    int priority;         // Critical = 3, Normal = 2, Low = 1
} NetworkPacket;

typedef struct {
    int threatID;
    int severity;         // 1 = Low, 2 = Medium, 3 = High
    char action[50];      // Suggested action (e.g., "Monitor", "Quarantine", "Shutdown")
    char metadata[256];   // Additional info about the flagged threat
} ThreatInfo;

// Queues and Synchronization Objects
QueueHandle_t criticalQueue, normalQueue, lowQueue;
QueueHandle_t threatQueue;
SemaphoreHandle_t quarantineSemaphore;
SemaphoreHandle_t databaseMutex;

// High-Speed Packet Sniffer Task
void PacketSnifferTask(void *pvParameters) {
    int packetID = 1;
    while (1) {
        NetworkPacket packet;
        packet.packetID = packetID++;
        snprintf(packet.packetData, sizeof(packet.packetData), "Packet %d Data", packet.packetID);
        packet.priority = rand() % 3 + 1; // Random priority (1 = Low, 2 = Normal, 3 = Critical)

        // Choose the queue based on priority
        QueueHandle_t targetQueue = (packet.priority == 3) ? criticalQueue :
                                     (packet.priority == 2) ? normalQueue :
                                     lowQueue;

        if (xQueueSend(targetQueue, &packet, portMAX_DELAY) != pdPASS) {
            UART_printf("Sniffer: Queue Full, Dropped Packet ID %d\n", packet.packetID);
        } else {
            UART_printf("Sniffer: Captured Packet ID %d, Priority %d\n", packet.packetID, packet.priority);
        }

        vTaskDelay(pdMS_TO_TICKS(10)); // Simulate packet arrival rate
    }
}

// Threat Analyzer Task
void ThreatAnalyzerTask(void *pvParameters) {
    QueueHandle_t inputQueue = (QueueHandle_t)pvParameters;
    NetworkPacket packet;

    while (1) {
        if (xQueueReceive(inputQueue, &packet, portMAX_DELAY) == pdPASS) {
            UART_printf("Analyzer: Received Packet ID %d\n", packet.packetID);

            // Access shared threat database with mutex
            xSemaphoreTake(databaseMutex, portMAX_DELAY);
            // Simulate threat analysis
            UART_printf("Analyzer: Processing Packet ID %d\n", packet.packetID);
            xSemaphoreGive(databaseMutex);

            ThreatInfo threat;
            threat.threatID = packet.packetID;
            threat.severity = rand() % 3 + 1; // Random severity level
            snprintf(threat.action, sizeof(threat.action), "Action for Packet %d", packet.packetID);
            snprintf(threat.metadata, sizeof(threat.metadata), "Analyzed data for Packet %d", packet.packetID);

            if (xQueueSend(threatQueue, &threat, portMAX_DELAY) != pdPASS) {
                UART_printf("Analyzer: Threat Queue Full, Dropped Threat ID %d\n", threat.threatID);
            } else {
                UART_printf("Analyzer: Flagged Threat ID %d, Severity %d\n", threat.threatID, threat.severity);
            }
        }
    }
}

// Response Handler Task
void ResponseHandlerTask(void *pvParameters) {
    ThreatInfo threat;

    while (1) {
        if (xQueueReceive(threatQueue, &threat, portMAX_DELAY) == pdPASS) {
            if (threat.severity == 1) {
                UART_printf("Response: Monitoring Threat ID %d\n", threat.threatID);
            } else if (threat.severity == 2) {
                if (xSemaphoreTake(quarantineSemaphore, pdMS_TO_TICKS(1000)) == pdPASS) {
                    UART_printf("Response: Quarantined Threat ID %d\n", threat.threatID);
                    xSemaphoreGive(quarantineSemaphore);
                } else {
                    UART_printf("Response: Failed to Quarantine Threat ID %d\n", threat.threatID);
                }
            } else if (threat.severity == 3) {
                UART_printf("Response: Shutdown Triggered for Threat ID %d\n", threat.threatID);
            }
        }
    }
}

// Logger Task
void LoggerTask(void *pvParameters) {
    while (1) {
        UART_printf("Logger: System Operational\n");
        vTaskDelay(pdMS_TO_TICKS(5000)); // Periodic logging
    }
}

// Main Function
int main(void) {

    UART_init();
    // Create Queues
    criticalQueue = xQueueCreate(10, sizeof(NetworkPacket));
    normalQueue = xQueueCreate(10, sizeof(NetworkPacket));
    lowQueue = xQueueCreate(10, sizeof(NetworkPacket));
    threatQueue = xQueueCreate(10, sizeof(ThreatInfo));

    // Create Semaphores
    quarantineSemaphore = xSemaphoreCreateBinary();
    databaseMutex = xSemaphoreCreateMutex();

    // Initialize Semaphore
    xSemaphoreGive(quarantineSemaphore);

    // Verify Queue and Semaphore Creation
    if (!criticalQueue || !normalQueue || !lowQueue || !threatQueue || !quarantineSemaphore || !databaseMutex) {
        UART_printf("System Initialization Failed\n");
        return -1;
    }

    // Create Tasks
    xTaskCreate(PacketSnifferTask, "Sniffer", 1000, NULL, 3, NULL);
    xTaskCreate(ThreatAnalyzerTask, "CriticalAnalyzer", 1000, (void *)criticalQueue, 2, NULL);
    xTaskCreate(ThreatAnalyzerTask, "NormalAnalyzer", 1000, (void *)normalQueue, 2, NULL);
    xTaskCreate(ThreatAnalyzerTask, "LowAnalyzer", 1000, (void *)lowQueue, 2, NULL);
    xTaskCreate(ResponseHandlerTask, "ResponseHandler", 1000, NULL, 2, NULL);
    xTaskCreate(LoggerTask, "Logger", 1000, NULL, 1, NULL);

    // Start Scheduler
    vTaskStartScheduler();

    // Should never reach here
    while (1);
    return 0;
}
