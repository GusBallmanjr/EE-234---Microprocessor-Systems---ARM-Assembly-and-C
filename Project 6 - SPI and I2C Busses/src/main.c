#include "interrupts.h"


int main(void) {

	configure_UART1();
	configure_SPI0();
	configure_sensors();
	configure_GTC();
	configure_I2C();
	initialize_interrupt_system();

	// Task 1:
	//UART1_putS("Task 1:\n");
	//get_IDs();
	//UART1_putS("\nTask 2:\n");

	// Task 2
	//print_AG();
	//UART1_putS("Task 3:\n");

	// Task 3:


	for (;;) {
	}

    return 0;
}
