#include "interupts.h"


int main(void) {

    //setup UART
	configure_uart1();


	//configure interrupt system below
	disable_ARM_interrupts();
	configure_uart1_interrupt();
	enable_ARM_interupts();


	//endless loop
	for(;;);

    return 0;
}