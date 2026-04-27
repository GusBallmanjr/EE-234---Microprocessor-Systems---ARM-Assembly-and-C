#include "SPI0.h"

SPI_Configuration_Status_registers SPI0_REG = {
    .CR = (uint32_t*) 0xE0006000,
    .SR = (uint32_t*) 0xE0006004,
    .IER = (uint32_t*) 0xE0006008,
    .IDR = (uint32_t*) 0xE000600C,
    .IMR = (uint32_t*) 0xE0006010,
    .ER = (uint32_t*) 0xE0006014,
    .DR = (uint32_t*) 0xE0006018,
    .TXD = (uint32_t*) 0xE000601C,
    .RXD = (uint32_t*) 0xE0006020,
    .SICR = (uint32_t*) 0xE0006024,
    .TXWR = (uint32_t*) 0xE0006028,
    .RX_THRESH = (uint32_t*) 0xE000602C,
    .MOD_ID = (uint32_t*) 0xE00060FC };


void reset_SPI(void) {
	uint32_t i = 0;
	SLCR_UNLOCK = UNLOCK_KEY;	//unlock SLCRs
	SLCR_SPI_RST = 0xF;		//assert SPI reset

	for(i = 0; i < 1000; i++);

	SLCR_SPI_RST = 0;		//deassert
	SLCR_LOCK = LOCK_KEY;		//relock SLCRs
}
/*
void BAUD_div_SPI0(uint32_t BAUD_div) {
    *SPI0_REG.CR &= (0xFFFFFFFF & (BAUD_div << 3));
}

void SPI0_Mode(uint32_t mode) {
    *SPI0_REG.CR |= mode;
}
*/
void configure_SPI0(void) {
    reset_SPI();
    *SPI0_REG.ER = 0;
    *SPI0_REG.CR = 0x7827;//0x7E27;
    *SPI0_REG.ER = 1;
    set_SPI0_RX_threshold(1);
}

void enable_SPI0_interrupt(void) {
    *SPI0_REG.IER = 0x4; // Transmit threshold interrupt
}

void clear_SPI0_interrupt(void) {
	*SPI0_REG.SR = 0x1;
}

void set_SPI0_RX_threshold(uint32_t threshold) {
    *SPI0_REG.RX_THRESH = (threshold & 0x7F);
}

uint8_t get_SPI0_data (uint8_t address) {
	uint8_t throwaway = 0;
	while ((*SPI0_REG.SR & 0x8) != 0); // Transmit FIFO full bit
	*SPI0_REG.TXD = address | 0x80;
	while ((*SPI0_REG.SR & 0x10) == 0); // Receive FIFO not empty
	throwaway = *SPI0_REG.RXD;
	while ((*SPI0_REG.SR & 0x8) != 0); // Transmit FIFO empty
	*SPI0_REG.TXD = 0x00;
	while ((*SPI0_REG.SR & 0x10) == 0); // Receive FIFO not empty
    return *SPI0_REG.RXD;
}

void send_SPI0_data(uint8_t address, uint8_t data) {
	while ((*SPI0_REG.SR & 0x8) != 0); // Transmit FIFO empty
	*SPI0_REG.TXD = address;
	while ((*SPI0_REG.SR & 0x8) != 0); // Transmit FIFO empty
	*SPI0_REG.TXD = data;
}

void SPI0_slave_select(uint32_t select) {
    *SPI0_REG.CR &= ~(0xF << 10);
    *SPI0_REG.CR |= ((select & 0xF) << 10);
}

void handle_SPI0_interrupt(void) {
	uint32_t num = (*SPI0_REG.RXD << 8);
	char data[10] = "";
	num |= *SPI0_REG.RXD;
	UART1_putS("X: ");
	int_hexString(num, data);
	UART1_putS(data);
	UART1_putC('\n');

	num = (*SPI0_REG.RXD << 8);
	num |= *SPI0_REG.RXD;
	UART1_putS("Y: ");
	int_hexString(num, data);
	UART1_putS(data);
	UART1_putC('\n');

	num = *SPI0_REG.RXD;
	num |= *SPI0_REG.RXD;
	UART1_putS("Z: ");
	int_hexString(num, data);
	UART1_putS(data);
	UART1_putC('\n');
}
/*
void SPI0_commands(char cmd[], uint8_t address) {
    if (cmd == "-ag") {
        SPI0_slave_select(0);
        UART1_sendInt(get_SPI0_data (address));
    }
    else if (cmd == "-m") {
        SPI0_slave_select(1);
        UART1_sendInt(get_SPI0_data (address));
    }
    else if (cmd == "-h") {
        print_SPI0_cmd();
    }
}

void print_SPI0_cmd(void) {
    UART1_putS("Help:\n\tsensors -ag <address> : get gyro and accelerometer data\n\tsensors -m <address> : get magnetometer data\n");
}
    */
