/**
  ******************************************************************************
  * @file           : i2c_lcd.h
  * @brief          : 16x2 LCD Driver with I2C Backpack (PCF8574) for STM32 HAL
  ******************************************************************************
  * Wiring:
  * - SCL -> PB6
  * - SDA -> PB7
  */

#ifndef __I2C_LCD_H
#define __I2C_LCD_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

/* Function Prototypes */
uint8_t LCD_Init(void);
void LCD_Clear(void);
void LCD_SetCursor(uint8_t row, uint8_t col);
void LCD_Print(const char *str);
void LCD_PrintLine(uint8_t row, const char *str);
void LCD_UpdateDisplay(float temp, uint8_t temp_valid, uint8_t motion_detected, uint8_t fan_active, uint8_t fan_state);

#ifdef __cplusplus
}
#endif

#endif /* __I2C_LCD_H */
