#ifndef __SHARED_H__
#define __SHARED_H__
 
#include <linux/ioctl.h>

#ifdef __KERNEL__
#include <linux/kernel.h>
#else
#include <stdlib.h> 
#include <stdio.h>
#endif

#define SUCCESS 0 

/* Max length of the message from the device */ 
#define BUF_LEN 80

/* Device file that will be created in /dev */
#define DEVICE_NAME "chardev3"

#define DEVICE_PATH "/dev/"DEVICE_NAME

#define MAGIC_NUM 'K'

#define IOCTL_SET_BAUDRATE _IOW(MAGIC_NUM, 0, void*)
#define IOCTL_SET_PARITY  _IOW(MAGIC_NUM, 1, void*)
#define IOCTL_SET_STOPBITS  _IOW(MAGIC_NUM, 2, void*)

#endif
