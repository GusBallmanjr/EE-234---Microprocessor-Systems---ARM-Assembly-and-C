#include "interupts.h"
#include "UART1.h"

void disable_ARM_interrupts(void) {
	__asm volatile ("cpsid if"); // ARM Assembly to disable both interupt request and the fast interupt request (f will remain off)
}

void enable_ARM_interupts(void) {
	__asm volatile ("cpsie i"); // enables interupt requests
}

void configure_uart1_interrupt(void) {
	if (*U1.IMR == (1 << 2)) { // rx full
		*GIC_REG.ICCIAR = 82;
	}
}

void set_rx_trigger (uint32_t n) {
	*U1.RXTG = n; // set level at which rx bit will trigger
}

void enable_UART1_rx(void) {
	*U1.IER = 1;
	set_rx_trigger(1);
}

void clear_UART1_interupt(void) {
	U1.ISR = U1.ISR;
}

void handle_UART1_Interupt(void) {
	char collected_char = '\0';

	while(!UART1_empty_RX()) {
		collected_char = UART1_getC();
	}

	clear_UART1_interupt();
}

void IQR_handler(void* handler(void)) { // function pointer as parameter
	Xil_ExceptionRegisterHandler(5, handler, NULL);
}

void handler(void) {
	uint32_t id = *GIC_REG.ICCIAR;


	switch(id) {
        case UART1_INT_ID:
			handle_UART1_Interupt();
            break;        
    }


	//inform GIC that this interrupt has ended
	*GIC_REG.ICCEOIR = id;

}