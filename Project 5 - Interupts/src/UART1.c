#include "UART1.h"

UART1 U1 = {
    .BASE = (uint32_t*)0xE0001000,
    .STATUS = (uint32_t*)0xE000102C,
    .IMR = (uint32_t*)0xE0001010,
    .IER = (uint32_t*)0xE0001008,
    .IDR = (uint32_t*)0xE000100C,
    .ISR = (uint32_t*)0xE0001014,
    .RXTG = (uint32_t*)0xE0001020
};

void UART1_reset(void) {
    *U1.BASE = 3;
}

void configure_UART1() {
    UART1_reset();
    *(U1.BASE + (0x34/4)) = BAUD_DIV;

    *(U1.BASE + (0x18/4)) = BAUDGEN;

    *(U1.BASE + (0x4/4)) = 0x020;
}

uint32_t UART1_empty_TX(void) {
    if ((*(U1.BASE + (0x2C/4)) & 0x10) != 0) return 1; // Transmitter Full bit from FIFO Status Register
    else return 0;
}

uint32_t UART1_empty_RX(void) {
    if ((*(U1.BASE + (0x2C/4)) & 0x2) != 0) return 1; // Receiver empty bit from FIFO Status Register
    else return 0;
}

char UART1_getC(void) {
    while(UART1_empty_RX());
    return (char)*(U1.BASE + (0x30/4)); // FIFO Register
}

void UART1_putC(char data) {
    while(UART1_empty_TX());

    *(U1.BASE + (0x30/4)) = (uint32_t)data; // FIFO Register
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
    UART1_getC(); // get rid of enter at beginning
    for (i = 0; i < max - 1; i++) {
    	char c = UART1_getC();
    	if (c == '\n' || c == '\0' || c == '\r') {
    		return i;
    	}
    	str[i] = c;
    }
    str[i] = '\0';
    return i;
}


void UART1_sendInt(uint8_t integer) {
	while(UART1_empty_TX());

	*(U1.BASE + (0x30/4)) = (uint8_t)(integer + '0'); // FIFO Register
}
