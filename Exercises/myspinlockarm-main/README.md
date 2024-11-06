# MySinLockARM



## Getting started

This project demonstrates a simple implementation of a spinlock using ARM assembly instructions `LDREX` and `STREX`. 

Spinlocks are fundamental synchronization primitives used in multi-threaded or multi-core environments to protect shared resources by allowing only one thread or core to access the resource at a time. This project includes `C` and inline `ARM assembly` code to create, acquire, and release a spinlock on ARM v7 architecture.

## What is a Spinlock?

A spinlock is a type of lock that causes a thread attempting to acquire it to loop repeatedly ("spin") while waiting for the lock to become available. Spinlocks are typically used in low-level, high-performance scenarios, such as operating system kernels or embedded systems, where tasks need to wait for resources in a way that avoids context switching.

- **Advantages**: Spinlocks are fast in scenarios where the wait time is expected to be very short because they avoid the overhead of blocking and waking up threads.
- **Disadvantages**: Spinlocks can lead to high CPU utilization if the lock is held for a long time, as the waiting thread keeps "spinning" and consuming CPU cycles.

## Project Structure
```
├── library.c
├── linker.ld
├── main.c
├── Makefile
├── README.md
├── startup.s
├── sync_primitives.c
└── sync_primitives.h
```
## Spinlock Primitives

### `spinlock_init`
This function initializes the spinlock by setting it to an unlocked state (e.g., `0`). Initialization must be done before any thread or core attempts to acquire the lock.

### `spinlock_acquire`
The spinlock_acquire function tries to acquire the lock using ARM's exclusive `LDREX` (Load-Exclusive) and `STREX` (Store-Exclusive) instructions.

### `spinlock_release` 
The spinlock_release function releases the lock by setting it to 0, indicating that the resource is available.

## ARM implementation

### Atomic instructons
`LDREX` loads the lock value into a register and sets an exclusive monitor for that address, which allows the ARM core to detect if other cores modify it.
`STREX` tries to store a new value to the lock if no other core has modified it since `LDREX`. 

If `STREX` succeeds, it returns 0, meaning the lock was acquired. If it fails (another core modified the lock), it returns 1, and we retry.

### Single-core vs Multi-core

On a single-core processor, using `LDREX` and `STREX` for a spinlock is generally less important, because there are no other cores that could modify the memory location being locked. 

Spinlocks are in general designed for multi-core environments where one core might acquire a lock, and other cores need to wait until the lock is released.

Though, disabling interrupts (e.g., using cpsid i to disable interrupts and cpsie i to enable them back) is more efficient for achieving mutual exclusion on a single-core ARM processor, on ARM, `LDREX` and `STREX` are the standard instructions for atomic operations. 

`LDREX`: Loads a value from a memory address and marks it as "exclusive." This means that the processor begins tracking that address and will monitor if any other instruction or process accesses or modifies it.

`STREX`: Attempts to store a new value to the same address only if no other process or interrupt has accessed it since the `LDREX` operation. If the store succeeds (meaning no other process has accessed the address), `STREX` returns 0. If it fails (another process or interrupt accessed the memory), `STREX` returns 1, and the code should retry the operation.

Even in single-core systems, `LDREX` and `STREX` are useful because interrupts or context switches can interrupt the sequence, potentially causing the memory location to be accessed by another context. If this happens, the exclusive monitor will detect that the address was accessed, causing `STREX` to fail. This ensures that the operation remains atomic even if an interrupt tries to access the same address during a context switch.





