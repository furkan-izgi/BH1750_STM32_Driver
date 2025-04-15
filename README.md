# BH1750 Light Sensor Driver (STM32 HAL - I2C)

This project provides a lightweight STM32 HAL-based driver for the BH1750 digital light intensity sensor. It allows you to easily configure measurement modes, adjust sensitivity, and read lux values via the I2C interface.

> NOTE: This has not been tested with any simulation or on a real circuit :/

## 📦 File Structure

| File        | Description                                           |
|-------------|-------------------------------------------------------|
| BH1750.h    | Header file with structures, mode definitions, and function declarations. |
| BH1750.c    | Source file containing implementation using STM32 HAL I2C. |

## ⚙️ Hardware Requirements

- STM32 Microcontroller (e.g. STM32F1, STM32F4)
- BH1750 Light Sensor (I2C-based)
- I2C lines (SCL, SDA)
- Pull-up resistors (typically 4.7kΩ)

## 🔢 Available Measurement Modes

| Definition                   | Description                          |
|-----------------------------|--------------------------------------|
| BH1750_CONT_H_RES_MODE      | Continuous high-res (1 lx, 120 ms)   |
| BH1750_CONT_H_RES_MODE_2    | Continuous high-res 2 (0.5 lx)       |
| BH1750_CONT_L_RES_MODE      | Continuous low-res (4 lx, 16 ms)     |
| BH1750_ONE_TIME_H_MODE      | One-time high-res                    |
| BH1750_ONE_TIME_H_MODE_2    | One-time high-res 2                  |
| BH1750_ONE_TIME_L_MODE      | One-time low-res                     |

⚠️ For one-time modes, the mode must be resent before each measurement.

## 🔎 References

- BH1750 Datasheet: [BH1750FVI Datasheet](https://www.mouser.com/datasheet/2/348/bh1750fvi-e-186247.pdf)
- STM32Cube HAL Documentation: [HAL Doc](https://www.st.com/en/embedded-software/stm32cubemx.html)

## 📄 License

This driver is provided under the MIT License for educational and development purposes.

Feel free to contribute, fork, or ask questions! 😊
