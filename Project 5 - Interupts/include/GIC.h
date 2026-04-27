#ifndef GIC_H
#define GIC_H

#include <stdint.h>

typedef struct GIC_Configuration_Registers {
    uint32_t* ICCPMR;
    uint32_t* ICCICR;
    uint32_t* ICCIAR;
    uint32_t* ICCEOIR;
    uint32_t* ICCHPIR;
    uint32_t* ICDDCR;
    uint32_t* ICDISER;
    uint32_t* ICDICER;
    uint32_t* ICDIPR;
    uint32_t* ICDIPTR;
    uint32_t* ICDICFR;
} GIC_Configuration_Registers;

GIC_Configuration_Registers GIC_REG = { 
    .ICCPMR = (uint32_t*)0xF8F00104,
    .ICCICR = (uint32_t*)0xF8F00100,
    .ICCIAR = (uint32_t*)0xF8F0010C,
    .ICCEOIR = (uint32_t*)0xF8F00110,
    .ICCHPIR = (uint32_t*)0xF8F00118,
    .ICDDCR = (uint32_t*)0xF8F01000,
    .ICDISER = (uint32_t*)0xF8F01100,
    .ICDICER = (uint32_t*)0xF8F01180,
    .ICDIPR = (uint32_t*)0xF8F01400,
    .ICDIPTR = (uint32_t*)0xF8F01800,
    .ICDICFR = (uint32_t*)0xF8F01C00
};

#endif
