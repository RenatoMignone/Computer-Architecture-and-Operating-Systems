# Exercise 2: simulated hardware device 
By now, given all the curses you've thrown at it, you are familiar with qemu.  Let's start by looking at the memory map of the virt board on which you just booted Linux.

Run

```
qemu-system-aarch64 -M virt -S -nographic
```

This runs the 64 bit arm simulator using virt as a board, stopping the execution with -S, so we can avoid providing an elf file, and without opening the GUI.

Now press

```
ctrl a
```

to enter in command mode, and

```
c
```

to enter qemu consolle. You should see something like

```
QEMU 6.2.0 monitor - type 'help' for more information
(qemu)
```

Now type

```
info mtree
```

to visualize the simulated memory tree, which specofies the memory map of our board. Search for the line

```
0000000009000000-0000000009000fff (prio 0, i/o): pl011
```

This is the UART (https://developer.arm.com/Processors/PL011).

When you are done you can quit qemu's monitor by simply typing

```
quit
```

## Remap addresses in Linux virtual address space
Linux implements a memory management system based on virtal addresses. This guarantees a simplified, more efficient and more secure access to the memory. When you want to interact with a physical address in Linux you need first of all to remap the address into the Linux virtual address space. You can do this using the ioremap function:

```c
#include <linux/io.h>

#define UART_BASE_ADDR 0x09000000
#define UART_LAST_ADDR 0x09000fff
#define UART_SIZE UART_LAST_ADDR - UART_BASE_ADDR

static void __iomem *mapped_addr;

mapped_addr = ioremap(UART_BASE_ADDR, UART_SIZE);
if (!mapped_addr) {
    pr_err("Unable to remap the address\n");
    return -ENOMEM;
}
```

Once the address is remapped, you can use it like a regular pointer. However, when working with IO peripherals, it’s essential to ensure direct access. This avoids issues such as data caching or compiler optimizations that might reorder instructions, potentially disrupting the intended access sequence to the peripheral or causing similar problems. To avoid these issues linux provides functions to read and write from memory addresses:

```c
void writeb(u8 value, volatile void __iomem *addr);
u8 readb(const volatile void __iomem *addr);
```

These allow to read and write 8 bit values from and to the peripheral addresses. For example:

```c
writeb('a', mapped_addr);
```
Finally you can unmap the address with:

```c
if (mapped_addr) {
    iounmap(mapped_addr);
    pr_info("Unmapped addredd\n");
}
```

## Requirements
1) Write a very simple Linux kenel module, as the ones you designed in lab 6
2) Map the UART in virtual address space in the init function
3) Write a string to the UART: you can reuse the same functions you developed during the bare-metal labs
4) Unmap the UART in the cleanup function
