#include "GTC.h"

Global_Timer_Registers GTC_REG = {
    .DR0 = (uint32_t*) 0xF8F00200,
    .DR1 = (uint32_t*) 0xF8F00204,
    .CR = (uint32_t*) 0xF8F00208,
    .ISR = (uint32_t*) 0xF8F0020C,
    .COMP0 = (uint32_t*) 0xF8F00210,
    .COMP1 = (uint32_t*) 0xF8F00214,
    .AI = (uint32_t*) 0xF8F00218
};

void disable_GTC(void) {
    *GTC_REG.CR = 0x0;
}

void enable_GTC(void) {
    *GTC_REG.CR = 0x7;
}

void set_count_GTC(uint64_t count) {
    *GTC_REG.COMP0 = (uint32_t)(count & 0xFFFFFFFF);
    *GTC_REG.COMP1 = (uint32_t)((count >> 32) & 0xFFFFFFFF);
}

void clear_GTC_interrupt(void) {
    *GTC_REG.ISR = 0x1;
}

uint64_t get_GTC_count(void) {
    uint64_t low = (uint64_t)(*GTC_REG.DR0);
    uint64_t high = (uint64_t)(*GTC_REG.DR1);
    return (high << 32) | low;
}

void configure_GTC(void) {
    disable_GTC();
    clear_GTC_interrupt();
    set_count_GTC(0x13D92D49); // 333333333 count
    enable_GTC();
}

void handle_GTC_interrupt(void) {

	uint64_t count = get_GTC_count();

	//UART1_sendIntLONG((uint32_t)(count & 0xFFFFFFFF));
	UART1_putC('\n');

	set_count_GTC(count + 0x13D92D49);
	clear_GTC_interrupt();
}
