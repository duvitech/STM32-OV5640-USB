/*
 * sccb.h
 *
 *  Created on: Feb 23, 2024
 *      Author: gvigelet
 */

#ifndef INC_SCCB_H_
#define INC_SCCB_H_

#include "main.h"

#define    OV5640_SIOC_H()       HAL_GPIO_WritePin(SIOC_GPIO_Port, SIOC_Pin, GPIO_PIN_SET)
#define    OV5640_SIOC_L()       HAL_GPIO_WritePin(SIOC_GPIO_Port, SIOC_Pin, GPIO_PIN_RESET)
#define    OV5640_SIOD_H()       HAL_GPIO_WritePin(SIOD_GPIO_Port, SIOD_Pin, GPIO_PIN_SET)
#define    OV5640_SIOD_L()       HAL_GPIO_WritePin(SIOD_GPIO_Port, SIOD_Pin, GPIO_PIN_RESET)

#define    OV5640_SIOD_READ()     HAL_GPIO_ReadPin(SIOD_GPIO_Port, SIOD_Pin)


void SCCB_Init(void);
void SCCB_Start(void);
void SCCB_Stop(void);
void SCCB_No_Ack(void);
uint8_t SCCB_WR_Byte(uint8_t dat);
uint8_t SCCB_RD_Byte(void);

#endif /* INC_SCCB_H_ */
