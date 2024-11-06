#include <stdint.h>
//ifndef is a preprocessor directive, to tell him if not defined, he has to use the code between it and #endif
#ifndef USART_H
#define USART_H

// Define the timeout value for the USART
#define TIMEOUT 1000
// Define the base address for the USART1 peripheral
#define USART1_BASE 0x40011000


// Define the return values for the uart
enum {
  USART_TX_ERROR = -1,
  USART_TX_INCOMPLETE = 0,
  USART_TX_COMPLETE = 1,
  USART_TX_BUSY = 2,
};

// Define a struct for USART registers
typedef struct {
    volatile uint32_t status_reg;   // Status register (Offset 0x00) this register is full of flags that 
                                    // represent the status of the register
    volatile uint32_t data_reg;     // Data register (Offset 0x04:) data to be transmitted or received
    // Other USART registers can be added as needed
} usart_reg;

// Define a pointer to the USART1 registers at the base address 0x40011000
// This pointer is declared as volatile to prevent the compiler from optimizing it
// away in certain situations
// The pointer is declared as const to prevent modification of the pointer itself
// The pointer is declared as static to limit the scope of the pointer to this file and can't be
// accessed from other files
// USART is memory-mapped, this means that it can be accessed by just pointing to a given address 
// in the memory of the microcontroller, in this case pointing to the usart1_base address
// we are just accessing to it by pointing that memory address.
static volatile usart_reg *const usart1_t = (usart_reg *)USART1_BASE;

// Function to check if transmit buffer is ready, it is static so it is only visible in this file
static uint32_t usart_is_tx_ready(void);
// Function to transmit a string of bytes
uint32_t usart_tx_start(const uint8_t *data_bytes, uint32_t n_bytes);

#endif // USART_H

//A cast in C (like (usart_reg *)) tells the compiler, "Treat this numeric value as if it points to a usart_reg structure.
//" So after the cast, 0x40011000 is now seen as pointing to the start of a usart_reg structure. 
//That means when you later access usart1_t->status_reg, the compiler knows that 
//status_reg is at offset 0x00 from the base address, and data_reg is at offset 0x04.
