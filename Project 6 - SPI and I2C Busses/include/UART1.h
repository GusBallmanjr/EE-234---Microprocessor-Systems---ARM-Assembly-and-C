#ifndef UART1_H
#define UART1_H
#include <stdint.h>
#include "sensors.h"

#define BAUD_DIV 6
#define BAUDGEN 0x7C

typedef struct UART1 {
    volatile uint32_t* BASE;
    volatile uint32_t* STATUS;
    volatile uint32_t* IMR;
    volatile uint32_t* IER;
    volatile uint32_t* IDR;
    volatile uint32_t* ISR;
    volatile uint32_t* RXTG;
} UART1;

extern UART1 U1;

void UART1_reset(void);
void configure_UART1(void);
uint32_t UART1_empty_TX(void);
uint32_t UART1_empty_RX(void);
char UART1_getC(void);
void UART1_putC(char data);
void UART1_putS(char str[]);
uint32_t UART1_getln(char str[], uint32_t max);
void UART1_sendInt(uint8_t integer, uint8_t signed_variable);
uint8_t UART1_getInt(void);
void UART1_commands(void);
void set_rx_trigger (uint32_t n);
void enable_UART1_rx(void);
void clear_UART1_interrupt(void);
void handle_UART1_interrupt(void);

#endif
