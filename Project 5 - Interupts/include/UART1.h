#ifndef UART1_H
#define UART1_H
#include <stdint.h>

typedef struct UART1 {
    uint32_t* BASE;
    uint32_t* STATUS;
    uint32_t* IMR;
    uint32_t* IER;
    uint32_t* IDR;
    uint32_t* ISR;
    uint32_t* RXTG;
} UART1;

UART1 U1 = { 
    .BASE = (uint32_t*)0xE0001000,
    .STATUS = (uint32_t*)0xE000102C,
    .IMR = (uint32_t*)0xE0001010,
    .IER = (uint32_t*)0xE0001008,
    .IDR = (uint32_t*)0xE000100C,
    .ISR = (uint32_t*)0xE0001014,
    .RXTG = (uint32_t*)0xE0001020
};

#define BAUD_DIV 6
#define BAUDGEN 0x70

void UART1_reset(void);
void configure_UART1(void);
uint32_t UART1_empty_TX(void);
uint32_t UART1_empty_RX(void);
char UART1_getC(void);
void UART1_putC(char data);
void UART1_putS(char str[]);
uint32_t UART1_getln(char str[], uint32_t max);

#endif