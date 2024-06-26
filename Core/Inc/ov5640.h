/*
 * ov5640.h
 *
 *  Created on: Feb 23, 2024
 *      Author: gvigelet
 */

#ifndef INC_OV5640_H_
#define INC_OV5640_H_

#include "main.h"
#include <stdio.h>
#include <stdint.h>

#define  OV5640_PWDN_Pin_RESET   HAL_GPIO_WritePin(OV5640_PWDN_GPIO_Port, OV5640_PWDN_Pin, GPIO_PIN_RESET)


#define OV5640_ID               0X5640
//#define OV5640_ADDR        		0X78
#define OV5640_ADDR        		0X6C
#define OV5640_CHIPIDH          0X300A
#define OV5640_CHIPIDL          0X300B

#define   USER_KEY_PRESS        HAL_GPIO_ReadPin(USER_KEY_GPIO_Port, USER_KEY_Pin) == 0
#define   WAKE_UP_KEY_PRESS     HAL_GPIO_ReadPin(WAKE_UP_KEY_GPIO_Port, WAKE_UP_KEY_Pin) == 0

#define   XSIZE      160
#define   YSIZE      120
#define   LCD_GRAM_ADDRESS   0x60020000     // FSMC Bank1 NOR/PSRM 1   (LCD_RS=1)


#define   QQVGA_160_120    0
#define   QCIF_176_144     1
#define   QVGA_320_240     2
#define   WQVGA_400_240    3
#define   CIF_352_288      4
#define   VGA_640_480      5

#define   jpeg_buf_size     31*1024




uint8_t OV5640_WR_Reg(uint16_t reg,uint8_t data);
uint8_t OV5640_RD_Reg(uint16_t reg);
uint8_t OV5640_Init(void);
uint16_t OV5640_Read_ID(void);
void OV5640_JPEG_Mode(void);
void OV5640_RGB565_Mode(void);
void OV5640_Exposure(uint8_t exposure);
void OV5640_Light_Mode(uint8_t mode);
void OV5640_Color_Saturation(uint8_t sat);
void OV5640_Brightness(uint8_t bright);
void OV5640_Contrast(uint8_t contrast);
void OV5640_Sharpness(uint8_t sharp);
void OV5640_Special_Effects(uint8_t eft);
void OV5640_Test_Pattern(uint8_t mode);
void OV5640_Flash_Lamp(uint8_t sw);
uint8_t OV5640_OutSize_Set(uint16_t offx,uint16_t offy,uint16_t width,uint16_t height);
uint8_t OV5640_Focus_Init(void);
uint8_t OV5640_Auto_Focus(void);
void rgb565_test(void);
void jpeg_test(uint8_t jpg_size);

#endif /* INC_OV5640_H_ */
