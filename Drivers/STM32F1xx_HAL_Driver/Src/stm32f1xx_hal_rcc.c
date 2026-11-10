/**
  ******************************************************************************
  * @file    stm32f1xx_hal_rcc.c
  * @brief   RCC HAL module driver.
  ******************************************************************************
  */

#include "stm32f1xx_hal.h"

HAL_StatusTypeDef HAL_RCC_OscConfig(RCC_OscInitTypeDef  *RCC_OscInitStruct)
{
  if (RCC_OscInitStruct->OscillatorType & RCC_OSCILLATORTYPE_HSI)
  {
    if (RCC_OscInitStruct->HSIState != RCC_HSI_OFF)
    {
      RCC->CR |= RCC_CR_HSION;
      while ((RCC->CR & RCC_CR_HSIRDY) == 0U)
      {
      }
    }
  }

  return HAL_OK;
}

HAL_StatusTypeDef HAL_RCC_ClockConfig(RCC_ClkInitTypeDef  *RCC_ClkInitStruct, uint32_t FLatency)
{
  UNUSED(FLatency);

  if (RCC_ClkInitStruct->ClockType & RCC_CLOCKTYPE_SYSCLK)
  {
    if (RCC_ClkInitStruct->SYSCLKSource == RCC_SYSCLKSOURCE_HSI)
    {
      RCC->CFGR &= ~RCC_CFGR_SW_Msk;
      while ((RCC->CFGR & RCC_CFGR_SWS_Msk) != 0U)
      {
      }
    }
  }

  SystemCoreClockUpdate();
  HAL_InitTick(TICK_INT_PRIORITY);

  return HAL_OK;
}
