#ifndef __SHARED_H__
#define __SHARED_H__

#define SUCCESS 0 

/* Max length of the message from the device */ 
#define BUF_LEN 80
#define MAX_INT_COUNT 20

/* Device file that will be created in /dev */
#define DEVICE_NAME "chardev"

#define DEVICE_PATH "/dev/"DEVICE_NAME

#ifdef __KERNEL__
#include <linux/kernel.h>
#else
#include <stdlib.h> 
#include <stdio.h>
#endif

void array_to_string(int *array, size_t size, char *str);
void string_to_array(const char *input, int *output, int max_size);

#endif
