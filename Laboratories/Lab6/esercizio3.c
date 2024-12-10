#include <linux/init.h> 
#include <linux/kernel.h> /* for ARRAY_SIZE() */ 
#include <linux/module.h> 
#include <linux/moduleparam.h> 
#include <linux/printk.h> 
#include <linux/stat.h> 
#include <linux/delay.h>
 
MODULE_LICENSE("GPL");

static int myintarray[5] = {0,0,0,0,0}; 
static int arr_argc = 0;
static int sum = 0;

module_param_array(myintarray, int, &arr_argc, 0644); 
MODULE_PARM_DESC(myintarray, "An array of integers");


static int __init esercizio3_init(void){

    for(int i=0; i<ARRAY_SIZE(myintarray);i++){
        sum+=myintarray[i];
    }

    return 0;
}

static void __exit esercizio3_exit(void){
    pr_info("The sum of the values in the array is: %d\n",sum);
}

module_init(esercizio3_init); 
module_exit(esercizio3_exit);
