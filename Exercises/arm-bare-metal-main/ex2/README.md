# Exercise: Sum of Array using ARMv7 Assembly (Function Call)

## Objective:
Write an ARMv7 assembly program that computes the sum of an array of integers using a function call. The program should execute in a bare-metal environment and output the result (for example, via a memory-mapped I/O register like a UART).

## Problem Breakdown:
* Define an array of integers in the .data section of the program.
* Implement a function that takes the array's address and length as arguments, computes the sum of the array elements, and returns the result.
* Call the function from the main program to compute the sum and store the result in a register.
* Output the result to a memory-mapped I/O register, such as a UART (if available in the bare-metal setup), or store the result in memory.

## Program Requirements:

* Use ARMv7 assembly instructions.
* * The sum function should use function calling conventions: passing arguments through registers (e.g., R0 for the address, R1 for the array length) and returning the result in R0.
* The program should run bare-metal (no OS).

## Test the program:
Use the online emulator at [https://cpulator.01xz.net/?sys=arm](https://cpulator.01xz.net/?sys=arm)

## Explanation of the Code:

### Data Section:

The array array is defined in the .data section and contains five integers (1, 2, 3, 4, 5). This can be changed to test different arrays.

### Main Program (_start):

The program starts by loading the base address of the array into register R0 and the array's length into R1.

The BL (Branch with Link) instruction calls the `sum_array` function, which computes the sum of the array elements.

After the function completes, the sum is stored in R0. 

In a bare-metal scenario, the result could be displayed using UART (or other output mechanisms depending on the target hardware), but here we loop infinitely for simplicity.


### Function (sum_array):
The sum_array function takes two arguments:

* R0: the address of the array.
* R1: the length of the array.

Inside the function, we initialize two registers:

* R2: This stores the accumulated sum.
* R3: This is used as the index into the array.

The function iterates through the array using a loop (sum_loop), where it:

* Loads each element from memory (LDR R4, [R0, R3, LSL #2]).
* Adds the element to the sum.
* Increments the index.

Once all elements are processed, the sum is returned in R0, and the function exits using BX LR (branch to link register, returning to the caller).



