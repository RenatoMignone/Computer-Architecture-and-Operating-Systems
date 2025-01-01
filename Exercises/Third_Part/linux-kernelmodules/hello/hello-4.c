/* 
 * hello-3.c - Illustrating the __init, __initdata and __exit macros. 
 */ 
#include <linux/init.h> /* Needed for the macros */ 
#include <linux/module.h> /* Needed by all modules */ 
#include <linux/printk.h> /* Needed for pr_info() */ 

MODULE_LICENSE("GPL"); 
MODULE_AUTHOR("LKMPG"); 
MODULE_DESCRIPTION("A sample driver"); 

static int hello4_data __initdata = 4; 
 
static int __init hello_3_init(void) 
{ 
    pr_info("Hello, world %d\n", hello4_data); 
    return 0; 
} 
 
static void __exit hello_3_exit(void) 
{ 
    pr_info("Goodbye, world 4\n"); 
} 
 
module_init(hello_3_init); 
module_exit(hello_3_exit); 
 
MODULE_LICENSE("GPL");
