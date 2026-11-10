/**
  ******************************************************************************
  * @file    stm32f1xx_hal_cortex.h
  * @brief   Header file of CORTEX HAL module.
  ******************************************************************************
  */

#ifndef __STM32F1xx_HAL_CORTEX_H
#define __STM32F1xx_HAL_CORTEX_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f1xx_hal_def.h"

void HAL_NVIC_SetPriorityGrouping(uint32_t PriorityGroup);
void HAL_NVIC_SetPriority(IRQn_Type IRQn, uint32_t PreemptPriority, uint32_t SubPriority);
void HAL_NVIC_EnableIRQ(IRQn_Type IRQn);
void HAL_NVIC_DisableIRQ(IRQn_Type IRQn);
void HAL_NVIC_SystemReset(void);
uint32_t HAL_SYSTICK_Config(uint32_t TicksNumb);

#ifdef __cplusplus
}
#endif

#endif /* __STM32F1xx_HAL_CORTEX_H */
