/* 
 * chardev.c: Creates a read-only char device that says how many times 
 * you have read from the dev file 
 */ 
 
#include <linux/atomic.h> 
#include <linux/cdev.h> 
#include <linux/delay.h> 
#include <linux/device.h> 
#include <linux/fs.h> 
#include <linux/init.h> 
#include <linux/kernel.h> /* for sprintf() */ 
#include <linux/module.h> 
#include <linux/printk.h> 
#include <linux/types.h> 
#include <linux/uaccess.h> /* for get_user and put_user */ 
#include <linux/version.h> 
 
#include <asm/errno.h>


/*------------------------------------------------------------------------------*/ 
/*------------------------------------------------------------------------------*/ 
/*  Prototypes - this would normally go in a .h file */ 
static int device_open(struct inode *, struct file *); 
static int device_release(struct inode *, struct file *); 
static ssize_t device_read(struct file *, char __user *, size_t, loff_t *); 
static ssize_t device_write(struct file *, const char __user *, size_t, loff_t *); 
 
/*------------------------------------------------------------------------------*/ 
/*------------------------------------------------------------------------------*/ 
#define SUCCESS 0 
#define DEVICE_NAME "chardev" /* Dev name as it appears in /proc/devices   */ 
#define BUF_LEN 80 /* Max length of the message from the device */ 
 
/* Global variables are declared as static, so are global within the file. */ 
static int major; /* major number assigned to our device driver */ 
 
enum { 
    CDEV_NOT_USED, 
    CDEV_EXCLUSIVE_OPEN, 
}; 

/* Is device open? Used to prevent multiple access to device */ 
static atomic_t already_open = ATOMIC_INIT(CDEV_NOT_USED); 
 
static char msg[BUF_LEN + 1]; /* The msg the device will give when asked */ 
 
static struct class *cls; 
 
static struct file_operations chardev_fops = { 
    .read = device_read, 
    .write = device_write, 
    .open = device_open, 
    .release = device_release, 
}; 




/*------------------------------------------------------------------------------*/ 
/*------------------------------------------------------------------------------*/ 
static int __init chardev_init(void) 
{ 
    //the first parameter 0 is the major number, if set to 0 the kernel will assign a major number dynamically
    //the second parameter is the name of the device
    //the third parameter is the file operations structure, so the structure that defines the functions to call when the device is read, written, opened or closed
    major = register_chrdev(0, DEVICE_NAME, &chardev_fops); 
 
    if (major < 0) { 
        pr_alert("Registering char device failed with %d\n", major); 
        return major; 
    } 
 
    pr_info("I was assigned major number %d.\n", major); 

    /* The class_create function in the Linux kernel is part of the Linux device model. It is used when writing kernel modules, typically for device drivers, to create a device class that organizes devices of a similar type in the /sys/class/ directory. */

    #if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 4, 0) 
        cls = class_create(DEVICE_NAME); 
    #else 
        cls = class_create(THIS_MODULE, DEVICE_NAME); 
    #endif


    /* The MKDEV macro in the Linux kernel is used to encode a major and minor device number into a single dev_t value. This dev_t is used to uniquely identify a device in the system. */
    //the first parameter "cls" is the class to which the device belongs
    //the second parameter is the parent device, if there is no parent, we can set it to NULL
    //the third parameter is the device number, thanks to the MKDEV macro we can create it from the major and minor numbers
    //the fourth parameter is the private data for the device, in this case we don't need it so we set it to NULL
    //the fifth parameter is the name of the device
    device_create(cls, NULL, MKDEV(major, 0), NULL, DEVICE_NAME); 
 
    pr_info("Device created on /dev/%s\n", DEVICE_NAME); 
 
    return SUCCESS; 
} 




/*------------------------------------------------------------------------------*/ 
/*------------------------------------------------------------------------------*/ 
static void __exit chardev_exit(void) 
{ 
    device_destroy(cls, MKDEV(major, 0)); 
    class_destroy(cls); 
 
    /* Unregister the device */ 
    unregister_chrdev(major, DEVICE_NAME); 
} 



/*------------------------------------------------------------------------------*/ 
/*------------------------------------------------------------------------------*/ 
/* Methods */ 
/* Called when a process tries to open the device file, like 
 * "sudo cat /dev/chardev" 
 */ 
