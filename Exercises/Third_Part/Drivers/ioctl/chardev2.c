/* 
 * chardev2.c - Create an input/output character device 
 */ 

#include <linux/atomic.h> 
#include <linux/cdev.h> 
#include <linux/delay.h> 
#include <linux/device.h> 
#include <linux/fs.h> 
#include <linux/init.h> 
#include <linux/module.h> /* Specifically, a module */ 
#include <linux/printk.h> 
#include <linux/types.h> 
#include <linux/uaccess.h> /* for get_user and put_user */ 
#include <linux/version.h> 
 
#include <asm/errno.h> 
 
#include "chardev.h" 

/*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*/

#define SUCCESS 0 
#define DEVICE_NAME "char_dev" 
#define BUF_LEN 80 
 
enum { 
    CDEV_NOT_USED, 
    CDEV_EXCLUSIVE_OPEN, 
}; 
 

/*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*/
/* Is the device open right now? Used to prevent concurrent access into the same device*/ 
static atomic_t already_open = ATOMIC_INIT(CDEV_NOT_USED); 
 
/* The message the device will give when asked */ 
static char message[BUF_LEN + 1]; 
static struct class *cls;



/*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*/
/* This is called whenever a process attempts to open the device file */ 
static int device_open(struct inode *inode, struct file *file) 
{ 
    pr_info("device_open(%p)\n", file); 
 
    try_module_get(THIS_MODULE); 
    return SUCCESS; 
} 


/*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*/
static int device_release(struct inode *inode, struct file *file) 
{ 
    pr_info("device_release(%p,%p)\n", inode, file); 
 
    module_put(THIS_MODULE); 
    return SUCCESS; 
} 



/*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*/
/* Called when a process, which already opened the dev file, attempts to read from it.*/

//The first parameter is a pointer to the file structure that represents the opened file 
//The second parameter is a pointer to the user-space buffer where the data will be written. __user is a macro that tells the compiler that the buffer is in the user space.
//The third parameter is the length of the buffer. Tells the driver how much data can safely be written to the buffer.
//The fourth parameter is a pointer to the current position in the file. It is used to keep track of where the last read operation ended.

static ssize_t device_read(struct file *file, /* see include/linux/fs.h   */ 
                           char __user *buffer, /* buffer to be filled  */ 
                           size_t length, /* length of the buffer     */ 
                           loff_t *offset) 
{ 
    /* Number of bytes actually written to the buffer */ 
    int bytes_read = 0; 

    /* How far did the process reading the message get? Useful if the message 
     * is larger than the size of the buffer we get to fill in device_read. 
     */ 
    const char *message_ptr = message; 
 
    if (!*(message_ptr + *offset)) { /* we are at the end of message */ 
        *offset = 0; /* reset the offset */ 
        return 0; /* signify end of file */ 
    } 
 
    message_ptr += *offset; 
 
    /* Actually put the data into the buffer */ 
    while (length && *message_ptr) { 
        /* Because the buffer is in the user data segment, not the kernel 
         * data segment, assignment would not work. Instead, we have to 
         * use put_user which copies data from the kernel data segment to 
         * the user data segment. 
         */ 
        put_user(*(message_ptr++), buffer++); 
        length--; 
        bytes_read++; 
    } 
 
    pr_info("Read %d bytes, %ld left\n", bytes_read, length); 
 
    *offset += bytes_read; 
 
    /* Read functions are supposed to return the number of bytes actually 
     * inserted into the buffer. 
     */ 
    return bytes_read; 
} 
 



/*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*/
/* called when somebody tries to write into our device file. */ 
static ssize_t device_write(struct file *file, const char __user *buffer, 
                            size_t length, loff_t *offset) 
{ 
    int i; 
 
    pr_info("device_write(%p,%p,%ld)", file, buffer, length); 
 

    //this loop will copy the data from the user space to the kernel space
    //so the get user function, copies a single data item from the user space to the kernel space
    
    //The "message" array represents the data in the kernel space, so the location where the data will be copied
    //The "buffer" represents the data in the user space that have to be copied to the kernel space
    for (i = 0; i < length && i < BUF_LEN; i++) 
        get_user(message[i], buffer + i); 
 
    /* Again, return the number of input characters used. */ 
    return i; 
} 
 




/*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*/

//ioctl stands for input/output control. It is a system call that allows a user to control a device driver.

