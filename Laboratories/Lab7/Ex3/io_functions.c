#include "io_functions.h"
#include "uart.h"

/* Non static to use it in chrdev. Map custom functions */
struct file_operations __chrdev_fops__ = { 
    .read = device_read, 
    .write = device_write, 
    .unlocked_ioctl = device_ioctl, 
    .open = device_open, 
    .release = device_release
}; 

/* Is device open? Used to prevent multiple access to device */ 
static atomic_t already_open = ATOMIC_INIT(CDEV_NOT_USED); 

static void __iomem *mapped_addr;
Uart_TxRx *pl011;
 
/* Custom open */
static int device_open(struct inode *inode, struct file *file) 
{ 

   /* The atomic_cmpxchg (atomic compare-and-exchange) operation is an important
	* atomic primitive provided by the Linux kernel. It is used for implementing
	* synchronization mechanisms without locks, ensuring that concurrent
	* modifications to shared data are safe.*/ 
    if (atomic_cmpxchg(&already_open, CDEV_NOT_USED, CDEV_EXCLUSIVE_OPEN)) 
        return -EBUSY; 
 
	/* The try_module_get function in the Linux kernel is used to increment the
	 * reference count of a module. It ensures that the module remains loaded in
	 * memory while its functionality is being accessed, preventing the module
	 * from being unloaded prematurely.*/
    try_module_get(THIS_MODULE); 

	/* Greet the desired person. Visualize with sudo dmesg
	 * WARNING: insmod doesn't accept spaces
	*/
	mapped_addr = ioremap(UART_BASE_ADDR, UART_SIZE);

	if (!mapped_addr) {
		pr_err("ioremap failed\n");
		return -ENOMEM;
	}

	pr_info("Device address remapped");

	uart_init(pl011, mapped_addr);
 
    return SUCCESS; 
} 
 
/* Custom release */
static int device_release(struct inode *inode, struct file *file) 
{ 
    /* Leave the resource to the next caller */ 
    atomic_set(&already_open, CDEV_NOT_USED); 
 
	/* Decrement the usage count, or else once you opened the file, you will 
     * never get rid of the module.  */ 
    module_put(THIS_MODULE); 
 
    return SUCCESS; 
} 
 
/* Custom read */
static ssize_t device_read(

	/* see include/linux/fs.h   */ 
	struct file *filp,

	/* buffer to fill with data */ 
	char __user *buffer,

	/* length of the buffer     */ 
	size_t length,

	/* The offset keeps track of where the last read ended. This allows the
	 * driver to know where to start the next read operation (useful for
	 * sequential access to data, like a file).*/
	loff_t *offset
){ 

	pr_alert("Read is not supported\n");

	return 0;

} 
 
/* Custom write */
static ssize_t device_write(
	struct file *filp,
	const char __user *buff, 
	size_t len,
	loff_t *off
){ 

	char msg_ptr[BUF_LEN];
	int bytes_write = 0;
	int i = 0;

	for(i = 0; i < len; i++){
		get_user(msg_ptr[i], buff + i);
		bytes_write ++;
	}

	uart_tx(pl011, msg_ptr, len);

	return bytes_write;
} 


/* This function is called whenever a process tries to do an ioctl on our 
 * device file. We get two extra parameters (additional to the inode and file 
 * structures, which all device functions get): the number of the ioctl called 
 * and the parameter given to the ioctl function. 
 * 
 * If the ioctl is write or read/write (meaning output is returned to the 
 * calling process), the ioctl call returns the output of this function. 
 */ 
static long device_ioctl(struct file *file, unsigned int ioctl_num, unsigned
		long ioctl_param){ 

	Uart_TxRx *uart = (Uart_TxRx *)ioctl_param;

    /* We don't want to talk to two processes at the same time. */ 
    if (atomic_cmpxchg(&already_open, CDEV_NOT_USED, CDEV_EXCLUSIVE_OPEN)) 
        return -EBUSY; 
 
    /* Switch according to the ioctl called */ 
    switch (ioctl_num) { 

		case IOCTL_SET_BAUDRATE:

			*(int*)(uart -> baudrate_div) = 16;
			pr_info("Baudrate set\n");

			break; 

		case IOCTL_SET_PARITY: 

			*(int*)(uart -> lcr) |= (1 << 1);
			pr_info("Parity enabled\n");

			break; 

		case IOCTL_SET_STOPBITS: 

			*(int*)(uart -> lcr) |= (1 << 3);
			pr_info("Set number of stopbits to 2\n");

			break; 
    } 
 
    /* We're now ready for our next caller */ 
    atomic_set(&already_open, CDEV_NOT_USED); 
 
    return SUCCESS; 
} 

