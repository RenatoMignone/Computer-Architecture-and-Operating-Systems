#ifndef __IO_FUNCTIONS_H__
#define __IO_FUNCTIONS_H__

#include <linux/module.h> 
#include "shared.h"

static int device_open(struct inode *, struct file *); 

static int device_release(struct inode *, struct file *); 

static ssize_t device_read(struct file *, char __user *, size_t, loff_t *); 

static ssize_t device_write(struct file *, const char __user *, size_t, 
                            loff_t *); 

static long device_ioctl(struct file *file, unsigned int ioctl_num, unsigned
		long ioctl_param);

enum { 
    CDEV_NOT_USED, 
    CDEV_EXCLUSIVE_OPEN, 
}; 

#endif
