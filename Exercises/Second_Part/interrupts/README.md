# Interrupts

This project shows the use of interrupts in FreeRTOS. 

It programs two interrupt handlers for the two timers in MPS2 QEMU (Arm Cortex-M3) emulated board.

It defines a main TASK performing CPU intensive operations.

Finally, it shows how to implement a deferred interrupt using a semaphore and how interrupts can be nested.
