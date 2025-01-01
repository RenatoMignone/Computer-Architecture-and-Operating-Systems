# Timers
This project shows different wais to implement periodic operations in FreeRTOS.

Clone this repository using the following command:
```
git clone --recurse-submodules https://baltig.polito.it/-/ide/project/teaching-material/exercises-caos-and-os/timers
```
## Use of Delay Functions 

Create a Free RTOS project that runs a periodic main task named TaskA. This task emulates intensive CPU usage by performing busy waiting at each period.
Use the `xTaskGetTickCount()` funtion to measure the time passing in this task.  

The system need to run a periodic sampling function activated every 220 ns.

Implement this sampling function as and additional task that uses the `vTaskDelay()` to block the task between to activiations.

## Use of FreeRTOS software timer
Create a Free RTOS project that creates two software timers:

* A one shot timer activated after 2 sec,
* A periodic timer activated every second.

Both timer are associated to the same handler funtion that prints a different message depenging on the activated timer.

### Tips

In order to activate timers the following configurations are required:

**FreeRTOSConfig.h**
```
...
//EXERCISE CHANGE: You need to increase the heap size otherwise your app could fail silently
//when enabling timers
#define configTOTAL_HEAP_SIZE                    ( ( size_t ) ( 200 * 1024 ) )
...
//EXERCISE CHANGE: enable the timers.
#define configUSE_TIMERS                         1
#define configTIMER_TASK_PRIORITY                ( configMAX_PRIORITIES - 4 )
#define configTIMER_QUEUE_LENGTH                 20
#define configTIMER_TASK_STACK_DEPTH             ( configMINIMAL_STACK_SIZE * 2 )
...
```

**Makefile**
```
...
# Kernel files
SOURCE_FILES += $(KERNEL_DIR)/queue.c
SOURCE_FILES += $(KERNEL_DIR)/timers.c
...
```
