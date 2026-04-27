#ifndef GTC_H
#define GTC_H
#include <stdint.h>

typedef struct Global_Timer_Registers {
    volatile uint32_t* DR0;
    volatile uint32_t* DR1;
    volatile uint32_t* CR;
    volatile uint32_t* ISR;
    volatile uint32_t* COMP0;
    volatile uint32_t* COMP1;
    volatile uint32_t* AI;
} Global_Timer_Registers;

extern Global_Timer_Registers GTC_REG;

void disable_GTC(void);
void enable_GTC(void);
void set_count_GTC(uint64_t count);
void clear_GTC_interrupt(void);
uint64_t get_GTC_count(void);
void configure_GTC(void);
void handle_GTC_interrupt(void);

#endif
