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

## Map the physical address into Linux kernel virtual memory address space
In order to safely access I/O and memory Linux works with a virtual address
space. This allows to efficiently handle memory protectio and possible space
limitation issues.

To map a physical address into the kernel virtual address space you can use ioremap:

```
static void __iomem *mapped_addr; // Pointer for the mapped memory

mapped_addr = ioremap(PHYSICAL_ADDRESS, SIZE);

if (!mapped_addr) {

	pr_err("my_device: failed to map physical address\n");
	unregister_chrdev(ret, DEVICE_NAME);

	return -ENOMEM;
}
```
