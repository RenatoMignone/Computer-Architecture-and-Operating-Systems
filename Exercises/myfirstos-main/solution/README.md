# MyFirstOS

Create an example of a simple flat operating system using fixed tasks running on an ARM Cortex-M4 board, emulated using QEMU.

## Requirements

The characteristics of the operating system are:

* The OS can run two tasks. Each task prints a message when it starts, performs a loop, and then prints a message before concluding its computation.
* Each task must have a dedicated stack. The OS is responsible for handling task switching.
* The OS schedules the two tasks by alternating their execution in an infinite loop.

Create a project with the related makefile to build the OS

## Makefile Explanation

This Makefile is used to compile and link an ARM Cortex-M4 project. It defines the toolchain, flags, and rules for building the project from assembly and C source files.

### Toolchain and Flags

- **Toolchain Definitions**:
  - `AS = arm-none-eabi-as`: Assembler for ARM architecture.
  - `CC = arm-none-eabi-gcc`: C compiler for ARM architecture.
  - `LD = arm-none-eabi-ld`: Linker for ARM architecture.

- **Compiler Flags**:
  - `CPU = -mcpu=cortex-m4`: Specifies the target CPU architecture.
  - `THUMB = -mthumb`: Indicates that the Thumb instruction set should be used.
  - `CFLAGS = -c $(CPU) $(THUMB)`: Compiler flags for generating object files without linking.
  - `LDFLAGS = -T linker.ld`: Linker flags, specifying the linker script (`linker.ld`).

### Object Files

- **OBJ**: 
  ```makefile
  OBJ = startup.o library.o tasks.o main.o 
  ```
  This variable lists the object files that will be created from the source files.

### Output File

- **TARGET**: 
  ```makefile
  TARGET = main.elf
  ```
  This variable defines the final output file name after linking.

### Default Target

- **Default Target**: 
  ```makefile
  all: $(TARGET)
  ```
  This is the default target that will be built when you run `make`. It depends on the `$(TARGET)`.

### Build Rules

#### Rule for `startup.o`
- **Creating `startup.o`**:
  ```makefile
  startup.o: startup.s
	$(AS) $(CPU) startup.s -o startup.o
  ```
  This rule compiles the assembly file `startup.s` into an object file `startup.o` using the assembler.

#### Rule for `main.o`
- **Creating `main.o`**:
  ```makefile
  main.o: main.c
	$(CC) $(CFLAGS) main.c -o main.o
  ```
  This rule compiles the C source file `main.c` into an object file `main.o`.

#### Rule for `library.o`
- **Creating `library.o`**:
  ```makefile
  library.o: library.c
	$(CC) $(CFLAGS) library.c -o library.o
  ```
  This rule compiles the C source file `library.c` into an object file `library.o`.

#### Rule for `tasks.o`
- **Creating `tasks.o`**:
  ```makefile
  tasks.o: tasks.c
	$(CC) $(CFLAGS) tasks.c -o tasks.o
  ```
  This rule compiles the C source file `tasks.c` into an object file `tasks.o`.

#### Linking Object Files
- **Linking to create the final ELF file**:
  ```makefile
  $(TARGET): $(OBJ)
	$(LD) $(LDFLAGS) $(OBJ) -o $(TARGET)
  ```
  This rule links all object files listed in `$(OBJ)` to create the final executable file `main.elf`.

### Cleanup Rule

- **Cleaning up generated files**:
  ```makefile
  clean:
	rm -f $(OBJ) $(TARGET)
  ```
  This rule defines the `clean` target, which removes all generated object files and the final executable. This is useful for starting a fresh build.



## Main.c Explenation

### Includes and Defines

```c
#include <stdint.h>
#define STACK_SIZE 4096 
```
- `#include <stdint.h>`: Includes the standard integer types header, which allows the use of fixed-width integer types.
- `#define STACK_SIZE 4096`: Defines the size of the stack for each task as 4096 bytes.

### Stack Definition

```c
uint32_t stack_task1[STACK_SIZE];
uint32_t stack_task2[STACK_SIZE];
```
- Two stacks are defined for two tasks, `stack_task1` and `stack_task2`, each with a size of `STACK_SIZE`.

### Stack Pointers

```c
uint32_t *sp_task1 = &stack_task1[STACK_SIZE - 1];
uint32_t *sp_task2 = &stack_task2[STACK_SIZE - 1];
```
- Two stack pointers, `sp_task1` and `sp_task2`, are declared and initialized to point to the top of their respective stacks (the last element in the array).

### External Functions

```c
extern void my_printf(const char *s); 
extern void task1(void);
extern void task2(void);
```
- `my_printf`: An external function used for printing messages (implementation not shown).
- `task1` and `task2`: External function prototypes for the two tasks.

### Task Switching Function

- `switch_task`: This function performs the task switching using inline assembly. It saves the current stack pointer and sets the stack pointer to the next task's stack.


### Main Function

- The `main` function initializes a pointer `current_sp` for the current stack pointer. It enters an infinite loop that alternates between `task1` and `task2` by calling `switch_task` before each task call. This simulates a round-robin task scheduler, although the tasks themselves would loop indefinitely in this basic example.


