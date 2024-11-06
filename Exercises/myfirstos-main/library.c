volatile unsigned int *const USART1_PTR = (unsigned int *)0x40011004;


void my_printf(const char *s) {
    while(*s != '\0') { /* Loop until end of string */
        *USART1_PTR= (unsigned int)(*s); /* Transmit char */
        s++; /* Next char */
    }
}


