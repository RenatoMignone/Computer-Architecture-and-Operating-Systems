# Exercise 2- FreeRTOS and Priority Inversion

Create a FreeRTOS project based on the mps2-an385 emulated board in QEMU and the basic skeleton provided in this folder.

The base project provides hardware support based on the CMSIS Board Support Package. It implements the printf function and initializes the hardware timer 0 available on the board to issue periodic interrupts.

The project must create four  periodic tasks (A, B, C, and D) with a period of 20ms. At every period each task simply prints a message on screen, and the print operation represents a critical section for the task. By using a set of sincronization primitives make sure the critical sections are constinuosly executed in the following order:
- Task A
- Tas B or Tasc C (only one of the two)
- Task D 


Refer to the FreeRTOS documentation here: [https://www.freertos.org/Documentation/00-Overview]()








