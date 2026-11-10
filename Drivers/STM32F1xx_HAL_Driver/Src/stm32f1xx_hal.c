/**
  ******************************************************************************
  * @file    stm32f1xx_hal.c
  * @brief   HAL module driver.
  ******************************************************************************
  */

#include "stm32f1xx_hal.h"

__IO uint32_t uwTick;
uint32_t uwTickPrio   = (1UL << __NVIC_PRIO_BITS) - 1UL;

HAL_StatusTypeDef HAL_Init(void)
{
  /* Configure Flash prefetch */
#if (PREFETCH_ENABLE != 0)
  /* Prefetch enable */
#endif

  /* Set Interrupt Group Priority */
  NVIC_SetPriorityGrouping(0x00000003U);

  /* Use systick as time base source and configure 1ms tick (default clock 8MHz) */
  HAL_InitTick(TICK_INT_PRIORITY);

  /* Init the low level hardware */
  HAL_MspInit();

  return HAL_OK;
}

HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
{
  /* Configure SysTick to generate interrupt every 1 ms */
  if (SysTick_Config(SystemCoreClock / 1000U) > 0U)
  {
    return HAL_ERROR;
  }

  /* Configure SysTick IRQ priority */
  if (TickPriority < (1UL << __NVIC_PRIO_BITS))
  {
    NVIC_SetPriority(SysTick_IRQn, TickPriority);
    uwTickPrio = TickPriority;
  }
  else
  {
    return HAL_ERROR;
  }

  return HAL_OK;
}

__WEAK void HAL_IncTick(void)
{
  uwTick += 1U;
}

uint32_t HAL_GetTick(void)
{
  return uwTick;
}

void HAL_Delay(uint32_t Delay)
{
  uint32_t tickstart = HAL_GetTick();
  uint32_t wait = Delay;

  /* Add a freq priority workload if tick is less than 1ms */
  if (wait < HAL_MAX_DELAY)
  {
    wait += (uint32_t)(1U);
  }

  while ((HAL_GetTick() - tickstart) < wait)
  {
  }
}

void HAL_SuspendTick(void)
{
  SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;
}

void HAL_ResumeTick(void)
{
  SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
}

__WEAK void HAL_MspInit(void)
{
}
