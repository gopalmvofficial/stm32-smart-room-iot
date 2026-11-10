/**
  ******************************************************************************
  * @file    system_stm32f1xx.h
  * @brief   CMSIS Cortex-M3 Device System Header File.
  ******************************************************************************
  */

#ifndef __SYSTEM_STM32F1XX_H
#define __SYSTEM_STM32F1XX_H

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>

extern uint32_t SystemCoreClock;          /*!< System Clock Frequency (Core Clock) */
extern const uint8_t AHBPrescTable[16];    /*!< AHB prescalers table values */
extern const uint8_t APBPrescTable[8];     /*!< APB prescalers table values */

extern void SystemInit(void);
extern void SystemCoreClockUpdate(void);

#ifdef __cplusplus
}
#endif

#endif /* __SYSTEM_STM32F1XX_H */
