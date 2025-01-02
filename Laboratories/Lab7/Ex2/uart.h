#ifndef __UART_H__
#define __UART_H__

#include <linux/io.h>

#define UART_BASE_ADDR 0x09000000
#define UART_LAST_ADDR 0x09000fff
#define UART_SIZE UART_LAST_ADDR - UART_BASE_ADDR

/* Define the timeout value for the UART */
#define TIMEOUT 1000

typedef struct {
    void __iomem *status_reg;
    void __iomem *data_reg;
} Uart_TxRx;

/* Define the return values for the uart */
enum {
  UART_TX_ERROR = -1,
  UART_TX_INCOMPLETE = 0,
  UART_TX_COMPLETE = 1,
  UART_TX_BUSY = 2,
};

void uart_init(Uart_TxRx *uart, void __iomem *base_addr);

uint32_t uart_tx(Uart_TxRx *uart, const uint8_t *str, uint32_t n_bytes);

#endif
