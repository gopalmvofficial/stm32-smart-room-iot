/**
  ******************************************************************************
  * @file           : pir.c
  * @brief          : HC-SR501 PIR Motion Sensor Driver for STM32 HAL
  * @author         : Gopal M V
  ******************************************************************************
  */

#include "pir.h"

void PIR_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Enable GPIOA Clock */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /* Configure PA0 as Input with Pull-down */
  GPIO_InitStruct.Pin = PIR_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(PIR_PORT, &GPIO_InitStruct);
}

uint8_t PIR_Read(void)
{
  if (HAL_GPIO_ReadPin(PIR_PORT, PIR_PIN) == GPIO_PIN_SET)
  {
    return 1; /* Motion Detected */
  }
  return 0; /* Clear */
}
