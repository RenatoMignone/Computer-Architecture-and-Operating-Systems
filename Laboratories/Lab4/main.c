#include "FreeRTOS.h"
#include "task.h"
#include "uart.h"
#include "queue.h"

#define mainTASK_PRIORITY    ( tskIDLE_PRIORITY + 2 )

static int seed;

void intToString(int num, char* str);
int my_rand();
int my_rand_range(int min, int max);
void vTaskFunction(void *pvParameters);

void vTaskFunction_Producer(void *pvParameters);
void vTaskFunction_Consumer(void *pvParameters);


int main(int argc, char **argv){

  (void) argc;
  (void) argv;

  UART_init();

  QueueHandle_t queue_handler = xQueueCreate(100, sizeof(int));


  xTaskCreate(
    // Function which implements the task
    vTaskFunction_Producer,
    // Name of the task (debug purposes, not used by the kernel)
    "Producer",
    // Stack to allocate to the task
    configMINIMAL_STACK_SIZE,
    // Parameter passed to the task. Not needed for Hello World example
    queue_handler,
    // Priority assigned to the task
    mainTASK_PRIORITY,
    // Task handle. Not required
    NULL
  );

  xTaskCreate(
    // Function which implements the task
    vTaskFunction_Consumer,
    // Name of the task (debug purposes, not used by the kernel)
    "Consumer",
    // Stack to allocate to the task
    configMINIMAL_STACK_SIZE,
    // Parameter passed to the task. Not needed for Hello World example
    queue_handler,
    // Priority assigned to the task
    mainTASK_PRIORITY,
    // Task handle. Not required
    NULL
  );


  // Give control to the scheduler
  vTaskStartScheduler();

  // If everything ok should never reach here
  for( ; ; );
}



////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/* Support function */
void intToString(int num, char* str) {
  // Handle the case of zero explicitly
  if (num == 0) {
    str[0] = '0';
    str[1] = '\0'; // Null-terminate the string
    return;
  }

  // Determine if the number is negative
  int isNegative = 0;
  if (num < 0) {
    isNegative = 1;
    num = -num; // Make the number positive for easier handling
  }

  // Temporary variable to hold the string in reverse
  char temp[12]; // Enough to hold all digits of int including sign and null
  int index = 0;

  // Extract digits and store them in reverse order
  while (num > 0) {
    temp[index++] = (num % 10) + '0'; // Convert digit to character
    num /= 10; // Reduce num by a factor of 10
  }

  // Add the negative sign if the number was negative
  if (isNegative) {
    temp[index++] = '-';
  }

  // Null-terminate the string
  temp[index] = '\0';

  // Reverse the string into the output buffer
  for (int i = 0; i < index; i++) {
    str[i] = temp[index - 1 - i];
  }
  str[index] = '\0'; // Null-terminate the output string
}







////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
// Function to initialize the seed with the current tick count
void init_seed() {
  seed = xTaskGetTickCount(); // Initialize the seed with the system tick count
}

// Linear Congruential Generator (LCG)
int my_rand() {
  seed = (seed * 1103515245 + 12345) & 0x7fffffff;
  return seed;
}

// Function to get a random number in a specific range
int my_rand_range(int min, int max) {
  return min + (my_rand() % (max - min + 1));
}











////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
/* Task Function */
void vTaskFunction_Producer(void *pvParameters) {

  // Avoid warning about unused pvParameters
  (void) pvParameters;

  QueueHandle_t myQueue = (QueueHandle_t)pvParameters;

  char buffer0[20];
  char buffer1[20];


  for (;;) {

    for (int i=0; i<100; i++) {
      int rand_num = my_rand_range(0, 999);

      xQueueSend( myQueue , &rand_num , 0);

      intToString(rand_num, buffer0);
      intToString(i, buffer1);
      UART_printf("Rand num inserted in the queue:");
      UART_printf(buffer1);
      UART_printf(" : ");
      UART_printf(buffer0);
      UART_printf("\n");

      vTaskDelay(pdMS_TO_TICKS(300));
    }
    vTaskDelete(NULL);

  }
}

void vTaskFunction_Consumer(void *pvParameters) {

  (void) pvParameters;

  int value;
  char buffer0[20];
  char buffer1[20];
  int counter = 0;

  for (;;) {
    //vTaskDelay(pdMS_TO_TICKS(1));


    QueueHandle_t myQueue = (QueueHandle_t)pvParameters;

    for (int i=0; i<100; i++) {

      xQueueReceive( myQueue, &value, 0  );

      intToString(value, buffer0);
      intToString(i, buffer1);
      UART_printf("Rand num readed from the queue: ");
      UART_printf(buffer1);
      UART_printf(" : ");
      UART_printf(buffer0);
      UART_printf("\n");

      if(value != 0 && value % 7 == 0){
        UART_printf(buffer0);
        UART_printf(" is a multiple of 7");
        counter++;
      }

      UART_printf("\n");
      UART_printf("\n");
      
      vTaskDelay(pdMS_TO_TICKS(300));
    }
    intToString(counter, buffer0);
    UART_printf("Number of numbers multiple of 7: ");
    UART_printf(buffer0);

    vTaskDelete(NULL);
  }
}

