#include <linux/init.h> 
#include <linux/kernel.h> /* for ARRAY_SIZE() */ 
#include <linux/module.h> 
#include <linux/moduleparam.h> 
#include <linux/printk.h> 
#include <linux/stat.h> 

MODULE_LICENSE("GPL");

static char *mystring="";

module_param(mystring, charp, 0644);
MODULE_PARM_DESC(mystring, "Name of a person");

static int __init ex1_init(void){
    
    pr_info("This is the name of the Person: %s\n",mystring);

    return 0;
}

static void __exit ex1_exit(void){
    pr_info("GoodBye, world");
}

module_init(ex1_init);
module_exit(ex1_exit);