static int device_open(struct inode *inode, struct file *file) 
{ 
    static int counter = 0; 

    /* The atomic_cmpxchg (atomic compare-and-exchange) operation is an important atomic primitive provided by the Linux kernel. 
    It is used for implementing synchronization mechanisms without locks, ensuring that concurrent modifications to shared data are safe.*/ 
    
    //This code snippet is used to prevent multiple access to the device.
    //The first parameter is a variable where we store the informations about the state of the device
    //The second parameter is the value we want to compare with the value stored in the first parameter,
    //if the two values are equal, the function will change the value stored in the first parameter with the value passed as the third parameter
    if (atomic_cmpxchg(&already_open, CDEV_NOT_USED, CDEV_EXCLUSIVE_OPEN)) 
        return -EBUSY; //Device is busy
 
    sprintf(msg, "I already told you %d times Hello world!\n", counter++); 

    /* The try_module_get function in the Linux kernel is used to increment the reference count of a module. 
    It ensures that the module remains loaded in memory while its functionality is being accessed,
    preventing the module from being unloaded prematurely.*/
    try_module_get(THIS_MODULE); 
 
    return SUCCESS; 
} 



/*------------------------------------------------------------------------------*/ 
/*------------------------------------------------------------------------------*/ 
/* Called when a process closes the device file. */ 
static int device_release(struct inode *inode, struct file *file) 
{ 
    /* We're now ready for our next caller */
    //sets the value of the variable to CDEV_NOT_USED, so the device is not used anymore 
    atomic_set(&already_open, CDEV_NOT_USED); 
 
    //Decreases the reference count of the module. The reference count means the number of processes that are using the module.
    //If the reference count value reaches 0, the module will be unloaded from memory.
    module_put(THIS_MODULE); 
 
    return SUCCESS; 
} 




/*------------------------------------------------------------------------------*/ 
/*------------------------------------------------------------------------------*/ 
/* Called when a process, which already opened the dev file, attempts to read from it.*/

//The first parameter is a pointer to the file structure that represents the opened file 
//The second parameter is a pointer to the user-space buffer where the data will be written. __user is a macro that tells the compiler that the buffer is in the user space.
//The third parameter is the length of the buffer. Tells the driver how much data can safely be written to the buffer.
//The fourth parameter is a pointer to the current position in the file. It is used to keep track of where the last read operation ended.

static ssize_t device_read(struct file *filp, /* see include/linux/fs.h   */ 
                           char __user *buffer, /* buffer to fill with data */ 
                           size_t length, /* length of the buffer     */ 
                           loff_t *offset /* The offset keeps track of where the last read ended. This allows the driver to know where to start the next read operation (useful for sequential access to data, like a file).*/
			   ) 
{ 
    /* Number of bytes actually written to the buffer */ 
    int bytes_read = 0; 
    const char *msg_ptr = msg; 
 
    if (!*(msg_ptr + *offset)) { /* we are at the end of message */ 
        *offset = 0; /* reset the offset */ 
        return 0; /* signify end of file */ 
    } 
 
    //now here we add to the pointer the value of the offset, so we can start reading from the offset
    msg_ptr += *offset; 
 

    /* Actually put the data into the buffer */
    //Here we loop while the value of lenght is greater than 0 and the value of the pointer is different from 0
    while (length && *msg_ptr) { 
        /* The buffer is in the user data segment, not the kernel 
         * segment so "*" assignment won't work.  We have to use 
         * put_user which copies data from the kernel data segment to 
         * the user data segment. 
         */ 

        //The put_user function copies a single data item from the kernel space to the user space
        //The msg_ptr is the data in the kernel space
        //The buffer is the one in the user space.

        put_user(*(msg_ptr++), buffer++); 
        length--; //one byte has been read and copied from the kernel space to the user one
        bytes_read++; //increment the number of bytes read
    } 
 
    *offset += bytes_read; 
 
    /* Most read functions return the number of bytes put into the buffer. */ 
    return bytes_read; 
} 




/*------------------------------------------------------------------------------*/ 
/*------------------------------------------------------------------------------*/ 
/* Called when a process writes to dev file: echo "hi" > /dev/hello */ 
static ssize_t device_write(struct file *filp, const char __user *buff, 
                            size_t len, loff_t *off) 
{ 
    pr_alert("Sorry, this operation is not supported.\n"); 
    return -EINVAL; //standard error code thta tells us that the operation in not supported for this file.
} 
 
/*------------------------------------------------------------------------------*/ 
/*------------------------------------------------------------------------------*/ 





module_init(chardev_init); 
module_exit(chardev_exit); 
 
MODULE_LICENSE("GPL");

