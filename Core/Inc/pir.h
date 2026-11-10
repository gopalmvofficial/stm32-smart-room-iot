/**
  ******************************************************************************
  * @file           : pir.h
  * @brief          : HC-SR501 PIR Motion Sensor Driver for STM32 HAL
  ******************************************************************************
  */

#ifndef __PIR_H
#define __PIR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

#ifndef PIR_PORT
#define PIR_PORT GPIOA
#endif

#ifndef PIR_PIN
#define PIR_PIN GPIO_PIN_0
#endif

/* Function Prototypes */
void PIR_Init(void);
uint8_t PIR_Read(void);

#ifdef __cplusplus
}
#endif

#endif /* __PIR_H */
