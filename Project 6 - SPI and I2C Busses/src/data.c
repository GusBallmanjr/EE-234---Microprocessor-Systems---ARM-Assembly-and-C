#include "data.h"

void int_hexString(uint32_t integer, char* outStr) {
	uint32_t num = integer;
	uint32_t remainder = 0;

	outStr[0] = '0';
	outStr[1] = 'x';

	for (int i = 0; i < 8; i++) {
		remainder = num & 0xF;

	    if (remainder < 10) {
	    	outStr[9 - i] = '0' + remainder;
	    }
	    else {
	    	outStr[9 - i] = 'A' + (remainder - 10);
	    }
	    num >>= 4;
	}
	outStr[10] = '\0';
}

uint32_t hexString_int(char* hex) {
    uint32_t value = 0;
    hex += 2;
    for (uint32_t i = 0; hex[i] != '\0'; i++) {
        if (hex[i] >= '0' && hex[i] <= '9') hex[i] = hex[i] - '0';
        else if (hex[i] >= 'a' && hex[i] <= 'f') hex[i] = hex[i] - 'a' + 10;
        else if (hex[i] >= 'A' && hex[i] <= 'F') hex[i] = hex[i] - 'A' + 10;
        value = (value << 4) | (hex[i] & 0xF);
    }
    return value;
}
