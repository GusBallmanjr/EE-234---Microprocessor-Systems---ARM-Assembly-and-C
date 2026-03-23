#ifndef INTERUPTS_H
#define INTERUPTS_H
#include "GIC.h"

#define UART1_INT_ID 82

void disable_ARM_interrupts(void);
void enable_ARM_interupts(void);
void configure_uart1_interrupt(void);
void set_rx_trigger (uint32_t n);
void enable_UART1_rx(void);
void clear_UART1_interupt(void);
void handle_UART1_Interupt(void);
void handler(void);

#endif