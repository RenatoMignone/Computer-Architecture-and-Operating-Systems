#include "chrdev.h"

static int __init chardev_init(void) 
{ 
	/* Reister charcater device */
    major = register_chrdev(0, DEVICE_NAME, &__chrdev_fops__); 
 
    if (major < 0) { 
        pr_alert("Registering char device failed with %d\n", major); 
        return major; 
    } 
 
    pr_info("I was assigned major number %d.\n", major); 

	/* The class_create function in the Linux kernel is part of the Linux device
	 * model. It is used when writing kernel modules, typically for device
	 * drivers, to create a device class that organizes devices of a similar
	 * type in the /sys/class/ directory. */

#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 4, 0) 
    cls = class_create(DEVICE_NAME); 
#else 
    cls = class_create(THIS_MODULE, DEVICE_NAME); 
#endif

	/* The MKDEV macro in the Linux kernel is used to encode a major and minor
	 * device number into a single dev_t value. This dev_t is used to uniquely
	 * identify a device in the system. */
    device_create(cls, NULL, MKDEV(major, 0), NULL, DEVICE_NAME); 
 
    pr_info("Device created on /dev/%s\n", DEVICE_NAME); 
 
    return SUCCESS; 
} 
 
static void __exit chardev_exit(void) 
{ 
    device_destroy(cls, MKDEV(major, 0)); 
    class_destroy(cls); 
 
    /* Unregister the device */ 
    unregister_chrdev(major, DEVICE_NAME); 
} 
 
module_init(chardev_init); 
module_exit(chardev_exit); 
 
MODULE_LICENSE("GPL");
