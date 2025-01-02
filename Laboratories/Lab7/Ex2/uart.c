#include "uart.h"

void uart_init(Uart_TxRx *uart, void __iomem *base_addr){

	if (!uart || !base_addr) {
        pr_err("Invalid arguments: uart=%p, base_addr=%p\n", uart, base_addr);
        return;
    }
	uart -> data_reg = base_addr + 0x0;
	uart -> status_reg = base_addr + 0x18;
}


// Function to check if transmit buffer is ready
static uint32_t uart_is_tx_ready(Uart_TxRx *uart) {

    int ret = 0;

    // Bit 7 of the status register indicates if the transmit buffer is empty
    if(*(int*)(uart -> status_reg) & (1 << 7)) {
        ret = 1;
    }

    return ret;
}

uint32_t uart_tx(Uart_TxRx * uart, const uint8_t *str, uint32_t n_bytes){

    if (str == NULL) {

		// Return an error if the string pointer is NULL: NO DATA TO TRANSMIT
        return UART_TX_ERROR;  
	}

    for (int i = 0; i < n_bytes; i++) {

        int timeout = TIMEOUT;

        // Wait until the transmit buffer is empty
        // If the buffer is not ready, wait for a timeout
        // Optionally add a timeout to avoid infinite waiting
        
        while (!uart_is_tx_ready(uart)) {

            if (timeout == 0) {
                return UART_TX_BUSY;
            }

            timeout = timeout - 1;
        }

        // Transmit the current byte
        writeb(*str, uart -> data_reg);

        // Move to the next byte
        str++;

        // If the null-terminator is encountered early, return incomplete status
        if (*str == '\0' && i < n_bytes - 1) {
            return UART_TX_INCOMPLETE;
        }
    }

    return UART_TX_COMPLETE;
}
