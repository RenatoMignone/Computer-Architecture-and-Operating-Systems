# Exercise: Count Set Bits (Population Count)

## Objective:
Write an ARMv7 assembly program that counts the number of set bits (1s) in a 32-bit integer. The program should run bare-metal and use a function call to compute the population count.

## Problem Breakdown:
* Define a 32-bit integer in the .data section.
* Implement a function that counts the set bits in the integer.
* Call the function from the main program and store the result in a register.
* 


## Test the program:
Use the online emulator at [https://cpulator.01xz.net/?sys=arm](https://cpulator.01xz.net/?sys=arm)

## Explanation of the Code:

* The program defines a 32-bit integer in the .data section.
* The `count_set_bits` function uses a loop to check each bit in the integer. 
* The TST instruction is used to test if the least significant bit is set (1), and the count is incremented using ADDNE (Add if Not Equal).
* After processing all bits (when the number becomes zero), the function returns the count in R0.