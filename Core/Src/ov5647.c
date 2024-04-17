/*
 * ov5647.c
 *
 *  Created on: Feb 24, 2024
 *      Author: gvigelet
 */
#include "ov5647.h"
#include <stdio.h>

static int ov5647_write(uint16_t reg, uint8_t val)
{
	unsigned char data[3] = { reg >> 8, reg & 0xff, val};

	// Check if the I2C handle is valid
    if (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY) {
    	printf("===> ERROR I2C Not in ready state\r\n");
        return -1; // I2C is not in a ready state
    }

    if(HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)(OV5647_ADDRESS << 1), data, 3, HAL_MAX_DELAY)!= HAL_OK)
	{
        /* Error_Handler() function is called when error occurs. */
        Error_Handler();
	}

    if (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY) {
    	printf("===> ERROR I2C Not in ready state\r\n");
        return -1; // I2C is not in a ready state
    }

	return 0;
}

static int ov5647_read(uint16_t reg, uint8_t *val)
{
	uint8_t buf[2] = { reg >> 8, reg & 0xff };

	// Check if the I2C handle is valid
    if (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY) {
    	printf("===> ERROR I2C Not in ready state\r\n");
        return -1; // I2C is not in a ready state
    }

    if(HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)(OV5647_ADDRESS << 1), buf, 2, HAL_MAX_DELAY)!= HAL_OK)
	{
        /* Error_Handler() function is called when error occurs. */
        Error_Handler();
	}

    if (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY) {
    	printf("===> ERROR I2C Not in ready state\r\n");
        return -1; // I2C is not in a ready state
    }

	// Check if the I2C handle is valid
    if (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY) {
    	printf("===> ERROR I2C Not in ready state\r\n");
        return -1; // I2C is not in a ready state
    }

    if(HAL_I2C_Master_Receive(&hi2c1, (uint16_t)(OV5647_ADDRESS << 1), val, 1, HAL_MAX_DELAY)!= HAL_OK)
	{
        /* Error_Handler() function is called when error occurs. */
        Error_Handler();
	}

    if (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY) {
    	printf("===> ERROR I2C Not in ready state\r\n");
        return -1; // I2C is not in a ready state
    }

	return 0;
}


static int ov5647_write_array(const struct regval_list *regs, int array_size)
{
	int i, ret;

	for (i = 0; i < array_size; i++) {
		ret = ov5647_write(regs[i].addr, regs[i].data);
		if (ret < 0)
			return ret;
	}

	return 0;
}


int ov5647_set_virtual_channel(int channel)
{
	uint8_t channel_id;
	int ret;

	ret = ov5647_read(OV5647_REG_MIPI_CTRL14, &channel_id);
	if (ret < 0)
		return ret;

	channel_id &= ~(3 << 6);

	return ov5647_write(OV5647_REG_MIPI_CTRL14,
			    channel_id | (channel << 6));
}


int ov5647_stream_on()
{
	int ret;

	ret = ov5647_write(OV5647_REG_FRAME_OFF_NUMBER, 0x00);
	if (ret < 0)
		return ret;

	return ov5647_write(OV5640_REG_PAD_OUT, 0x00);
}

int ov5647_stream_off()
{
	int ret;

	ret = ov5647_write(OV5647_REG_FRAME_OFF_NUMBER, 0x0f);
	if (ret < 0)
		return ret;

	return ov5647_write(OV5640_REG_PAD_OUT, 0x01);
}

int ov5647_power_on()
{
	int ret;
	ret = ov5647_write_array(sensor_oe_enable_regs,
				 ARRAY_SIZE(sensor_oe_enable_regs));
	if (ret < 0) {
		printf("write sensor_oe_enable_regs error\r\n");
		return ret;
	}

	ret = ov5647_stream_off();
	if (ret < 0) {
		printf("camera not available, check power\r\n");
		return ret;
	}
	return 0;
}

int ov5647_power_off()
{
	int ret;
	uint8_t rdval;
	/* Enter software standby */
	ret = ov5647_read(OV5647_SW_STANDBY, &rdval);
	if (ret < 0)
		printf("software standby failed\r\n");

	rdval &= ~0x01;
	ret = ov5647_write(OV5647_SW_STANDBY, rdval);
	if (ret < 0)
		printf("software standby failed\r\n");
}

int ov5647_detect()
{
	uint8_t read;
	uint16_t chip_id;
	int ret;

    HAL_StatusTypeDef status;

	status = HAL_I2C_IsDeviceReady(&hi2c1, OV5647_ADDRESS << 1, 2, 200); // Address shift left by 1 for read/write bit
	if (status != HAL_OK) {

		printf("Device Not Ready\r\n");
		return -1;
	}

	ret = ov5647_write(OV5647_SW_RESET, 0x01);
	if (ret < 0)
	{
		printf("Failed to write OV5647_SW_RESET\r\n");
		return ret;
	}

	ret = ov5647_read(OV5647_REG_CHIPID_H, &read);
	if (ret < 0)
	{
		printf("Failed to read OV5647_REG_CHIPID_H got %x\r\n", read);
		return ret;
	}
	chip_id = read << 8;

	if (read != 0x56) {
		printf("ID High expected 0x56 got %x", read);
		return -1;
	}

	ret = ov5647_read(OV5647_REG_CHIPID_L, &read);
	if (ret < 0)
	{
		printf("Failed to read OV5647_REG_CHIPID_L got %x\r\n", read);
		return ret;
	}

	chip_id = chip_id | read;

	if (read != 0x47) {
		printf("ID Low expected 0x47 got %x", read);
		return -1;
	}

	printf("CHIP ID: %04X\r\n", chip_id);
	return ov5647_write(OV5647_SW_RESET, 0x00);
}


int ov5647_set_1080p()
{
	int ret = 0;

	ret = ov5647_write_array(ov5647_1080p30_10bpp, ARRAY_SIZE(ov5647_1080p30_10bpp));
	if(ret<0){
		printf("Failed to write 1080p Registers\r\n");
	}
	return ret;
}
