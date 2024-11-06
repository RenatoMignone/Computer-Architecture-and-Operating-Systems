#include "usart.h"
#include "delay.h"
#include <stdint.h>

#define BUFFER_SIZE 100

int main(void) {
    // Initialize the usart
    uint8_t *msg_0 = (uint8_t *)"Hello, World\n";
    usart_tx_start(msg_0, BUFFER_SIZE);
    uint8_t *msg_1 = (uint8_t *)"Hola, Mundo\n";
    usart_tx_start(msg_1, BUFFER_SIZE);
    uint8_t *msg_2 = (uint8_t *)"Ciao, Mondo\n";
    usart_tx_start(msg_2, BUFFER_SIZE);

}
