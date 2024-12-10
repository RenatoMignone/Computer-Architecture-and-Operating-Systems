#include <linux/init.h>
#include <linux/kernel.h> /* for ARRAY_SIZE() */
#include <linux/module.h> 
#include <linux/moduleparam.h> 
#include <linux/printk.h> 
#include <linux/stat.h> 
#include "string_rev.h"


MODULE_LICENSE("GPL");

static char *mystring = "";


module_param(mystring, charp, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
MODULE_PARM_DESC(mystring, "My string");


static int __init esercizio4_init(void){
    char *c = reverse_string(mystring);

    pr_info("This is the reversed String: %s\n", c);
    return 0;
}


module_init(esercizio4_init);



