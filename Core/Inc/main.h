/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   Contains hardware pin definitions and shared prototypes.
  ******************************************************************************
  */
/* USER CODE END Header */

#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* Hardware Pin Definitions --------------------------------------------------*/
#define LED_PIN          GPIO_PIN_13
#define LED_GPIO_PORT    GPIOC

#define PIR_PIN          GPIO_PIN_0
#define PIR_PORT         GPIOA

#define DS18B20_PIN      GPIO_PIN_1
#define DS18B20_PORT     GPIOA

#define LCD_SCL_PIN      GPIO_PIN_6
#define LCD_SDA_PIN      GPIO_PIN_7
#define LCD_PORT         GPIOB

#define RELAY1_PIN       GPIO_PIN_12
#define RELAY1_PORT      GPIOB

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
