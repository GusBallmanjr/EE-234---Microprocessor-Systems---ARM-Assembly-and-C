#ifndef SENSORS_H
#define SENSORS_H
#include "SPI0.h"
#include "data.h"
#include "UART1.h"

typedef struct Accelerometer_Gyro_Registers_values {
    uint8_t ID;
    uint8_t CTRL_REG1_G;
    uint8_t TEMP_OUT;
    uint8_t OUT_X_G;
    uint8_t OUT_Y_G;
    uint8_t OUT_Z_G;
    uint8_t CTRL_REG6_XL;
    uint8_t OUT_X_XL;
    uint8_t OUT_Y_XL;
    uint8_t OUT_Z_XL;
} Accelerometer_Gyro_Registers_values;

typedef struct Magnetometer_Registers_values {
    uint8_t ID;
    uint8_t CTRL_REG3_M;
    uint8_t OUT_X_M;
    uint8_t OUT_Y_M;
    uint8_t OUT_Z_M;
} Magnetometer_Registers_values;

extern Accelerometer_Gyro_Registers_values ACC_GYRO_REG;
extern Magnetometer_Registers_values MAGMET_REG;

//void sensor_commands(char cmd[], uint8_t address);
//void print_sensor_cmd(void);

void get_IDs(void);
void configure_sensors(void);
void read_output_data_ag(uint32_t* x, uint32_t* y, uint32_t* z);
void print_AG(void);

#endif
