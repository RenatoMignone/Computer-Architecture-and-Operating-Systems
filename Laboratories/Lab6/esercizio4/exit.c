#include <linux/init.h> 
#include <linux/kernel.h> /* for ARRAY_SIZE() */ 
#include <linux/module.h> 
#include <linux/moduleparam.h> 
#include <linux/printk.h> 
#include <linux/stat.h>

MODULE_LICENSE("GPL");


static void __exit esercizio4_exit(void){
    pr_info("Kernel Module removed\n");
}

module_exit(esercizio4_exit);
