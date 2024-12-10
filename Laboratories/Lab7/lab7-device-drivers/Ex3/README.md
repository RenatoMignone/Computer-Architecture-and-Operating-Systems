# Develop a complete device driver to simulate the interaction with the UART

Check the documentation of the pl011 UART registers:

https://developer.arm.com/documentation/ddi0183/g/programmers-model/summary-of-registers

Search for the Line Control Register. This is where you can enable the parity bit, configure the number of stop bits and similar.

## Requirements
    1) Develop a complete Linux device driver able to support open, close, read, write and ioctl
    2) Use ioctl commands to:
        a) set the baud rate
        b) enable the parity bit
        c) select the amount of stop bits
        d) select the data width

Since the UART in qemu is only simulated, setting these registers doesn't change anything in practice. Print messages to the log to signal the correct behavior of the ioctl command.  
