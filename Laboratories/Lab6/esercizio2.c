#include <linux/init.h> 
#include <linux/kernel.h> /* for ARRAY_SIZE() */ 
#include <linux/module.h> 
#include <linux/moduleparam.h> 
#include <linux/printk.h> 
#include <linux/stat.h> 
#include <linux/delay.h>
 
MODULE_LICENSE("GPL");


static char *mystring = "";
static int myint = 0;
static int flag = 0;

module_param(myint, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
MODULE_PARM_DESC(myint, "An integer");

module_param(flag, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
MODULE_PARM_DESC(flag, "The flag");

module_param(mystring, charp, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
MODULE_PARM_DESC(mystring, "My string");





static int __init ex2_init(void){

    while(flag){
        pr_info("My string is: %s\n",mystring);
        msleep(2000);
    }

    return 0;
}




static void __exit ex2_exit(void){
    pr_info("GoodBye World\n");
}



module_init(ex2_init);
module_exit(ex2_exit);
