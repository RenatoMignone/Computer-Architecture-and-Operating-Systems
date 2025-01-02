/* Needed by all modules */
#include <linux/module.h>

/* Needed for pr_info() */
#include <linux/printk.h>

#include <linux/io.h>

#include "uart.h"

/*Limit the scope of the variable to this file*/
static void __iomem *mapped_addr;

int init_module(void){

	Uart_TxRx *pl011;

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

	uart_tx(pl011, "Hello world!\n", 14);	

	return 0;
}

void cleanup_module(void){

	if (mapped_addr) {
		iounmap(mapped_addr);
		pr_info("Unmapped addredd\n");
	}

	pr_info("Exiting from the module!\n");
}

MODULE_LICENSE("GPL");
