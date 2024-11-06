#include <stdio.h>
#include "usart.h"

uint32_t usart_tx_start(const uint8_t *data_bytes, uint32_t n_bytes) {

  //checking for the null pointer error
    if (data_bytes == NULL) {
        return USART_TX_ERROR;  // Return an error if the string pointer is NULL
                                // NO DATA TO TRANSMIT
    }

    for (int i = 0; i < n_bytes; i++) {
        // Wait until the transmit buffer is empty
        // If the buffer is not ready, wait for a timeout
        // Optionally add a timeout to avoid infinite waiting
        
        int timeout = TIMEOUT;
        while (!usart_is_tx_ready()) {
            if (timeout == 0) {
                return USART_TX_BUSY;
            }
            timeout = timeout - 1;
        }

        // Transmit the current byte
        usart1_t->data_reg = (unsigned int)(*data_bytes);
        // Move to the next byte
        data_bytes++;

        // If the null-terminator is encountered early, return incomplete status
        if (*data_bytes == '\0' && i < n_bytes - 1) {
            return USART_TX_INCOMPLETE;
        }
    }

    return USART_TX_COMPLETE;
}

// Function to check if transmit buffer is ready
static uint32_t usart_is_tx_ready(void) {
    int ret = 0;
    // Bit 7 of the status register indicates if the transmit buffer is empty
    // << = left shift bitwise operation in C (number << bits to shift)
    if(usart1_t->status_reg & (1 << 7)) {
        ret = 1;
    }
    return ret;
}


