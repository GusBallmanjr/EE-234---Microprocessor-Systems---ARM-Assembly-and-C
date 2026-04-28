#include "XADC.h"


XADC_Registers XADC_REG = {
    .TEMP = (uint32_t*) 0x43C50200,
    .VCCINT = (uint32_t*) 0x43C50204,
    .VP =  (uint32_t*) 0x43C5020C,
    .VCCINTPS = (uint32_t*) 0x43C50234,
    .CR0 = (uint32_t*) 0x43C50300
};

GPIO_registers GPIO_REG = {
    .Buttons = (uint32_t*) 0x41200000,
    .LEDs = (uint32_t*) 0x41210000,
    .Switches = (uint32_t*) 0x41220000,
    .DATA_OUT = (uint32_t*) 0xE000A040,
    .DATA_IN = (uint32_t*) 0xE000A060,
    .DIRECTION = (uint32_t*) 0xE000A204,
    .OUT_ENABLE = (uint32_t*) 0xE000A208,
    .RGB_EN = {
        .Reg1 = (uint32_t*) 0x43C00000,
        .Reg2 = (uint32_t*) 0x43C00010,
        .Reg3 = (uint32_t*) 0x43C00020,
        .Reg4 = (uint32_t*) 0x43C00030,
        .Reg5 = (uint32_t*) 0x43C00040,
        .Reg6 = (uint32_t*) 0x43C00050  },
    .RGB_PERIOD = {
        .Reg1 = (uint32_t*) 0x43C00004,
        .Reg2 = (uint32_t*) 0x43C00014,
        .Reg3 = (uint32_t*) 0x43C00024,
        .Reg4 = (uint32_t*) 0x43C00034,
        .Reg5 = (uint32_t*) 0x43C00044,
        .Reg6 = (uint32_t*) 0x43C00054  },
    .RGB_WIDTH = {
        .Reg1 = (uint32_t*) 0x43C00008,
        .Reg2 = (uint32_t*) 0x43C00018,
        .Reg3 = (uint32_t*) 0x43C00028,
        .Reg4 = (uint32_t*) 0x43C00038,
        .Reg5 = (uint32_t*) 0x43C00048,
        .Reg6 = (uint32_t*) 0x43C00058  }
};


void configure_RGBs(uint32_t period, uint32_t width) {
	*GPIO_REG.RGB_PERIOD.Reg1 = period;
	*GPIO_REG.RGB_PERIOD.Reg2 = period;
	*GPIO_REG.RGB_PERIOD.Reg3 = period;
	*GPIO_REG.RGB_PERIOD.Reg4 = period;
	*GPIO_REG.RGB_PERIOD.Reg5 = period;
	*GPIO_REG.RGB_PERIOD.Reg6 = period;

	*GPIO_REG.RGB_WIDTH.Reg1 = width;
	*GPIO_REG.RGB_WIDTH.Reg2 = width;
	*GPIO_REG.RGB_WIDTH.Reg3 = width;
	*GPIO_REG.RGB_WIDTH.Reg4 = width;
	*GPIO_REG.RGB_WIDTH.Reg5 = width;
	*GPIO_REG.RGB_WIDTH.Reg6 = width;
}

void XADC_Channel_Select(uint32_t channel) {
	uint32_t temporary;
	channel &= 0x1F;

	temporary = *XADC_REG.CR0;

	temporary &= 0xFFE0;
	temporary |= channel;

	*XADC_REG.CR0 = temporary | (0x3 << 12); // Channel plus averaging
}

uint32_t get_XADC_VP_DATA(uint32_t channel) {
	XADC_Channel_Select(channel);
	for (volatile uint32_t i = 0; i < 1000; i++);
	uint32_t data = 0;
	data = (*XADC_REG.VP >> 4) & 0xFFF;
	return data;
}
