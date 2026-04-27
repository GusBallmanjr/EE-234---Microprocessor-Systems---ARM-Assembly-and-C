#include "interrupts.h"

GIC_Configuration_Registers GIC_REG = {
    .ICCPMR = (uint32_t*)0xF8F00104,
    .ICCICR = (uint32_t*)0xF8F00100,
    .ICCIAR = (uint32_t*)0xF8F0010C,
    .ICCEOIR = (uint32_t*)0xF8F00110,
    .ICCHPIR = (uint32_t*)0xF8F00118,
    .ICDDCR = (uint32_t*)0xF8F01000,
    .ICDISER = (uint32_t*)0xF8F01100,
    .ICDICER = (uint32_t*)0xF8F01180,
    .ICDIPR = (uint32_t*)0xF8F01400,
    .ICDIPTR = (uint32_t*)0xF8F01800,
    .ICDICFR = (uint32_t*)0xF8F01C00
};

// ****************************************************************************************************************************

void disable_ARM_interrupts(void) {
	uint32_t cpsr_val =0;

		asm("mrs %0, cpsr\n" : "=r" (cpsr_val) );	//get current cpsr

		cpsr_val &= ~(0xFF);	//clear lower 8 bits
		cpsr_val |= 0xDF;	//set all bits but the 'T' bit

		asm("msr cpsr, %0\n" : : "r" (cpsr_val));
}

void enable_ARM_interrupts(void) {
	uint32_t cpsr_val =0;

		asm("mrs %0, cpsr\n" : "=r" (cpsr_val) );	//get current cpsr value

		cpsr_val &= ~(0xFF);	//clear lower 8 bits
		cpsr_val |= 0x5F;	//set all bits but 'T' and 'I' bit

		asm("msr cpsr, %0\n" : : "r" (cpsr_val));
}

// ****************************************************************************************************************************

void disable_GIC_distributor(void) {
    *GIC_REG.ICDDCR = 0x0;
}

void enable_GIC_distributor(void) {
    *GIC_REG.ICDDCR = 0x1;
}

void disable_IRQ_passthrough(void) {
    *GIC_REG.ICCICR = 0x0;
}

void enable_IRQ_passthrough(void) {
	*GIC_REG.ICCICR = 0x1;
}

void set_GIC_priority_mask(uint32_t priority) {
    *GIC_REG.ICCPMR = (priority & 0xFF);
}

// ****************************************************************************************************************************

void disable_interrupt(uint32_t reg_index, uint32_t reg, uint32_t bit_index, uint32_t shift) {
	//*(GIC_REG.ICDIPTR + reg) &= ~(0xFF << shift);
    *(GIC_REG.ICDICER + reg_index) = (0x1 << bit_index);
}

void enable_interrupt(uint32_t reg_index, uint32_t reg, uint32_t bit_index, uint32_t shift) {
	*(GIC_REG.ICDIPTR + reg) &= ~(0xFF << shift);
	*(GIC_REG.ICDIPTR + reg) |= (0x1 << shift);
    *(GIC_REG.ICDISER + reg_index) = (0x1 << bit_index);
}

void set_interrupt_priority(uint32_t reg, uint32_t shift, uint32_t priority) {
    *(GIC_REG.ICDIPR + reg) &= ~(0xFF << shift);
    *(GIC_REG.ICDIPR + reg) |= ((priority & 0xF8) << shift);
}

void set_interrupt_sensitivity(uint32_t interrupt_num, uint8_t sensitivity) {
    *(GIC_REG.ICDICFR + (interrupt_num/16)) &= ~((0x3) << ((interrupt_num % 16) * 2));
    *(GIC_REG.ICDICFR + (interrupt_num/16)) |= ((sensitivity & 0x3) << ((interrupt_num % 16) * 2));
}

void configure_interrupt(uint32_t interrupt_id, uint32_t priority, uint32_t sensitivity) {
	uint32_t reg_index = interrupt_id / 32;
	uint32_t bit_index = interrupt_id % 32;
	uint32_t reg = interrupt_id / 4;
	uint32_t shift = (interrupt_id % 4) * 8;

	disable_interrupt(reg_index, reg, bit_index, shift);
	set_interrupt_priority(reg, shift, priority);
	set_interrupt_sensitivity(interrupt_id, sensitivity);

	enable_interrupt(reg_index, reg, bit_index, shift);
	switch (interrupt_id) {
		case UART1_INTERRUPT_ID:
			enable_UART1_rx();
			break;
		case GPIO_INTERRUPT_ID:
			configure_button_interrupts();
			break;
		default:
			break;
	}
}

// UART ***********************************************************************************************************************

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

// GPIO ***********************************************************************************************************************

void handle_GPIO_interrupt(void)
{
	if(get_btn4_flag()) {
    	write_GPIO_RGB(0, 0x7);
    	clear_GPIO_int_status(BT4_BANK, BT4_MASK);
    }

    if(get_btn5_flag()) {
    	write_GPIO_RGB(0, 0x0);
        clear_GPIO_int_status(BT5_BANK, BT5_MASK);
    }
}

// GTC ************************************************************************************************************************

void handle_GTC_interrupt(void) {

	uint64_t count = get_GTC_count();

	//UART1_sendIntLONG((uint32_t)(count & 0xFFFFFFFF));
	UART1_putC('\n');

	set_count_GTC(count + 0x13D92D49);
	clear_GTC_interrupt();
}
