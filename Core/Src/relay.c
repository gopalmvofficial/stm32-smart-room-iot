/**
  ******************************************************************************
  * @file           : relay.c
  * @brief          : 5V Relay Driver for STM32 HAL (Channel 1 on PB12)
  * @author         : Gopal M V
  ******************************************************************************
  * Control Logic:
  * - Active-LOW input circuitry:
  *     RESET (0V) -> Relay energized (ON)
  *     SET (3.3V) -> Relay de-energized (OFF)
  ******************************************************************************
  */

#include "relay.h"

static uint8_t relay_state = 0; /* 0 = OFF, 1 = ON */
static uint32_t last_toggle_time = 0;

void Relay_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Enable GPIOB Clock */
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /* Configure PB12 Output Level (Active-LOW: Set HIGH initially -> Relay OFF) */
  HAL_GPIO_WritePin(RELAY1_PORT, RELAY1_PIN, GPIO_PIN_SET);
  relay_state = 0;

  /* Configure PB12 as Push-Pull Output */
  GPIO_InitStruct.Pin = RELAY1_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(RELAY1_PORT, &GPIO_InitStruct);
}

void Relay_SetState(uint8_t state)
{
  relay_state = state ? 1 : 0;
  if (relay_state)
  {
    /* Active-LOW: Drive PB12 LOW to turn Relay ON */
    HAL_GPIO_WritePin(RELAY1_PORT, RELAY1_PIN, GPIO_PIN_RESET);
  }
  else
  {
    /* Active-LOW: Drive PB12 HIGH to turn Relay OFF */
    HAL_GPIO_WritePin(RELAY1_PORT, RELAY1_PIN, GPIO_PIN_SET);
  }
}

void Relay_Toggle(void)
{
  Relay_SetState(!relay_state);
}

uint8_t Relay_GetState(void)
{
  return relay_state;
}

/**
  * @brief  Periodic test task (toggles relay state every 2000 ms non-blockingly)
  */
void Relay_TestTask(void)
{
  uint32_t current_time = HAL_GetTick();
  if (current_time - last_toggle_time >= 2000U)
  {
    last_toggle_time = current_time;
    Relay_Toggle();
  }
}
