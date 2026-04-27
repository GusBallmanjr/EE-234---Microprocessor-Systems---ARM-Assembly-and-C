#include "GIC.h"

void disable_GIC_distributor(void) {
    *GIC_REG.ICDDCR = 0;
}

void enable_GIC_distributor(void) {
    *GIC_REG.ICDDCR = 1;
}

void disable_IQR_passthrough(void) {
    *GIC_REG.ICCICR = 0x3;
}

void set_GIC_priority_mask(uint32_t priority) {
    *GIC_REG.ICCPMR = (priority & 0xFF);
}
