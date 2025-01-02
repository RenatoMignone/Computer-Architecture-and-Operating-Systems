#include "io_functions.h"


/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* Non static to use it in chrdev. Map custom functions */
struct file_operations __chrdev_fops__ = { 
    .read		= device_read,
    .write		= device_write,
    .open		= device_open,
    .release	= device_release
}; 

/* Is device open? Used to prevent multiple access to device */ 
static atomic_t already_open = ATOMIC_INIT(CDEV_NOT_USED); 

static int sum = 0;
 


/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
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
 
    return SUCCESS; 
} 
 


/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
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
 


/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* Custom read */
static ssize_t device_read(
	struct file *filp,
	char __user *buffer,
	size_t length,
	loff_t *offset
){ 
    /* Number of bytes actually written to the buffer */ 
	int bytes_read = 0; 
	char msg[BUF_LEN]; 
	int i;

	sprintf(msg, "%d", sum);

	i = 0;
	while(msg[i] != '\0'){
		put_user(msg[i], buffer); 
		pr_info("msg[%d] = %c\n", i, msg[i]);

		i++;
		bytes_read ++;
		buffer ++;
    } 

	put_user('\0', buffer); 
 
    /* Most read functions return the number of bytes put into the buffer. */ 
    return bytes_read; 
} 
 



/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* Custom write */
static ssize_t device_write(
	struct file *filp,
	const char __user *buff, 
	size_t len,
	loff_t *off
){ 

	char msg_ptr[BUF_LEN];
	int array[MAX_INT_COUNT] = {0};
	int bytes_write = 0;
	int i = 0;

	for(i = 0; i < len; i++){
		get_user(msg_ptr[i], buff + i);
		bytes_write ++;
	}

	string_to_array(msg_ptr, array, MAX_INT_COUNT);

	sum = 0;

	for(i = 0; i < MAX_INT_COUNT; i++){
		sum += array[i];
	}

	return bytes_write;
} 

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
