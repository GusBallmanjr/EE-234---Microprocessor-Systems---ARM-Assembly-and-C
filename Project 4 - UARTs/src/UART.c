#include "UART.h"

void UART1_reset(void) {
    uint32_t* UART1 = (uint32_t*)UART1_BASE;
    *UART1 = 3;
}

void configure_UART1() {
    UART1_reset();
    uint32_t* UART1 = (uint32_t*)(UART1_BASE + 0x34);
    *UART1 = BAUD_DIV;

    UART1 = (uint32_t*)(UART1_BASE + 0x18);
    *UART1 = BAUDGEN;

    UART1 = (uint32_t*)(UART1_BASE + 0x4);
    *UART1 = 0x020;
}

uint32_t UART1_empty_TX(void) {
    uint32_t* UART1 = (uint32_t*)(UART1_BASE + 0x2C); // FIFO Status Register

    if ((*UART1 & 0x10) != 0) return 1; // Transmitter Full bit
    else return 0;
}

uint32_t UART1_empty_RX(void) {
    uint32_t* UART1 = (uint32_t*)(UART1_BASE + 0x2C); // FIFO Status Register

    if ((*UART1 & 0x4) != 0) return 1; // Receiver Full bit
    else return 0;
}

char UART1_getC(void) {
    while(!UART1_empty_RX());

    uint32_t* UART1 = (uint32_t*)(UART1_BASE + 0x30); // FIFO Register
    return *UART1;
}

void UART1_putC(char data) {
    while(UART1_empty_TX());

    uint32_t* UART1 = (uint32_t*)(UART1_BASE + 0x30); // FIFO Register
    *UART1 = data;
}

void UART1_putS(char str[]) {
    for (int i = 0; str[i - 1] != '\0'; i++) {
        UART1_putC(str[i]);
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