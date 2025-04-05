#include "BH1750.h"

HAL_StatusTypeDef BH1750_Init(bh1750_t *bh1750, I2C_HandleTypeDef *hi2c){
	HAL_StatusTypeDef status;

	bh1750->hi2c = hi2c;
	bh1750->mode = BH1750_CONT_H_RES_MODE;
	bh1750->data[0] = 0;
	bh1750->data[1] = 0;

	/* Set Measurement Time to Default Value */
	status = BH1750_Change_Measurement_Time(bh1750, 69);
	if(status != HAL_OK) {
		return status;
	}

	/* Change Mode to Continuously H Resolution Mode */
	status = BH1750_Change_Mode(bh1750);
	if(status != HAL_OK) {
		return status;
	}

	/* Set Power to On */
	status = writeReg(bh1750, BH1750_POWER_ON);
	if(status != HAL_OK) {
		return status;
	}

	/* Reset the sensor */
	status = writeReg(bh1750, BH1750_RESET);
	if(status != HAL_OK) {
		return status;
	}

	return status;
}

static HAL_StatusTypeDef writeReg(bh1750_t *bh1750, uint8_t regAddr) {
	HAL_StatusTypeDef status;
	status = HAL_I2C_Master_Transmit(bh1750->hi2c, BH1750_WRITE_MODE, &regAddr, 1, BH1750_TIMEOUT);
	return status;
}

static HAL_StatusTypeDef writeRegs(bh1750_t *bh1750, uint8_t *regAddr, uint8_t size) {
	HAL_StatusTypeDef status;
	status = HAL_I2C_Master_Transmit(bh1750->hi2c, BH1750_WRITE_MODE, regAddr, size, BH1750_TIMEOUT);
	return status;
}

HAL_StatusTypeDef readReg(bh1750_t *bh1750, uint8_t regAddr) {
	HAL_StatusTypeDef status;

	status = HAL_I2C_Master_Transmit(bh1750->hi2c, BH1750_WRITE_MODE, &regAddr, 1, BH1750_TIMEOUT);
	if(status == HAL_OK) {
		status = HAL_I2C_Master_Receive(bh1750->hi2c, BH1750_READ_MODE, bh1750->data, 2, BH1750_TIMEOUT);
		return status;
	}
	return status;
}

HAL_StatusTypeDef BH1750_Change_Mode(bh1750_t *bh1750) {
	HAL_StatusTypeDef status;
	status = writeReg(bh1750, bh1750->mode);
	return status;
}

void BH1750_Get_Measurement(bh1750_t *bh1750) {
	HAL_StatusTypeDef status;
	uint16_t light_data;
	status = readReg(bh1750, bh1750->mode);
	if(status != HAL_OK) {
		bh1750->lumen = -1;
	}

	light_data = (bh1750->data[0] << 8) | bh1750->data[1];

	bh1750->lumen = (float)light_data / BH1750_LX_CONSTANT;
}

/* For more information, you can check p. 5 and 11 */
HAL_StatusTypeDef BH1750_Change_Measurement_Time(bh1750_t *bh1750, float sensitivity){
	HAL_StatusTypeDef status;

	if(!(sensitivity >= 31 && sensitivity <= 254)) { /* Check p. 11 for this */
		return HAL_ERROR;
	}

	uint8_t new_sens = BH1750_DEFAULT_SENSITIVITY * sensitivity;

	uint8_t sens_bits[2] = {BH1750_MT_REG_HIGH_BYTE, BH1750_MT_REG_LOW_BYTE};

	sens_bits[0] |= ((new_sens & 0xE0) >> 5);
	sens_bits[1] |= (new_sens & 0x1F);

	status = writeRegs(bh1750, sens_bits, 2);

	return status;
}
