/**
  ******************************************************************************
  * @file           : ds18b20.h
  * @brief          : DS18B20 1-Wire Temperature Sensor Driver for STM32 HAL
  ******************************************************************************
  */

#ifndef __DS18B20_H
#define __DS18B20_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

/* DS18B20 Pin Configuration (PA1) */
#ifndef DS18B20_PORT
#define DS18B20_PORT GPIOA
#endif

#ifndef DS18B20_PIN
#define DS18B20_PIN GPIO_PIN_1
#endif

/* Function Prototypes */
uint8_t DS18B20_Init(void);
uint8_t DS18B20_Reset(void);
void DS18B20_WriteByte(uint8_t data);
uint8_t DS18B20_ReadByte(void);
uint8_t DS18B20_StartConversion(void);
uint8_t DS18B20_ReadScratchpad(float *temperature);
uint8_t DS18B20_ReadTemperature(float *temperature);

#ifdef __cplusplus
}
#endif

#endif /* __DS18B20_H */
