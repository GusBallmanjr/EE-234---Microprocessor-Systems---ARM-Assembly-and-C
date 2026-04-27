#include "I2C.h"

I2C_registers I2C_REG = {
    .CR = (uint32_t*) 0xE0005000,
    .SR = (uint32_t*) 0xE0005004,
    .ADDR = (uint32_t*) 0xE0005008,
    .DATA = (uint32_t*) 0xE000500C,
    .ISR = (uint32_t*) 0xE0005010,
    .TRANS_SIZE = (uint32_t*) 0xE0005014,
    .SLV_PAUSE = (uint32_t*) 0xE0005018,
    .TIME_OUT = (uint32_t*) 0xE000501C,
    .IMR = (uint32_t*) 0xE0005020,
    .IER = (uint32_t*) 0xE0005024,
    .IDR = (uint32_t*) 0xE0005028
};

Temperature_Sensor_Registers_values TEMP_REG = {
    .TEMP = 0b0,
    .CONFIG = 0b01,
    .THYST = 0b10,
    .TOS = 0b11
};

void unlock_I2C_controller(void) {
    SLCR_UNLOCK = UNLOCK_KEY;	//unlock SLCRs
	SLCR_IIC_RST = 0x3;		//assert I2C reset
	SLCR_IIC_RST = 0;		//deassert I2C reset
	SLCR_LOCK = LOCK_KEY;		//relock SLCRs
}

void configure_I2C(void) {

    uint32_t DivA = 0;
    uint32_t DivB = 12;

    unlock_I2C_controller();

    *I2C_REG.CR = (1 << 6);

    *I2C_REG.CR = (DivA << 14) | (DivB << 8)  | (1 << 3) | (1 << 2) | (1 << 1);
}

void start_read_I2C(uint8_t address, uint32_t size) {
	uint16_t value = 0;
	*I2C_REG.CR |= 1;
	*I2C_REG.TRANS_SIZE = size;
	*I2C_REG.ADDR = (address & 0x7F);
}

uint16_t read_I2C_data(uint8_t address, uint32_t size) {

	uint16_t value = 0;
    *I2C_REG.CR |= 1;
    *I2C_REG.TRANS_SIZE = size;
    *I2C_REG.ADDR = (address & 0x7F);

    for(uint32_t i = 0; i < size; i++) {
        uint8_t byte = *I2C_REG.DATA;
        value = (value << 8) | byte;
        *I2C_REG.ISR = (1 << 3);
    }

    return value;
}

void write_I2C_data(uint8_t address, uint8_t *data, uint32_t size) {

    *I2C_REG.CR &= ~1;
    *I2C_REG.ADDR = (address & 0x7F);
    for(uint32_t i = 0; i < size; i++) {
        *I2C_REG.DATA = data[i];
    }
    while(((*I2C_REG.ISR) & (1 << 9)) == 0);
    *I2C_REG.ISR = (1 << 9);
}

void enable_I2C_interrupts(void) {

    *I2C_REG.IER = 0x2; // More Data Interrupt
}

void disable_I2C_interrupts(void) {
    *I2C_REG.IDR = 0x3FF;
}

void clear_I2C_interrupts(void) {
    *I2C_REG.ISR = 0x3FF;
}

void read_temp_data(char* data_str) {
	uint32_t data = read_I2C_data(0x48, 2);
	int_hexString(data, data_str);
}

void handle_I2C_interrupt(void) {
	uint16_t temp_data = 0, value = 0;

	for(uint32_t i = 0; i < 2; i++) {
		temp_data = *I2C_REG.DATA;
		temp_data >>= 5;
		value = (value << 8) | temp_data;
	}

	UART1_putS("TEMP: ");
	UART1_sendInt(value >> 3, 1);
	UART1_putS("\n");
	clear_I2C_interrupts();
}
