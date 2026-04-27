#include "sensors.h"

Accelerometer_Gyro_Registers_values ACC_GYRO_REG = {
    .ID = 0x0F,
    .CTRL_REG1_G = 0x10,
    .TEMP_OUT = 0x15,
    .OUT_X_G = 0x18,
    .OUT_Y_G = 0x1A,
    .OUT_Z_G = 0x1C,
    .CTRL_REG6_XL = 0x20,
    .OUT_X_XL = 0x28,
    .OUT_Y_XL = 0x2A,
    .OUT_Z_XL = 0x2C
};

Magnetometer_Registers_values MAGMET_REG = {
    .ID = 0x0F,
    .CTRL_REG3_M = 0x22,
    .OUT_X_M = 0x28,
    .OUT_Y_M = 0x2A,
    .OUT_Z_M = 0x2C
};

void get_IDs(void) {
	uint32_t id1 = 0, id2 = 0;
	char data[10] = "";
	SPI0_slave_select(0);
	id1 = get_SPI0_data(ACC_GYRO_REG.ID);
	SPI0_slave_select(0xF);

	SPI0_slave_select(0x1);
	id2 = get_SPI0_data(MAGMET_REG.ID);
	SPI0_slave_select(0xF);

	UART1_putS("Acc_Gyro ID: ");
	int_hexString(id1, data);
	UART1_putS(data);
	UART1_putS("\nMagnetometer ID: ");
	int_hexString(id2, data);
	UART1_putS(data);
}

void configure_sensors(void) {
    SPI0_slave_select(0);
    send_SPI0_data(0x20, 0x20); // Turn on Acc
    send_SPI0_data(0x10, 0x20); // Turn on Gyro
    SPI0_slave_select(0xF);
    SPI0_slave_select(0x1);
    send_SPI0_data(0x22, 0x00); // Turn on Magnetometer
    SPI0_slave_select(0xF);
}

void read_output_data_ag(uint32_t* x, uint32_t* y, uint32_t* z) {
    SPI0_slave_select(0xE);
    *x = get_SPI0_data(ACC_GYRO_REG.OUT_X_G) | (get_SPI0_data(ACC_GYRO_REG.OUT_X_G + 1) << 8);
    *y = get_SPI0_data(ACC_GYRO_REG.OUT_Y_G) | (get_SPI0_data(ACC_GYRO_REG.OUT_Y_G + 1) << 8);
    *z = get_SPI0_data(ACC_GYRO_REG.OUT_Z_G) | (get_SPI0_data(ACC_GYRO_REG.OUT_Z_G + 1) << 8);
    SPI0_slave_select(0xF);
}

void print_AG(void) {
    uint32_t x = 0, y = 0, z = 0;
    char data[8] = "";
    read_output_data_ag(&x, &y, &z);
    UART1_putS("X: ");
    int_hexString(x, data);
    UART1_putS(data);
    UART1_putC('\n');

    UART1_putS("Y: ");
    int_hexString(y, data);
    UART1_putS(data);
    UART1_putC('\n');

    UART1_putS("Z: ");
    int_hexString(z, data);
    UART1_putS(data);
    UART1_putC('\n');
}

/*
void sensor_commands(char cmd[], uint8_t address) {
    char* output = "";
    if (cmd == "-ag") {
        SPI0_slave_select(0);
        int_hexString(get_SPI0_data(address), output);
        UART1_putS(output);
    }
    else if (cmd == "-m") {
        SPI0_slave_select(1);
        int_hexString(get_SPI0_data(address), output);
        UART1_putS(output);
    }
    else if (cmd == "-h") {
        print_SPI0_cmd();
    }
}

void print_sensor_cmd(void) {
    UART1_putS("Help:\n\tsensors -ag <address> : get gyro and accelerometer data\n\tsensors -m <address> : get magnetometer data\n");
}

*/
