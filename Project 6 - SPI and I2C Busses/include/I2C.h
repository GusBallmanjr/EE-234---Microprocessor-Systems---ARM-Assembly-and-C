#ifndef I2C_H
#define I2C_H
#include <stdint.h>

#define SLCR_LOCK	*( (uint32_t *) 0xF8000004)
#define SLCR_UNLOCK	*( (uint32_t *) 0xF8000008)
#define SLCR_IIC_RST 	*( (uint32_t *) 0xF8000224)
#define UNLOCK_KEY	0xDF0D
#define LOCK_KEY	0x767B

typedef struct I2C_registers {
    volatile uint32_t* CR;
    volatile uint32_t* SR;
    volatile uint32_t* ADDR;
    volatile uint32_t* DATA;
    volatile uint32_t* ISR;
    volatile uint32_t* TRANS_SIZE;
    volatile uint32_t* SLV_PAUSE;
    volatile uint32_t* TIME_OUT;
    volatile uint32_t* IMR;
    volatile uint32_t* IER;
    volatile uint32_t* IDR;
} I2C_registers;

typedef struct Temperature_Sensor_Registers_values {
    uint8_t TEMP;
    uint8_t CONFIG;
    uint8_t THYST;
    uint8_t TOS;
} Temperature_Sensor_Registers_values;

extern I2C_registers I2C_REG;
extern Temperature_Sensor_Registers_values TEMP_REG;

void unlock_I2C_controller(void);
void configure_I2C(void);
void start_read_I2C(uint8_t address, uint32_t size);
uint16_t read_I2C_data(uint8_t address, uint32_t size);
void write_I2C_data(uint8_t address, uint8_t *data, uint32_t size);
void enable_I2C_interrupts(void);
void disable_I2C_interrupts(void);
void clear_I2C_interrupts(void);
void read_temp_data(char* data_str);
void handle_I2C_interrupt(void);

#endif
