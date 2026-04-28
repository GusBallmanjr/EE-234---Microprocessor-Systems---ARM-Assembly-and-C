#ifndef XADC_H
#define XADC_H

#include <stdint.h>

#define TEMP_SENSOR_Channel 0x0
#define VCCINT_Channel 0x1
#define VCCAUX_Channel 0x2
#define VP_Channel 0x3

typedef struct XADC_Registers {
    volatile uint32_t* TEMP;
    volatile uint32_t* VCCINT;
    volatile uint32_t* VP;
    volatile uint32_t* VCCINTPS;
    volatile uint32_t* CR0;
} XADC_Registers;

typedef struct RGB_registers_EN {
    volatile uint32_t* Reg1;
    volatile uint32_t* Reg2;
    volatile uint32_t* Reg3;
    volatile uint32_t* Reg4;
    volatile uint32_t* Reg5;
    volatile uint32_t* Reg6;
} RGB_registers_EN;

typedef struct RGB_registers_PERIOD {
    volatile uint32_t* Reg1;
    volatile uint32_t* Reg2;
    volatile uint32_t* Reg3;
    volatile uint32_t* Reg4;
    volatile uint32_t* Reg5;
    volatile uint32_t* Reg6;
} RGB_registers_PERIOD;

typedef struct RGB_registers_WIDTH {
    volatile uint32_t* Reg1;
    volatile uint32_t* Reg2;
    volatile uint32_t* Reg3;
    volatile uint32_t* Reg4;
    volatile uint32_t* Reg5;
    volatile uint32_t* Reg6;
} RGB_registers_WIDTH;

typedef struct GPIO_registers {
    volatile uint32_t* Buttons;
    volatile uint32_t* LEDs;
    volatile uint32_t* Switches;
    volatile uint32_t* DATA_OUT;
    volatile uint32_t* DATA_IN;
    volatile uint32_t* DIRECTION;
    volatile uint32_t* OUT_ENABLE;
    RGB_registers_EN RGB_EN;
    RGB_registers_PERIOD RGB_PERIOD;
    RGB_registers_WIDTH RGB_WIDTH;
} GPIO_registers;

extern XADC_Registers XADC_REG;
extern GPIO_registers GPIO_REG;

void configure_RGBs(uint32_t period, uint32_t width);
void XADC_Channel_Select(uint32_t channel);
uint32_t get_XADC_VP_DATA(uint32_t channel);

#endif
