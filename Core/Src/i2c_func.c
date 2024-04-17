/*
 * i2c_func.c
 *
 *  Created on: Feb 24, 2024
 *      Author: gvigelet
 */

#include "i2c_func.h"
#include <string.h>
#include <stdio.h>

extern I2C_HandleTypeDef hi2c1;

void I2C_scan() {
    printf("Scanning I2C bus for devices...\r\n");

    for (uint8_t address = 0x0; address < 0x7f; address++) {
        HAL_StatusTypeDef status;
        status = HAL_I2C_IsDeviceReady(&hi2c1, address << 1, 2, 200); // Address shift left by 1 for read/write bit
        if (status == HAL_OK) {
        	printf("%2x ", address);
        }else{
        	printf("-- ");
        }
        if (address > 0 && (address + 1) % 16 == 0) printf("\r\n");
    }

    printf("\r\n");
    fflush(stdout);
}

