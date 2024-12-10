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
 
/*  Prototypes - this would normally go in a .h file */ 
static int device_open(struct inode *, struct file *); 
static int device_release(struct inode *, struct file *); 
static ssize_t device_read(struct file *, char __user *, size_t, loff_t *); 
static ssize_t device_write(struct file *, const char __user *, size_t, 
                            loff_t *); 
 
#define SUCCESS 0 
#define DEVICE_NAME "chardev2" /* Dev name as it appears in /proc/devices   */ 
#define BUF_LEN 80 /* Max length of the message from the device */ 
#define MAX_ELEMENTS 1024 // Maximum number of integers to process



 
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
 
static int __init chardev_init(void) 
{ 
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
 
/* Methods */ 
 
/* Called when a process tries to open the device file, like 
 * "sudo cat /dev/chardev" 
 */ 
static int device_open(struct inode *inode, struct file *file) 
{ 
    static int counter = 0; 

   /* The atomic_cmpxchg (atomic compare-and-exchange) operation is an important atomic primitive provided by the Linux kernel. It is used for implementing synchronization mechanisms without locks, ensuring that concurrent modifications to shared data are safe.*/ 
    if (atomic_cmpxchg(&already_open, CDEV_NOT_USED, CDEV_EXCLUSIVE_OPEN)) 
        return -EBUSY; 
 
    sprintf(msg, "I already told you %d times Hello world!\n", counter++); 

    /* The try_module_get function in the Linux kernel is used to increment the reference count of a module. It ensures that the module remains loaded in memory while its functionality is being accessed, preventing the module from being unloaded prematurely.*/
    try_module_get(THIS_MODULE); 
    try_module_get(THIS_MODULE); 

    return SUCCESS; 
} 
 
/* Called when a process closes the device file. */ 
static int device_release(struct inode *inode, struct file *file) 
{ 
    /* We're now ready for our next caller */ 
    atomic_set(&already_open, CDEV_NOT_USED); 
 
    /* Decrement the usage count, or else once you opened the file, you will 
     * never get rid of the module. 
     */ 
    module_put(THIS_MODULE); 
 
    return SUCCESS; 
} 





 
/* Called when a process, which already opened the dev file, attempts to 
 * read from it. 
 */ 
static ssize_t device_read(struct file *filp, char __user *buff, size_t len, loff_t *off)
{
    int sum_result;
    if (len < sizeof(int)) {
        printk(KERN_ERR "Buffer size too small to return sum\n");
        return -EINVAL;
    }

    // Write the sum_result to the user buffer
    if (put_user(sum_result, (int __user *)buff)) {
        printk(KERN_ERR "Failed to write sum to user space\n");
        return -EFAULT;
    }

    printk(KERN_INFO "Sum sent to user: %d\n", sum_result);

    // Return the size of the data written
    return sizeof(int);
}






static ssize_t device_write(struct file *filp, const char __user *buff, size_t len, loff_t *off)
{
    size_t num_elements;
    int value, i;
    int sum_result = 0; // Reset the sum

    if (len % sizeof(int) != 0) {
        printk(KERN_ERR "Invalid length: Not a multiple of integer size\n");
        return -EINVAL;
    }

    num_elements = len / sizeof(int);


    for (i = 0; i < num_elements; i++) {
        if (get_user(value, ((int __user *)buff) + i)) {
            printk(KERN_ERR "Failed to read element %zd from user\n", i);
            return -EFAULT;
        }
        sum_result += value;
    }

    printk(KERN_INFO "Sum of array elements: %d\n", sum_result);

    return len;
}


 
module_init(chardev_init); 
module_exit(chardev_exit); 
 
MODULE_LICENSE("GPL");

