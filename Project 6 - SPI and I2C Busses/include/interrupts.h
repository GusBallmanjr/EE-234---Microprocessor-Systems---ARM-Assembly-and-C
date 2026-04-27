#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include <xil_exception.h>
#include "UART1.h"
#include "GPIO.h"
#include "I2C.h"

#define UART1_INTERRUPT_ID 82
#define GPIO_INTERRUPT_ID 52
#define GTC_INTERRUPT_ID 27
#define SPI0_INTERRUPT_ID 58
#define I2C_INTERRUPT_ID 80

typedef struct GIC_Configuration_Registers {
	volatile uint32_t* ICCPMR;
	volatile uint32_t* ICCICR;
	volatile uint32_t* ICCIAR;
	volatile uint32_t* ICCEOIR;
	volatile uint32_t* ICCHPIR;
	volatile uint32_t* ICDDCR;
	volatile uint32_t* ICDISER;
	volatile uint32_t* ICDICER;
	volatile uint32_t* ICDIPR;
	volatile uint32_t* ICDIPTR;
	volatile uint32_t* ICDICFR;
} GIC_Configuration_Registers;

void disable_ARM_interrupts(void);
void enable_ARM_interrupts(void);
void disable_GIC_distributor(void);
void enable_GIC_distributor(void);
void disable_IRQ_passthrough(void);
void enable_IRQ_passthrough(void);
void set_GIC_priority_mask(uint32_t priority);
void disable_interrupt(uint32_t reg_index, uint32_t reg, uint32_t bit_index, uint32_t shift);
void enable_interrupt(uint32_t reg_index, uint32_t reg, uint32_t bit_index, uint32_t shift);
void set_interrupt_priority(uint32_t reg, uint32_t shift, uint32_t priority);
void set_interrupt_sensitivity(uint32_t interrupt_num, uint8_t sensitivity);
void initialize_interrupt_system(void);
void configure_interrupt(uint32_t interrupt_id, uint32_t priority, uint32_t sensitivity);
void ISR(void* data);

extern GIC_Configuration_Registers GIC_REG;


#endif
