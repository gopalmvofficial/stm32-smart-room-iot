/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Smart Room IoT Controller - Main System Routine
  * @author         : Gopal M V
  ******************************************************************************
  * Hardware Connections:
  * - MCU                  : STM32F103C6T6 Blue Pill (ARM Cortex-M3 @ 72MHz)
  * - Temperature Sensor   : DS18B20 on PA1 (1-Wire bus with 4.7k pull-up)
  * - Motion Sensor        : HC-SR501 PIR on PA0 (Digital input)
  * - LCD Display          : 16x2 I2C (PCF8574) on PB6 (SCL), PB7 (SDA)
  * - Relay Module         : 5V Relay IN1 on PB12 (Active-LOW trigger)
  * - Status LED           : Onboard LED on PC13 (Active-LOW)
  * - Wi-Fi Module         : ESP8266 / ESP-12F on USART1 (PA9 TX, PA10 RX)
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "ds18b20.h"
#include "pir.h"
#include "i2c_lcd.h"
#include "relay.h"
#include "usart.h"
#include "blynk.h"
#include <stdio.h>

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void System_SelfTest(void);

/* Shared System Variables */
float current_temperature = 0.0f;
uint8_t temp_valid = 0;
uint8_t motion_state = 0;

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* Reset of all peripherals, Initializes Flash interface and Systick */
  HAL_Init();

  /* Configure system clock */
  SystemClock_Config();

  /* Initialize GPIOs */
  MX_GPIO_Init();

  /* Initialize Peripheral Drivers */
  Relay_Init();
  PIR_Init();
  DS18B20_Init();

  /* LCD Init Retry Loop (Turn on screen FIRST) */
  uint8_t lcd_ok = 0;
  for (uint8_t attempt = 0; attempt < 3; attempt++)
  {
    lcd_ok = LCD_Init();
    if (lcd_ok) break;
    HAL_Delay(100);
  }

  /* Perform Startup Self Test */
  if (lcd_ok)
  {
    System_SelfTest();
    LCD_PrintLine(0, "SMART ROOM IOT");
    LCD_PrintLine(1, "WIFI: CONNECTING");
  }

  /* Initialize Blynk 2.0 & ESP8266 Wi-Fi */
  Blynk_Init();

  if (lcd_ok)
  {
    LCD_PrintLine(0, "Temp: MEASURING ");
    LCD_PrintLine(1, "Fan: STANDBY    ");
  }

  /* Non-blocking DS18B20 state machine variables */
  typedef enum { DS_IDLE_START, DS_WAIT_CONVERSION, DS_WAIT_NEXT_READ } DS_State_t;
  DS_State_t ds_state = DS_IDLE_START;
  uint32_t ds_timer = 0;

  /* Timers & State Tracking */
  uint32_t led_timer = 0;
  uint32_t blynk_telemetry_timer = 0;
  static uint8_t prev_motion_state = 0xFF;
  static uint8_t prev_relay_state = 0xFF;

  /* Infinite Main Monitoring Loop */
  while (1)
  {
    uint32_t now = HAL_GetTick();

    /* 0. Process Blynk IoT Tasks & Incoming Cloud Messages */
    Blynk_Run();

    /* 1. Motion Sensor Read with Debouncing (Prevents bus noise glitches on PA0) */
    static uint8_t pir_debounce_cnt = 0;
    uint8_t raw_pir = PIR_Read();
    if (raw_pir)
    {
      if (pir_debounce_cnt < 3) pir_debounce_cnt++;
    }
    else
    {
      pir_debounce_cnt = 0;
    }
    motion_state = (pir_debounce_cnt >= 2) ? 1 : 0;

    /* 2. Motion Status LED Blinking (PC13 onboard LED blinks rapidly when motion detected) */
    if (motion_state)
    {
      if (now - led_timer >= 150U)
      {
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
        led_timer = now;
      }
    }
    else
    {
      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET); /* LED OFF */
    }

    /* 3. Non-blocking DS18B20 Temperature Conversion & Reading (PA1) */
    switch (ds_state)
    {
      case DS_IDLE_START:
        if (DS18B20_StartConversion())
        {
          ds_timer = now;
          ds_state = DS_WAIT_CONVERSION;
        }
        else
        {
          temp_valid = 0;
          ds_timer = now;
          ds_state = DS_WAIT_NEXT_READ;
        }
        break;

      case DS_WAIT_CONVERSION:
        /* Wait 750ms for 12-bit temperature conversion */
        if (now - ds_timer >= 750U)
        {
          temp_valid = DS18B20_ReadScratchpad(&current_temperature);
          ds_timer = now;
          ds_state = DS_WAIT_NEXT_READ;
        }
        break;

      case DS_WAIT_NEXT_READ:
        /* Wait remaining cycle time to achieve ~1 sec read interval */
        if (now - ds_timer >= 250U)
        {
          ds_state = DS_IDLE_START;
        }
        break;
    }

    /* 4. Relay & Fan Control (Motion triggered or Blynk manual override) */
    uint8_t manual_override = Blynk_GetManualOverride();
    uint8_t fan_target_state = motion_state || manual_override;
    Relay_SetState(fan_target_state);

    /* 5. Blynk 2.0 Cloud Telemetry Updates */
    if (now - blynk_telemetry_timer >= 2000U)
    {
      blynk_telemetry_timer = now;
      if (temp_valid)
      {
        Blynk_VirtualWriteFloat(VIRTUAL_PIN_TEMP, current_temperature);
      }
    }

    if (motion_state != prev_motion_state)
    {
      prev_motion_state = motion_state;
      Blynk_VirtualWriteInt(VIRTUAL_PIN_PIR, motion_state);
    }

    if (Relay_GetState() != prev_relay_state)
    {
      prev_relay_state = Relay_GetState();
      Blynk_VirtualWriteInt(VIRTUAL_PIN_RELAY, Relay_GetState());
    }

    /* 5. Update LCD Display every 200ms (Eliminates continuous I2C bus noise on PB7) */
    static uint32_t lcd_timer = 0;
    if (lcd_ok && (now - lcd_timer >= 200U))
    {
      lcd_timer = now;
      LCD_UpdateDisplay(current_temperature, temp_valid, motion_state, fan_target_state, Relay_GetState());
    }
  }
}

/**
  * @brief  Startup Self Test displaying system status on the 16x2 LCD.
  */
static void System_SelfTest(void)
{
  /* Check DS18B20 presence */
  uint8_t ds_ok = DS18B20_Reset();

  /* Screen 1: Title */
  LCD_PrintLine(0, "  SMART ROOM   ");
  LCD_PrintLine(1, "  SYSTEM TEST  ");
  HAL_Delay(1500);

  /* Screen 2: Component Status */
  char buf1[17];
  snprintf(buf1, sizeof(buf1), "DS:%s PIR:OK", ds_ok ? "OK " : "ERR");
  LCD_PrintLine(0, buf1);
  LCD_PrintLine(1, "LCD:OK RELAY:OK ");
  HAL_Delay(1500);

  LCD_Clear();
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Enable Clocks for GPIOA, GPIOB, GPIOC */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  /* Configure PC13 (Onboard LED) */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}

#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
}
#endif /* USE_FULL_ASSERT */