//This function provides an interface to perform custom operations on the character device that are not covered by standard read or write.
static long device_ioctl(struct file *file, //file: Represents the file structure associated with the device.
             unsigned int ioctl_num, //ioctl_num: The specific ioctl command number.
             unsigned long ioctl_param)  //ioctl_param: A parameter passed from user space, typically used to send data or a pointer.
{ 
    int i; 
    long ret = SUCCESS; 
 
    /* We don't want to talk to two processes at the same time. */ 
    //So we are checking wether the device is already open or not
    if (atomic_cmpxchg(&already_open, CDEV_NOT_USED, CDEV_EXCLUSIVE_OPEN)) 
        return -EBUSY; 
 



    /* Switch according to the ioctl called */ 
    //here each ioctl command corresponds to a number, so the ioctl_num parameter is used to determine which command was called
    switch (ioctl_num) { 

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //IOCTL_SET_MSG: Sets a message in the device's internal buffer (message).
        case IOCTL_SET_MSG: { 

            //The user provides a pointer to a message in user space (ioctl_param).
            char __user *tmp = (char __user *)ioctl_param; 
            char ch; 
    
            /* Find the length of the message */ 
            // get_user reads characters from user space to determine the message length.
            //Ensures no buffer overflow by capping the message length at BUF_LEN.*/
            get_user(ch, tmp); 
            for (i = 0; ch && i < BUF_LEN; i++, tmp++) 
                get_user(ch, tmp); 
    
            //We call our write function, to copy the message from the user space to the kernel space, so to write 
            //the message stored in the ioctl_param to the device's internal buffer.
            device_write(file, (char __user *)ioctl_param, i, NULL); 
            break; 
        } 


        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //IOCTL_GET_MSG: Retrieves the current message stored in the device.
        //Here we read the current device message in the device driver, and put it in the ioctl_param buffer
        //So the ioctl_param buffer, will act as the buffer in the user space where the message will be copied
        case IOCTL_GET_MSG: { 
            loff_t offset = 0; 
    
            //we read 99 bytes from the device's internal buffer (message) and copy them to the user space buffer (ioctl_param).
            i = device_read(file, (char __user *)ioctl_param, 99, &offset); 
    
            //Put a zero at the end of the buffer, so it will be properly terminated. 
            put_user('\0', (char __user *)ioctl_param + i); 
            break; 
        } 


        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //IOCTL_GET_NTH_BYTE: Fetches the nth byte of the current message.
        case IOCTL_GET_NTH_BYTE: 
            //This ioctl is both input (ioctl_param) and output (the return value of this function).
            //Returns the nth byte of the message, where ioctl_param specifies the index.
            ret = (long)message[ioctl_param]; 
            break; 
    } 
 
    /* We're now ready for our next caller */
    //so we set the device to not used. 
    atomic_set(&already_open, CDEV_NOT_USED); 
 
    return ret; 
} 
 

/*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*/
/* Module Declarations */ 
static struct file_operations fops = { 
    .read = device_read, 
    .write = device_write, 
    .unlocked_ioctl = device_ioctl, 
    .open = device_open, 
    .release = device_release, /* a.k.a. close */ 
}; 



/*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*/
/* Initialize the module - Register the character device */ 
static int __init chardev2_init(void) 
{ 
    /* Register the character device (at least try) */ 
    int ret_val = register_chrdev(MAJOR_NUM, DEVICE_NAME, &fops); 
 
    /* Negative values signify an error */ 
    if (ret_val < 0) { 
        pr_alert("%s failed with %d\n", 
                 "Sorry, registering the character device ", ret_val); 
        return ret_val; 
    } 
 
    #if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 4, 0) 
        cls = class_create(DEVICE_FILE_NAME); 
    #else 
        cls = class_create(THIS_MODULE, DEVICE_FILE_NAME); 
    #endif 
    
    device_create(cls, NULL, MKDEV(MAJOR_NUM, 0), NULL, DEVICE_FILE_NAME); 
 
    pr_info("Device created on /dev/%s\n", DEVICE_FILE_NAME); 
 
    return 0; 
} 



/*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*/
/* Cleanup - unregister the appropriate file from /proc */ 
static void __exit chardev2_exit(void) 
{ 
    device_destroy(cls, MKDEV(MAJOR_NUM, 0)); 
    class_destroy(cls); 
 
    /* Unregister the device */ 
    unregister_chrdev(MAJOR_NUM, DEVICE_NAME); 
} 


/*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*/
module_init(chardev2_init); 
module_exit(chardev2_exit); 
 
MODULE_LICENSE("GPL");


