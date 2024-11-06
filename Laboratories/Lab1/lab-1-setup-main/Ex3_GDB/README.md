## Exercise #3: GDB Debugging
In this exercise, we will use the GNU Debugger (GDB) to debug the program created in class. GDB is a
powerful debugger that allows you to inspect and modify the state of a program while it is running.
We will use GDB to set breakpoints, step through the program, and examine the values of registers
and memory locations.

### - Download and Install GDB
To install GDB for ARM, run the following command:
```bash
sudo apt install gdb-multiarch
```

### - On this repository, you will find different folders with the source code that you have seen in class
Let's start by debugging the code in the `exercise1` folder. 
To compile the code, you need to include the `-g` flag to generate debugging information. This is
required for GDB, to be able to display the source code and set breakpoints.
```bash
arm-none-eabi-as -mcpu=cortex-m4 -g sum.s -o sum.o
```
Now, let's link our object file to create an ELF file.
```bash
arm-none-eabi-ld -T linker.ld sum.o -o sum.elf
```
### - Start GDB and Load the Program
To start GDB and load the program, run the following command:
```bash
gdb-multiarch sum.elf
```
This will start GDB and load the `sum.elf` program. You should see the GDB prompt.

Open another terminal and start the QEMU emulator with the following command:
```bash
qemu-system-arm -M netduino2 -nographic -kernel sum.elf -s -S
```
Notice the `-s` and `-S` flags. The `-s` flag specifies to start a GDB server on port 1234. The `-S`
flag specifies to pause the program execution at the start. This allows you to connect GDB before
the program starts running.

### - Connect GDB to QEMU
In the GDB prompt, connect to the QEMU GDB server by running the following command:
```bash
target remote localhost:1234
```
This will connect GDB to the QEMU GDB server running on port 1234. You should see a message that GDB 
is connected to the remote target.

Now, you can use GDB commands to debug the program. Here are some useful commands:
- `layout regs`: Display the register values in a split window.
- `layout src`: Display the source code in a split window.
- `ni`: Step to the next instruction.
- `si`: Step to the next instruction, stepping into function calls.
- `break <line>`: Set a breakpoint at a specific line number.
- `break <function>`: Set a breakpoint at a specific function/label.
- `continue`: Continue running the program.
- `info registers`: Display the values of all registers.
- `x/<n>x <address>`: Display the hexadecimal memory contents at a specific address.

Now, run the programs that are in the other folders and try to debug them using GDB. You can use the
same steps as above to debug the programs. You can also try setting breakpoints, stepping through
the code, and examining the values of registers and memory locations. 







