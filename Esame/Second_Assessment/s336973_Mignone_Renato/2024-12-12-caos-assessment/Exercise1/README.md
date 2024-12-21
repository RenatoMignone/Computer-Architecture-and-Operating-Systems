# Exercise 2- FreeRTOS and Priority Inversion

Create a FreeRTOS project based on the mps2-an385 emulated board in QEMU and the basic skeleton provided in this folder.

The base project provides hardware support based on the CMSIS Board Support Package. It implements the printf function and initializes the hardware timer 0 available on the board to issue periodic interrupts.

The project must create a periodic task named TaskA. The period of the task is 1 sec. At any activation the task prints on screen the number of times the task was activated.
The project must also set two timers.
The first timer is periodic with a period of 2 sec. Every time the timer fires it enables a high priority task that prints on screen a message.
The second timer is a one shot timer with period of 2 sec that prints on screen the word "GAME OVER" when activated.

Refer to the FreeRTOS documentation here: [https://www.freertos.org/Documentation/00-Overview]()








