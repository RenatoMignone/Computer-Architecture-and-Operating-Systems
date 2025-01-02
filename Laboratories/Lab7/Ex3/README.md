# Exercise 1: dummy device 

Write a Linux device driver which simulates the interaction with a device by printing messages on screen. You can start from the code seen during the lesson and modify it. You find it linked in the Template directory. Use cat and echo to simulate read and write accesses.

## Requirements
	1) Modify the write code to take an array as an input from the user, copy it in the kernel space and compute the sum of its elements. If nothing was passed as an input print a warning message.
	2) Modify the read code to copy the result of the addition to user space in order for the user to see the result.
	3) Try to modify the open function to get the device two times with try\_module\_get. Leave the release function as it is (single module\_put). What happens now if you try to rmmod the module after using it (with cat or echo)
	4) Create a small user program to interact with the device using open, close, read and write functions.
