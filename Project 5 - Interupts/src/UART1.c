#include "UART1.h"

void UART1_reset(void) {
    *U1.BASE = 3;
}

void configure_UART1() {
    UART1_reset();
    *(U1.BASE + 0x34) = BAUD_DIV;

    *(U1.BASE + 0x18) = BAUDGEN;

    *(U1.BASE + 0x4) = 0x020;
}

uint32_t UART1_empty_TX(void) {
    if ((*(U1.BASE + 0x2C) & 0x10) != 0) return 1; // Transmitter Full bit from FIFO Status Register
    else return 0;
}

uint32_t UART1_empty_RX(void) {
    if ((*(U1.BASE + 0x2C) & 0x4) != 0) return 1; // Receiver Full bit from FIFO Status Register
    else return 0;
}

char UART1_getC(void) {
    while(!UART1_empty_RX());
    return (char)*(U1.BASE + 0x30); // FIFO Register
}

void UART1_putC(char data) {
    while(UART1_empty_TX());

    *(U1.BASE + 0x30) = (uint32_t)data; // FIFO Register
}

void UART1_putS(char str[]) {
    uint32_t i = 0;
    while (str[i] != '\0') {
    	UART1_putC(str[i]);
    	i++;
    }
}

uint32_t UART1_getln(char str[], uint32_t max) {
    uint32_t i = 0;
    for (i = 0; i < max; i++) {
        str[i] = UART1_getC();
        if (str[i] == '\n') {
            str[i] = '\0';
            break;
        };
    }
    return i;
}
