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
    return ((*(U1.STATUS) & 0x10) == 0); // Transmitter Full bit from FIFO Status Register
    // returns 1 if bit is not set (TX is not full)
}

uint32_t UART1_empty_RX(void) {
    return ((*(U1.STATUS) & 0x2) == 0); // Receiver empty bit from FIFO Status Register
    // returns 1 if bit is not set (RX is not Empty)
}

char UART1_getC(void) {
    while(!UART1_empty_RX());
    return (char)*(U1.BASE + (0x30/4)); // FIFO Register
}

void UART1_putC(char data) {
    while(!UART1_empty_TX());

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

void UART1_sendInt(uint8_t integer, uint8_t signed_variable) {

	if (signed_variable != 0 && integer < 0) {
        UART1_putS("-");
        integer = -integer;
    }

    if (integer >= 100) {
        while(!UART1_empty_TX());
        *(U1.BASE + (0x30/4)) = (uint8_t)((integer / 100) + '0');
        integer %= 100;
    }

    if (integer >= 10) {
        while(!UART1_empty_TX());
        *(U1.BASE + (0x30/4)) = (uint8_t)((integer / 10) + '0');
        integer %= 10;
    }

    while(!UART1_empty_TX());
    *(U1.BASE + (0x30/4)) = (uint8_t)(integer + '0');
}


uint8_t UART1_getInt(void) {
    while(!UART1_empty_RX());
    return (uint8_t) (*(U1.BASE + (0x30/4)) - '0');
}

void UART1_commands(void) {
    char str[9] = "", command[10] = "", cmd[5] = "";
    uint32_t i = 0;
    UART1_getln(str, 15);

    for (i = 0; i < 6; i++) {
        command[i] = str[i];
    }

    for(uint32_t j = 0; j < 3; j++) {
        cmd[j] = str[i];
        i++;
    }

    if (command == "sensors") {
        uint8_t address = UART1_getInt();
        sensor_commands(cmd, address);
    }
    //else if (command == "servo") {
     //   servo_commands(cmd);
    //}
}

void set_rx_trigger (uint32_t n) {
	*U1.RXTG = n; // set level at which RX bit will trigger
}

void enable_UART1_rx(void) {
	*U1.IDR = 0xFFFFFFFF; // Disable all interrupts
	*U1.IER = 0x1; // Enable RX Trigger interrupt
	set_rx_trigger(1);
}

void clear_UART1_interrupt(void) {
	*U1.ISR = *U1.ISR;
}

void handle_UART1_interrupt(void) {

	UART1_commands();
	clear_UART1_interrupt();
}
