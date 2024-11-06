## Exercise 1: User Mode Emulation

In this exercise, you will learn about User Mode Emulation on QEMU. This mode allows you to run a
program compiled for a different architecture than your host machine. In this case, we will run an
32-bit ARM program on your Machine (x86 architecture for Intel or AMD processors or ARM 64-bit
architecture for Apple M series processors) using QEMU.

The program is a simple “Hello World” application that prints a message to the console.

### Write and Compile a Simple Hello World Program for ARM

Create a new file named `hello.c` with the typical "Hello, World!" program content, once you have
created the file, we will compile it for ARM.

To compile the program for ARM, you need to install the ARM cross-compiler, there are different tool
chains that allows you to compile code for 32-bit ARM architecture, in this lab we will use the GNU
ARM toolchain. Since we will run the program on QEMU in User Mode Emulation, we will use the ARM
Linux GNU EABI toolchain. This toolchain allows you to compile code from your host machine to run on
a linux system with an 32-bit ARM architecture. To install the ARM Linux GNU EABI toolchain, run the
following command:

```bash
sudo apt install gcc-arm-linux-gnueabi
```
Now, you can compile the Hello World program for ARM by executing the following command:

```bash
arm-linux-gnueabi-gcc -o helloarm.o -static hello.c
```
This command will generate an executable file named `helloarm.o`. The `-static` flag is used to
create a statically linked binary, meaning that all libraries needed by the program are included
within the binary itself. This is particularly useful when compiling and running a program on a
machine that does not have all the required libraries installed. In this instance, we are compiling
the program for ARM while ensuring that all necessary libraries are included in the binary so that
it can run on the host machine.

If you try to execute the `helloarm.o` program on your host machine with the following command:

```bash
./helloarm.o
```
You will encounter an error message: 

```plaintext
-bash: ./helloarm.o: cannot execute binary file: Exec format error
```
This error occurs because the program was compiled for ARM. To verify the architecture of the
executable, you can use the following command:
```bash
file helloarm.o
```
The output should indicate that the file is an ARM executable.
```plaintext
helloarm.o: ELF 32-bit LSB executable, ARM, EABI5 version 1 (SYSV), statically linked, not stripped
```
### Compile and Run the Program for the Host Machine
Next, let’s compile the program for the host machine to observe the difference between using the
arm-linux-gnueabi-gcc compiler and your host machine’s compiler. To compile the program, run the
following command:

```bash
gcc -o hellohost.o hello.c
```
Note that when compiling the program for the host machine, we did not use the `-static` flag this
time. This indicates that the binary is dynamically linked, meaning that the libraries needed by the
program are not included in the binary itself but are loaded at runtime. This is the default
behavior when compiling programs for the host machine, and a statically linked binary is not
necessary. Since linux systems have the necessary libraries installed, the dynamically linked binary
will work without any issues. The resulting executable file will be named `hellohost.o`. If you run
the program with the following command:

```bash
./hellohost.o
```
You should see the message "Hello, World!" printed to the console.

If you verify the architecture of the executable using the `file` command, you will see that it is 
compiled for your host machine's architecture.
 ``` bash
file hellohost.o
```
The output should indicate that the file is an x86 executable or an ARM 64-bit executable, depending
on your host machine's architecture.
```plaintex
hellohost.o: ELF 64-bit LSB shared object, x86-64, version 1 (SYSV), dynamically linked, interpreter
/lib64/ld-linux-x86-64.so.2, for GNU/Linux 3.2.0, not stripped
```
For Apple M series processors:
```plaintex
hellohost.o: ELF 64-bit LSB shared object, ARM aarch64, version 1 (SYSV), dynamically linked,
interpreter /lib/ld-linux-aarch64.so.1, for GNU/Linux 3.7.0, not stripped
```

### Run the ARM Program on QEMU in User Mode Emulation 
To install the QEMU emulator for user mode emulation, run the following command:

```bash
sudo apt install qemu-user
```
Now, you can run the ARM program on your host machine using QEMU in User Mode Emulation. To do this, 
execute the following command:

```bash
qemu-arm helloarm.o
```
You should see the message "Hello, World!" printed to the console. This demonstrates that you can
run an ARM program on your host machine using QEMU in User Mode Emulation.

- [Next: Exercise 2: Bare-metal System](./Ex2_Baremetal/README.md)
- [Home](../README.md)




