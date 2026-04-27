#ifndef SPI0_H
#define SPI0_H
#include <stdint.h>

//SLCR addresses for SPI reset
#define SLCR_LOCK		*( (uint32_t *) 0xF8000004)
#define SLCR_UNLOCK		*( (uint32_t *) 0xF8000008)
#define SLCR_SPI_RST 	*( (uint32_t *) 0xF800021C)

//SLCR lock and unlock keys
#define UNLOCK_KEY	0xDF0D
#define LOCK_KEY	0x767B

typedef struct SPI_Configuration_Status_registers {
    volatile uint32_t* CR;
    volatile uint32_t* SR;
    volatile uint32_t* IER;
    volatile uint32_t* IDR;
    volatile uint32_t* IMR;
    volatile uint32_t* ER;
    volatile uint32_t* DR;
    volatile uint32_t* TXD;
    volatile uint32_t* RXD;
    volatile uint32_t* SICR;
    volatile uint32_t* TXWR;
    volatile uint32_t* RX_THRESH;
    volatile uint32_t* MOD_ID;
}  SPI_Configuration_Status_registers;

extern SPI_Configuration_Status_registers SPI0_REG;

void reset_SPI(void);
void configure_SPI0(void);
void enable_SPI0_interrupt(void);
void clear_SPI0_interrupt(void);
void set_SPI0_RX_threshold(uint32_t threshold);
uint8_t get_SPI0_data (uint8_t address);
void send_SPI0_data(uint8_t address, uint8_t data);
void SPI0_slave_select(uint32_t select);
void handle_SPI0_interrupt(void);
//void SPI0_commands(char cmd[], uint8_t address);
//void print_SPI0_cmd(void);


#endif
