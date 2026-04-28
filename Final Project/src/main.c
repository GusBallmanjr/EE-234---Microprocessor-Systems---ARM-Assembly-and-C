#include "XADC.h"


int main(void) {

	configure_RGBs(1024, 512);

	for (;;) {

    	if ((*GPIO_REG.Buttons & 0xF) == 0x1) {
    		*GPIO_REG.LEDs = (*GPIO_REG.Switches & 0xFFF);
    	}
    	else if ((*GPIO_REG.Buttons & 0xF) == 0x2) {
            *GPIO_REG.LEDs = get_XADC_VP_DATA(VP_Channel);
        }

    	if ((*GPIO_REG.Switches & 0xFFF) > get_XADC_VP_DATA(VP_Channel)) {
    		*GPIO_REG.RGB_EN.Reg1 = 0x0; // Blue
    		*GPIO_REG.RGB_EN.Reg2 = 0x0; // Green
    		*GPIO_REG.RGB_EN.Reg3 = 0x1; // Red
    	}
    	else if ((*GPIO_REG.Switches & 0xFFF) < get_XADC_VP_DATA(VP_Channel)) {
    		*GPIO_REG.RGB_EN.Reg1 = 0x1;
    		*GPIO_REG.RGB_EN.Reg2 = 0x0;
    		*GPIO_REG.RGB_EN.Reg3 = 0x0;
    	}
    	else { // Switches and ADC are equal
    		*GPIO_REG.RGB_EN.Reg1 = 0x0;
    		*GPIO_REG.RGB_EN.Reg2 = 0x1;
    		*GPIO_REG.RGB_EN.Reg3 = 0x0;
    	}
    }

    return 0;
}
