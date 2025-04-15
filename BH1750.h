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

#define BH1750_TIMEOUT								180		//ms
#define BH1750_LX_CONSTANT							1.2
#define BH1750_POWER_DOWN							0x00
#define BH1750_POWER_ON								0x01
#define BH1750_RESET								0x07

#define BH1750_DEFAULT_SENSITIVITY					69
#define BH1750_MT_REG_LOW_BYTE						0x60
#define BH1750_MT_REG_HIGH_BYTE						0x40

/* The slave address of the I2C changes depends on the
 * status of ADDR Terminal Voltage. Check p. 10
 */
typedef enum {
	ADDR_HIGH = 0xB8,
	ADDR_LOW  = 0x46
}bh1750_addr_status_t;

/* 	Name		Measurement Time		Resolution
*	H Mode		120 ms					1 lx		Generally recommended mode and suitable to detect for darkness
*	H Mode 2	120 ms					0.5 lx		Also suitable to detect for darkness
*	L Mode		16 ms					4 lx
*/
typedef enum {
	CONT_H_RES_MODE   = 0x10,
	CONT_H_RES_MODE_2 = 0x11,
	CONT_L_RES_MODE   = 0x13,
	ONE_TIME_H_MODE	  = 0x20,
	ONE_TIME_H_MODE_2 = 0x21,
	ONE_TIME_L_MODE   = 0x23
}bh1750_sensor_mode_t;

typedef struct {
	I2C_HandleTypeDef *hi2c;
	uint8_t i2c_addr;
	bh1750_sensor_mode_t mode;
	float lumen;
}bh1750_t;

uint8_t bh1750_init(bh1750_t *bh1750);
uint8_t set_slave_addr(bh1750_t *bh1750, bh1750_addr_status_t status);

uint8_t send_command(bh1750_t *bh1750, uint8_t *regAddr, uint8_t size);

uint8_t bh1750_get_lumen(bh1750_t *bh1750);
uint8_t bh1750_change_measurement_time(bh1750_t *bh1750, uint8_t sensitivity);

#endif /* INC_BH1750_H_ */
