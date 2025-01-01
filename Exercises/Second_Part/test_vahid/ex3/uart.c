#include "uart.h"

void UART_init( void )
{
    UART0_BAUDDIV = 16;
    UART0_CTRL = 1;
}

// void UART_printf(const char *s) {
//     while(*s != '\0') {
//         UART0_DATA = (unsigned int)(*s);
//         s++;
//     }
// }



// Simplified function to output an integer as a string
void UART_print_int(int value) {
    char buffer[12]; // Buffer to store the integer as a string
    char *ptr = buffer + sizeof(buffer) - 1; // Start at the end of the buffer
    *ptr = '\0'; // Null-terminate the string

    if (value == 0) {
        *(--ptr) = '0'; // Handle zero
    } else {
        bool is_negative = (value < 0);
        if (is_negative) {
            value = -value; // Make the value positive
        }

        // Convert the number to a string
        while (value > 0) {
            *(--ptr) = '0' + (value % 10);
            value /= 10;
        }

        if (is_negative) {
            *(--ptr) = '-'; // Add negative sign if needed
        }
    }

    // Send the string
    while (*ptr) {
        UART0_DATA = (unsigned int)(*ptr++);
    }
}

// UART_printf function with %d support
void UART_printf(const char *s, ...) {
    va_list args;
    va_start(args, s);

    while (*s != '\0') {
        if (*s == '%' && *(s + 1) == 'd') {
            s += 2; // Skip "%d"
            int value = va_arg(args, int);
            UART_print_int(value); // Print the integer
        } else {
            UART0_DATA = (unsigned int)(*s); // Send regular characters
            s++;
        }
    }

    va_end(args);
}

