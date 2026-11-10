/**
  ******************************************************************************
  * @file           : ds18b20.c
  * @brief          : DS18B20 1-Wire Temperature Sensor Driver for STM32 HAL
  * @author         : Gopal M V
  ******************************************************************************
  * 1-Wire Open-Drain timing driver with DWT cycle counter microsecond delays.
  */

#include "ds18b20.h"

/* Initialize DWT (Data Watchpoint and Trace) Cycle Counter for microsecond delays */
static void DWT_Init(void)
{
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

static void delay_us(uint32_t us)
{
  uint32_t startTicks = DWT->CYCCNT;
  uint32_t targetTicks = us * (SystemCoreClock / 1000000U);
  while ((DWT->CYCCNT - startTicks) < targetTicks);
}

static void DS18B20_SetPinOD(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = DS18B20_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(DS18B20_PORT, &GPIO_InitStruct);
}

uint8_t DS18B20_Init(void)
{
  DWT_Init();
  DS18B20_SetPinOD();
  HAL_GPIO_WritePin(DS18B20_PORT, DS18B20_PIN, GPIO_PIN_SET);
  delay_us(1000);
  return DS18B20_Reset();
}

uint8_t DS18B20_Reset(void)
{
  uint8_t response = 0;

  /* Pull low for 480us reset pulse */
  HAL_GPIO_WritePin(DS18B20_PORT, DS18B20_PIN, GPIO_PIN_RESET);
  delay_us(480);

  /* Release line high */
  HAL_GPIO_WritePin(DS18B20_PORT, DS18B20_PIN, GPIO_PIN_SET);
  delay_us(15); /* Wait 15us for DS18B20 to pull low */

  /* Sample presence pulse over a 100us window */
  for (int i = 0; i < 100; i++)
  {
    if (HAL_GPIO_ReadPin(DS18B20_PORT, DS18B20_PIN) == GPIO_PIN_RESET)
    {
      response = 1; /* Presence pulse detected! */
      break;
    }
    delay_us(1);
  }

  delay_us(380); /* Complete presence phase */
  return response;
}

void DS18B20_WriteByte(uint8_t data)
{
  for (int i = 0; i < 8; i++)
  {
    if (data & (1 << i))
    {
      /* Write 1: pull low for 2us, then release high for 60us */
      HAL_GPIO_WritePin(DS18B20_PORT, DS18B20_PIN, GPIO_PIN_RESET);
      delay_us(2);
      HAL_GPIO_WritePin(DS18B20_PORT, DS18B20_PIN, GPIO_PIN_SET);
      delay_us(60);
    }
    else
    {
      /* Write 0: pull low for 60us, then release high for 10us */
      HAL_GPIO_WritePin(DS18B20_PORT, DS18B20_PIN, GPIO_PIN_RESET);
      delay_us(60);
      HAL_GPIO_WritePin(DS18B20_PORT, DS18B20_PIN, GPIO_PIN_SET);
      delay_us(10);
    }
  }
}

uint8_t DS18B20_ReadByte(void)
{
  uint8_t value = 0;

  for (int i = 0; i < 8; i++)
  {
    /* Pull low for 2us to initiate read slot */
    HAL_GPIO_WritePin(DS18B20_PORT, DS18B20_PIN, GPIO_PIN_RESET);
    delay_us(2);

    /* Release line high */
    HAL_GPIO_WritePin(DS18B20_PORT, DS18B20_PIN, GPIO_PIN_SET);
    delay_us(10); /* Sample at t=12us */

    if (HAL_GPIO_ReadPin(DS18B20_PORT, DS18B20_PIN) == GPIO_PIN_SET)
    {
      value |= (1 << i);
    }

    delay_us(50); /* Complete 60us slot */
  }

  return value;
}

uint8_t DS18B20_StartConversion(void)
{
  if (!DS18B20_Reset())
  {
    return 0; /* Sensor missing */
  }

  DS18B20_WriteByte(0xCC); /* Skip ROM */
  DS18B20_WriteByte(0x44); /* Convert T command */
  return 1;
}

uint8_t DS18B20_ReadScratchpad(float *temperature)
{
  if (!DS18B20_Reset())
  {
    return 0; /* Sensor missing */
  }

  DS18B20_WriteByte(0xCC); /* Skip ROM */
  DS18B20_WriteByte(0xBE); /* Read Scratchpad */

  uint8_t temp_lsb = DS18B20_ReadByte();
  uint8_t temp_msb = DS18B20_ReadByte();

  int16_t raw_temp = (int16_t)((temp_msb << 8) | temp_lsb);

  /* 0xFFFF means disconnected / open bus */
  if (raw_temp == 0xFFFF)
  {
    return 0;
  }

  *temperature = (float)raw_temp / 16.0f;

  /* Filter out invalid range (< -55C or > 125C) */
  if (*temperature < -55.0f || *temperature > 125.0f)
  {
    return 0;
  }

  return 1;
}

uint8_t DS18B20_ReadTemperature(float *temperature)
{
  if (!DS18B20_StartConversion())
  {
    return 0;
  }

  HAL_Delay(750);
  return DS18B20_ReadScratchpad(temperature);
}
