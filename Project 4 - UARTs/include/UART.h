
#ifndef UART_H
#define UART_H

#include <stdint.h>

#define UART1_BASE 0xE0001000
#define BAUD_DIV 6
#define BAUDGEN 0x70

void UART1_reset(void);
void configure_UART1();
uint32_t UART1_empty_TX(void);
uint32_t UART1_empty_RX(void);
char UART1_getC(void);
void UART1_putC(char data);
void UART1_putS(char buffer[]);
uint32_t UART1_getln(char buffer[], uint32_t max);

#endif