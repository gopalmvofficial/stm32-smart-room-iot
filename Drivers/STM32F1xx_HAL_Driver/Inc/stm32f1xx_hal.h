/**
  ******************************************************************************
  * @file    stm32f1xx_hal.h
  * @brief   This file contains all the functions prototypes for the HAL module driver.
  ******************************************************************************
  */

#ifndef __STM32F1xx_HAL_H
#define __STM32F1xx_HAL_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f1xx_hal_conf.h"

HAL_StatusTypeDef HAL_Init(void);
HAL_StatusTypeDef HAL_DeInit(void);
void HAL_MspInit(void);
void HAL_MspDeInit(void);
HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority);

void HAL_IncTick(void);
void HAL_Delay(uint32_t Delay);
uint32_t HAL_GetTick(void);
void HAL_SuspendTick(void);
void HAL_ResumeTick(void);
uint32_t HAL_GetHalVersion(void);

#define __HAL_AFIO_REMAP_SWJ_NOJTAG()  do { \
                                           AFIO->MAPR &= ~AFIO_MAPR_SWJ_CFG_Msk; \
                                           AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_NOJTAG; \
                                         } while(0U)

#ifdef __cplusplus
}
#endif

#endif /* __STM32F1xx_HAL_H */
