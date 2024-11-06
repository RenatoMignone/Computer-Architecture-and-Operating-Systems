# MyFirstOS

Create an example of a simple flat operating system using fixed tasks running on an ARM Cortex-M4 board, emulated using QEMU.

## Requirements

The characteristics of the operating system are:

* The OS can run two tasks. Each task prints a message when it starts, performs a loop, and then prints a message before concluding its computation.
* Each task must have a dedicated stack. The OS is responsible for handling task switching.
* The OS schedules the two tasks by alternating their execution in an infinite loop.

Create a project with the related makefile to build the OS

## Skeleton Project

**Compile and run the program:**

* `startup.s` baremetal startup file
* `library.c` library of functions
* `main.c` main program printing "hello world"

**Compile and run the program:**
- To compile the skeleton use the following command:
    ```bash
    arm-none-eabi-as -mcpu=cortex-m4 startup.s -o startup.o
    arm-none-eabi-gcc -c -mcpu=cortex-m4 -mthumb library.c -o library.o
    arm-none-eabi-gcc -c -mcpu=cortex-m4 -mthumb main.c -o main.o
    arm-none-eabi-ld -T linker.ld startup.o library.o main.o -o main.elf
    ```
- To run the program in QEMU use the following command

    ```bash
    qemu-system-arm -M netduino2 -nographic -kernel main.elf
    ```
Now the program should be running on QEMU. You should see the message "Hello world!" printed on the
terminal. The process will be in an infinite loop. To end the simulation, press `Ctrl + A` and then
`X`.
