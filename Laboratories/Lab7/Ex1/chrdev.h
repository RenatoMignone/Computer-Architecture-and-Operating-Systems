#ifndef __CHRDEV_H__
#define __CHRDEV_H__

/* Needed to create the device*/
#include <linux/device.h> 

/* Neede to create device's class and to point to THIS_MODULE*/
#include <linux/version.h> 

#include "shared.h"

extern struct file_operations __chrdev_fops__;
 
/* Major number assigned to the device driver */ 
static int major;
 
/* Class of the device which will be created */
static struct class *cls; 

#endif
