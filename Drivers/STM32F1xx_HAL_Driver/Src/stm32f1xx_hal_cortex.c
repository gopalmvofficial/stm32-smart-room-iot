/**
  ******************************************************************************
  * @file    stm32f1xx_hal_cortex.c
  * @brief   CORTEX HAL module driver.
  ******************************************************************************
  */

#include "stm32f1xx_hal.h"

void HAL_NVIC_SetPriorityGrouping(uint32_t PriorityGroup)
{
  NVIC_SetPriorityGrouping(PriorityGroup);
}

void HAL_NVIC_SetPriority(IRQn_Type IRQn, uint32_t PreemptPriority, uint32_t SubPriority)
{
  UNUSED(SubPriority);
  NVIC_SetPriority(IRQn, PreemptPriority);
}

void HAL_NVIC_EnableIRQ(IRQn_Type IRQn)
{
  NVIC_EnableIRQ(IRQn);
}

void HAL_NVIC_DisableIRQ(IRQn_Type IRQn)
{
  UNUSED(IRQn);
}

void HAL_NVIC_SystemReset(void)
{
}

uint32_t HAL_SYSTICK_Config(uint32_t TicksNumb)
{
  return SysTick_Config(TicksNumb);
}
