#include "BH1750.h"


/*
 * @brief  	Inits the BH1750 sensor.
 * @param  	bh1750 pointer to store i2c handle, sensor mode, payload data
 * 		   	and lumen values
 * @retval 	Status code
 * 			- 0 success
 * 			- 1 i2c device not ready
 */
uint8_t bh1750_init(bh1750_t *bh1750) {
	uint8_t err;

	/* Check I2C Device Ready or Not */
	if(HAL_I2C_IsDeviceReady(bh1750->hi2c, bh1750->i2c_addr, 1, BH1750_TIMEOUT) != 0) {
		return 1;
	}

	/* Set Measurement Time to Default Value */
	err = bh1750_change_measurement_time(bh1750, BH1750_DEFAULT_SENSITIVITY);
	if(err != 0) {
		return err;
	}

	/* Set Power to On */
	err = send_command(bh1750, (uint8_t *)BH1750_POWER_ON, 1);
	if(err != 0) {
		return err;
	}

	/* Reset the sensor */
	err = send_command(bh1750, (uint8_t *)BH1750_RESET, 1);
	if(err != 0) {
		return err;
	}

	return 0;
}

/*
 * @brief  	Sets the i2c slave address of the sensor corresponding to ADDR Terminal Voltage
 * @param  	bh1750 pointer to store i2c handle, sensor mode, payload data
 * 		   	and lumen values
 * @param	regAddr to store register address
 * @param	status of ADDR Terminal
 * @retval 	Status code
 * 			- 0 success
 * 			- 1 handle null error
 */
uint8_t set_slave_addr(bh1750_t *bh1750, bh1750_addr_status_t status) {
	if(bh1750 == NULL) {
		return 1;
	}

	bh1750->i2c_addr = status;

	return 0;
}

/*
 * @brief  	Writes value(s) to the related register, which comes from the user
 * @param  	bh1750 pointer to store i2c handle, sensor mode, payload data
 * 		   	and lumen values
 * @param	regAddr to store register address
 * @param	size of the regAddr
 * @retval 	Status code
 * 			- 0 success
 * 			- 1 i2c transmit error
 */
uint8_t send_command(bh1750_t *bh1750, uint8_t *regAddr, uint8_t size) {
	if(HAL_I2C_Master_Transmit(bh1750->hi2c, bh1750->i2c_addr, regAddr, size, BH1750_TIMEOUT) != 0) {
		return 1;
	}
	return 0;
}


/*
 * @brief  	Gets the value of lumen
 * @param  	bh1750 pointer to store i2c handle, sensor mode, payload data
 * 		   	and lumen values
 * @retval 	Status code
 * 			- 0 success
 * 			- 1 i2c memory read error
 */
uint8_t bh1750_get_lumen(bh1750_t *bh1750) {
	uint16_t light_data;
	uint8_t package[2];

	if(HAL_I2C_Mem_Read(bh1750->hi2c, bh1750->i2c_addr, bh1750->mode, I2C_MEMADD_SIZE_8BIT, package, 2, BH1750_TIMEOUT) != 0) {
		bh1750->lumen = -1;
		return 1;
	}

	light_data = ((uint16_t)package[0] << 8) | (uint16_t)package[1];

	bh1750->lumen = (float)light_data / BH1750_LX_CONSTANT;

	return 0;
}

/*
 * @brief  	Gets the value of lumen. For more information, you can check p. 5 and 11
 * @param  	bh1750 pointer to store i2c handle, sensor mode, payload data
 * 		   	and lumen values
 * @param	sensivity of the measurement
 * @retval 	Status code
 * 			- 0 success
 * 			- 1 i2c transmit error
 * 			- 2 sensivity range error
 */
uint8_t bh1750_change_measurement_time(bh1750_t *bh1750, uint8_t sensitivity) {
	if(!(sensitivity >= 31 && sensitivity <= 254)) { /* Check p. 11 for this */
		return 2;
	}

	uint8_t sens_bits[2] = {BH1750_MT_REG_HIGH_BYTE, BH1750_MT_REG_LOW_BYTE};

	sens_bits[0] |= ((sensitivity & 0xE0) >> 5);
	sens_bits[1] |= (sensitivity & 0x1F);

	if(send_command(bh1750, sens_bits, 2) != 0) {
		return 1;
	}

	return 0;
}
