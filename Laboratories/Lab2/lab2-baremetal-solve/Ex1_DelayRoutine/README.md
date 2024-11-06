## Delay Routine using Polling Mode

In this exercise, you will implement a delay routine using a polling mechanism and learn how to write inline assembly code in a C program.

### What is a Delay Routine?

A delay routine is a function that causes the processor to wait for a specified period. This can be useful when you need to wait for a peripheral to be ready or create a delay between two events. There are two main ways to implement a delay routine: using a polling mechanism or a timer. In this exercise, you will implement a delay routine using the polling mechanism.

The polling mechanism involves repeatedly checking a condition. In the case of a delay routine, you create a loop that is executed a certain number of times to achieve the desired delay. If one loop iteration is not sufficient to create the needed delay, you can increase the number of iterations by nesting multiple loops.

### How to Implement the Delay Routine

To implement the delay routine, you need the following information:
1. The clock frequency of the board.
2. The number of clock cycles each loop iteration takes.
3. The desired delay time.

With this information, you can calculate the number of iterations required to achieve the desired delay time.

**Loop Counter = (Delay [s] × Clock Frequency [Hz]) / Clock Cycles per Iteration**

### How to Calculate the Number of Clock Cycles per Iteration

To calculate the number of clock cycles per iteration, you need to know how many instructions are inside your loop. This is why it’s important to implement the delay routine in **assembly**, as you have full control over the instructions being executed.

In class, you should have learned how many clock cycles each instruction takes to execute. For example, imagine a pipeline processor where each instruction takes 1 clock cycle to execute, and your delay routine includes the following instructions:

```assembly
loop:
    inst 1
    inst 2
    inst 3
    inst 4
    inst 5
    bne loop
```
The number of clock cycles per iteration would be 6, as you have 5 instructions and 1 branch instruction. With this information, you can calculate the number of iterations needed to achieve the desired delay time.

### Delay Routine Implementation

To implement the delay routine, create two files: `delay.h` and `delay.c`. The `delay.h` file will contain the declaration of the delay routine, while the `delay.c` file will contain the main code. Your C file should meet the following requirements:

- The delay routine should follow this prototype:

```c
void delay_routine_1(void);
```
- The delay routine should be implemented in assembly using the inline assembly feature of the C language, assuming the delay counter is stored in the r1 register.
- Inline assembly code can be included using the asm() directive, as shown below:
```c
asm("assembly code \n"
    "assembly code \n"
    );
```
**TIPS:**
- Use the `bne` or `beq` instruction to create a loop.
- Use the `subs` or `adds` instruction to decrement or increment the delay counter (`r1`).

- After implementing the delay routine, compile the code using the `arm-none-eabi-gcc` compiler as shown in the previous lab. It should compile without errors, and you should produce a `delay.o` file.
- Next, test the delay routine with QEMU and GDB.
- Copy the `linker.ld` and `startup.s` files from the previous lab to your current directory.
- Note that the `startup.s` file contains a `bl main` instruction that calls the `main` function. Since we do not have the `main` function yet, this will create an error. To fix this, comment out or remove the `bl main` instruction in the `startup.s` file.
- To call the delay routine, use the `bl delay_routine_1` instruction.
- Don’t forget to set the delay counter in the `r1` register before calling the delay routine. Avoid using large values for the delay counter, as this will take a long time to debug.

- You can now test your code with QEMU and GDB, as shown in the previous lab.

### Linking with C Code

Now, let’s create another delay routine that will be called from a `main` function. Add the following prototype to your `delay.h` file:

```c
void delay_routine_2(unsigned int delay_counter);
```
- Implement the new routine in the`delay.c` file. This delay routine should be written in assembly
using the inline assembly feature of C, this time assuming that the delay counter is stored in the
r0 register.

- Create a new file called `main.c`, which will contain the main function. The main function should be
written in C and call delay_routine_2 with a delay counter parameter.

- Compile all your C files using the arm-none-eabi-gcc compiler as shown in the previous lab. It
should compile without errors, producing `delay.o` and `main.o` files. 
- In the `startup.s` file, after your first delay routine call, you can call the main function using
  the `bl main` instruction.

- Now, assemble your code using the arm-none-eabi-as command. Then link your code with the
arm-none-eabi-ld command:
```bash
arm-none-eabi-ld -T linker.ld startup.o delay.o main.o -o delay.elf
```

Finally, you can test your code with QEMU and GDB, as shown in the previous lab.
- Use the `layout asm` command to display the assembly code and its address. 

You should observe that first the delay_routine_1 is called, then you should see your code jump to
the main function and call delay_routine_2. Something important to note is that the delay_routine_2
has a parameter, which is passed in the r0 register. This is the typical way to pass parameters to a
function in ARM assembly. If you have more than one parameter they will be passed in r0, r1 and r2.
If you have more than three parameters, they will be passed on the stack.

[Next: Create your Makefile](../Ex2_Makefile/README.md)
[Home](../README.md)
