/**
  ******************************************************************************
  * @file    stm32f1xx_hal_gpio.c
  * @brief   GPIO HAL module driver.
  ******************************************************************************
  */

#include "stm32f1xx_hal.h"

void HAL_GPIO_Init(GPIO_TypeDef  *GPIOx, GPIO_InitTypeDef *GPIO_Init)
{
  uint32_t position = 0x00U;
  uint32_t ioposition = 0x00U;
  uint32_t iocurrent = 0x00U;
  uint32_t config = 0x00U;

  for (position = 0U; position < 16U; position++)
  {
    ioposition = (0x01U << position);
    iocurrent = (uint32_t)(GPIO_Init->Pin) & ioposition;

    if (iocurrent == ioposition)
    {
      if (GPIO_Init->Mode == GPIO_MODE_OUTPUT_PP)
      {
        config = 0x00U | (GPIO_Init->Speed & 0x03U); // Push-pull output + speed
      }
      else if (GPIO_Init->Mode == GPIO_MODE_OUTPUT_OD)
      {
        config = 0x04U | (GPIO_Init->Speed & 0x03U); // Open-drain output + speed
      }
      else
      {
        config = 0x04U; // Floating input
      }

      if (position < 8U)
      {
        GPIOx->CRL &= ~(0x0FU << (position * 4U));
        GPIOx->CRL |= (config << (position * 4U));
      }
      else
      {
        GPIOx->CRH &= ~(0x0FU << ((position - 8U) * 4U));
        GPIOx->CRH |= (config << ((position - 8U) * 4U));
      }
    }
  }
}

void HAL_GPIO_DeInit(GPIO_TypeDef  *GPIOx, uint32_t GPIO_Pin)
{
  UNUSED(GPIOx);
  UNUSED(GPIO_Pin);
}

GPIO_PinState HAL_GPIO_ReadPin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
  if ((GPIOx->IDR & GPIO_Pin) != (uint32_t)GPIO_PIN_RESET)
  {
    return GPIO_PIN_SET;
  }
  else
  {
    return GPIO_PIN_RESET;
  }
}

void HAL_GPIO_WritePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState)
{
  if (PinState != GPIO_PIN_RESET)
  {
    GPIOx->BSRR = GPIO_Pin;
  }
  else
  {
    GPIOx->BRR = GPIO_Pin;
  }
}

void HAL_GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
  uint32_t odr = GPIOx->ODR;
  GPIOx->BSRR = ((odr & GPIO_Pin) << 16U) | (~odr & GPIO_Pin);
}

HAL_StatusTypeDef HAL_GPIO_LockPin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
  UNUSED(GPIOx);
  UNUSED(GPIO_Pin);
  return HAL_OK;
}
