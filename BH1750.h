/*
 * BH1750.h
 *
 *  		Created on: Mar 31, 2025
 *      		Author: Furkan
 * 		Used Datasheet: https://www.mouser.com/datasheet/2/348/bh1750fvi-e-186247.pdf
 */

#ifndef INC_BH1750_H_
#define INC_BH1750_H_

#include "main.h"

/* Device Infos */

#define BH1750_DEV_ID								0x23
#define BH1750_READ_MODE							0x47
#define BH1750_WRITE_MODE							0x46
#define BH1750_TIMEOUT								180		//ms

/* MEASREMENT ACCURACY */
#define BH1750_LX_CONSTANT							1.2

/* MODES (p. 5) */

/* Explanation of Asynchronous reset and Reset command
 *
 * Asynchronous reset
 *
 * All registers are reset. It is necessary on power supply sequence.
 * Please refer "Timing chart for VCC and DVI power supply sequence"
 * in page 6. It is power down mode during DVI = 'L'.
 *
 * Reset command
 *
 * Reset command is for only reset Illuminance data register. ( reset value is '0' )
 * Reset It is not necessary even power supply sequence.It is used for removing previous
 * measurement result. This command is not working in power down mode, so that please set
 * the power on mode before input this command.
 */

/* TODO: Enum might be good. */

#define BH1750_POWER_DOWN							0x00
#define BH1750_POWER_ON								0x01
#define BH1750_RESET								0x07

#define BH1750_DEFAULT_SENSITIVITY					69
#define BH1750_MT_REG_LOW_BYTE						0x60
#define BH1750_MT_REG_HIGH_BYTE						0x40

/* CONTINUOUS MODE */

/* 	Name		Measurement Time		Resolution
*	H Mode		120 ms					1 lx		Generally recommended mode and suitable to detect for darkness
*	H Mode 2	120 ms					0.5 lx		Also suitable to detect for darkness
*	L Mode		16 ms					4 lx
*/

#define BH1750_CONT_H_RES_MODE						0x10
#define BH1750_CONT_H_RES_MODE_2					0x11
#define BH1750_CONT_L_RES_MODE						0x13

/* ONE TIME MODE */
#define BH1750_ONE_TIME_H_MODE						0x20
#define BH1750_ONE_TIME_H_MODE_2					0x21
#define BH1750_ONE_TIME_L_MODE						0x23

typedef struct {
	I2C_HandleTypeDef *hi2c;
	uint8_t mode;
	uint8_t data[2];
	float lumen;
}bh1750_t;

HAL_StatusTypeDef BH1750_Init(bh1750_t *bh1750, I2C_HandleTypeDef *hi2c);

static HAL_StatusTypeDef writeReg(bh1750_t *bh1750, uint8_t regAddr);
static HAL_StatusTypeDef writeRegs(bh1750_t *bh1750, uint8_t *regAddr, uint8_t size);
HAL_StatusTypeDef readReg(bh1750_t *bh1750, uint8_t regAddr);

HAL_StatusTypeDef BH1750_Change_Mode(bh1750_t *bh1750);
void BH1750_Get_Measurement(bh1750_t *bh1750);

HAL_StatusTypeDef BH1750_Change_Measurement_Time(bh1750_t *bh1750, float sensivity);

#endif /* INC_BH1750_H_ */
