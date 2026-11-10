/**
  ******************************************************************************
  * @file           : relay.h
  * @brief          : 5V Relay Module Driver for STM32 HAL (Channel 1 on PB12)
  ******************************************************************************
  * NOTE: Fan/External Load is NOT connected yet.
  * PB12 is used strictly to test the relay module's input control circuitry.
  */

#ifndef __RELAY_H
#define __RELAY_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

#ifndef RELAY1_PORT
#define RELAY1_PORT GPIOB
#endif

#ifndef RELAY1_PIN
#define RELAY1_PIN GPIO_PIN_12
#endif

/* Function Prototypes */
void Relay_Init(void);
void Relay_SetState(uint8_t state); /* 1 = ON, 0 = OFF */
void Relay_Toggle(void);
uint8_t Relay_GetState(void);
void Relay_TestTask(void); /* Periodic non-blocking test (toggles every 2s) */

#ifdef __cplusplus
}
#endif

#endif /* __RELAY_H */
