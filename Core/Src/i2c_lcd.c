/**
  ******************************************************************************
  * @file           : i2c_lcd.c
  * @brief          : 16x2 HD44780 LCD Driver with PCF8574 I2C Backpack for STM32 HAL
  * @author         : Gopal M V
  ******************************************************************************
  * Hardware Wiring:
  * - SCL -> PB6 (Bit-bang Open-Drain)
  * - SDA -> PB7 (Bit-bang Open-Drain)
  */

#include "i2c_lcd.h"
#include "blynk.h"
#include <stdio.h>
#include <string.h>

/* I2C Pins on GPIOB */
#define SCL_PIN GPIO_PIN_6
#define SDA_PIN GPIO_PIN_7
#define I2C_PORT GPIOB

/* PCF8574 I2C Address (Auto-scans 0x01..0x7F, defaults to 0x27) */
static uint8_t lcd_i2c_addr = 0x27;

#define LCD_BACKLIGHT   0x08
#define ENABLE_BIT      0x04
#define RS_BIT          0x01

static void lcd_delay_ms(uint32_t ms)
{
  HAL_Delay(ms);
}

static void lcd_delay_us(uint32_t us)
{
  uint32_t count = us * 3U;
  while (count--)
  {
    __asm__ volatile("nop");
  }
}

/* Open-Drain Software I2C on PB6 (SCL) and PB7 (SDA) */
static void I2C_IO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  __HAL_RCC_GPIOB_CLK_ENABLE();

  GPIO_InitStruct.Pin = SCL_PIN | SDA_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(I2C_PORT, &GPIO_InitStruct);

  HAL_GPIO_WritePin(I2C_PORT, SCL_PIN, GPIO_PIN_SET);
  HAL_GPIO_WritePin(I2C_PORT, SDA_PIN, GPIO_PIN_SET);
}

static void I2C_Start(void)
{
  HAL_GPIO_WritePin(I2C_PORT, SDA_PIN, GPIO_PIN_SET);
  HAL_GPIO_WritePin(I2C_PORT, SCL_PIN, GPIO_PIN_SET);
  lcd_delay_us(20);
  HAL_GPIO_WritePin(I2C_PORT, SDA_PIN, GPIO_PIN_RESET);
  lcd_delay_us(20);
  HAL_GPIO_WritePin(I2C_PORT, SCL_PIN, GPIO_PIN_RESET);
  lcd_delay_us(20);
}

static void I2C_Stop(void)
{
  HAL_GPIO_WritePin(I2C_PORT, SDA_PIN, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(I2C_PORT, SCL_PIN, GPIO_PIN_SET);
  lcd_delay_us(20);
  HAL_GPIO_WritePin(I2C_PORT, SDA_PIN, GPIO_PIN_SET);
  lcd_delay_us(20);
}

static void I2C_BusReset(void)
{
  I2C_IO_Init();
  HAL_GPIO_WritePin(I2C_PORT, SDA_PIN, GPIO_PIN_SET);

  /* Clock 9 times to clear any stuck slave state */
  for (uint8_t i = 0; i < 9; i++)
  {
    HAL_GPIO_WritePin(I2C_PORT, SCL_PIN, GPIO_PIN_SET);
    lcd_delay_ms(1);
    HAL_GPIO_WritePin(I2C_PORT, SCL_PIN, GPIO_PIN_RESET);
    lcd_delay_ms(1);
  }

  I2C_Stop();
}

static uint8_t I2C_SendByte(uint8_t byte)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    if (byte & 0x80)
      HAL_GPIO_WritePin(I2C_PORT, SDA_PIN, GPIO_PIN_SET);
    else
      HAL_GPIO_WritePin(I2C_PORT, SDA_PIN, GPIO_PIN_RESET);

    lcd_delay_us(20);
    HAL_GPIO_WritePin(I2C_PORT, SCL_PIN, GPIO_PIN_SET);
    lcd_delay_us(20);
    HAL_GPIO_WritePin(I2C_PORT, SCL_PIN, GPIO_PIN_RESET);
    lcd_delay_us(20);
    byte <<= 1;
  }

  /* Read ACK bit */
  HAL_GPIO_WritePin(I2C_PORT, SDA_PIN, GPIO_PIN_SET); /* Release SDA */
  lcd_delay_us(20);
  HAL_GPIO_WritePin(I2C_PORT, SCL_PIN, GPIO_PIN_SET);
  lcd_delay_us(20);
  uint8_t ack = (HAL_GPIO_ReadPin(I2C_PORT, SDA_PIN) == GPIO_PIN_RESET) ? 1 : 0;
  HAL_GPIO_WritePin(I2C_PORT, SCL_PIN, GPIO_PIN_RESET);
  lcd_delay_us(20);

  return ack;
}

static uint8_t I2C_Write(uint8_t addr, uint8_t data)
{
  I2C_Start();
  if (!I2C_SendByte(addr << 1))
  {
    I2C_Stop();
    return 0;
  }
  if (!I2C_SendByte(data))
  {
    I2C_Stop();
    return 0;
  }
  I2C_Stop();
  return 1;
}

static void LCD_WriteExpander(uint8_t data)
{
  I2C_Write(lcd_i2c_addr, data | LCD_BACKLIGHT);
}

static void LCD_PulseEnable(uint8_t data)
{
  LCD_WriteExpander(data | ENABLE_BIT);
  lcd_delay_ms(1);
  LCD_WriteExpander(data & ~ENABLE_BIT);
  lcd_delay_ms(1);
}

static void LCD_SendNibble(uint8_t nibble, uint8_t mode)
{
  uint8_t data = (nibble & 0xF0) | mode;
  LCD_WriteExpander(data);
  LCD_PulseEnable(data);
}

static void LCD_Send(uint8_t value, uint8_t mode)
{
  LCD_SendNibble(value & 0xF0, mode);
  LCD_SendNibble((value << 4) & 0xF0, mode);
}

static void LCD_Command(uint8_t cmd)
{
  LCD_Send(cmd, 0);
  if (cmd == 0x01 || cmd == 0x02)
  {
    lcd_delay_ms(5);
  }
  else
  {
    lcd_delay_ms(1);
  }
}

static void LCD_Data(uint8_t data)
{
  LCD_Send(data, RS_BIT);
  lcd_delay_ms(1);
}

uint8_t LCD_Init(void)
{
  I2C_BusReset();
  lcd_delay_ms(100);

  /* Scan all I2C addresses 0x01..0x7F to find PCF8574 backpack */
  uint8_t found = 0;
  for (uint8_t a = 1; a < 128; a++)
  {
    if (I2C_Write(a, 0x00))
    {
      lcd_i2c_addr = a;
      found = 1;
      break;
    }
  }

  if (!found)
  {
    /* Common PCF8574 (0x27) and PCF8574A (0x3F) addresses */
    lcd_i2c_addr = 0x27;
  }

  /* HD44780 4-Bit Mode Hardware Reset Sequence */
  lcd_delay_ms(100);
  LCD_SendNibble(0x30, 0);
  lcd_delay_ms(10);
  LCD_SendNibble(0x30, 0);
  lcd_delay_ms(10);
  LCD_SendNibble(0x30, 0);
  lcd_delay_ms(10);
  LCD_SendNibble(0x20, 0); /* Force switch to 4-bit mode */
  lcd_delay_ms(10);

  LCD_Command(0x28); /* Function set: 4-bit, 2 lines, 5x8 dots */
  lcd_delay_ms(5);
  LCD_Command(0x08); /* Display OFF */
  lcd_delay_ms(5);
  LCD_Command(0x01); /* Clear Display */
  lcd_delay_ms(10);
  LCD_Command(0x06); /* Entry mode: increment cursor */
  lcd_delay_ms(5);
  LCD_Command(0x0C); /* Display ON, Cursor OFF, Blink OFF */
  lcd_delay_ms(5);

  return 1; /* Always enable LCD driver */
}

void LCD_Clear(void)
{
  LCD_Command(0x01);
  lcd_delay_ms(5);
}

void LCD_SetCursor(uint8_t row, uint8_t col)
{
  uint8_t address = (row == 0) ? (0x80 + col) : (0xC0 + col);
  LCD_Command(address);
}

void LCD_Print(const char *str)
{
  while (*str)
  {
    LCD_Data((uint8_t)(*str++));
  }
}

void LCD_PrintLine(uint8_t row, const char *str)
{
  char line_buf[17];
  snprintf(line_buf, sizeof(line_buf), "%-16.16s", str);
  LCD_SetCursor(row, 0);
  LCD_Print(line_buf);
}

void LCD_UpdateDisplay(float temp, uint8_t temp_valid, uint8_t motion_detected, uint8_t fan_active, uint8_t fan_state)
{
  static char prev_line1[17] = "";
  static char prev_line2[17] = "";
  static uint32_t last_screen_switch = 0;
  static uint8_t screen_mode = 0;
  char line1[17];
  char line2[17];

  uint32_t now = HAL_GetTick();
  static uint32_t last_anti_gibberish_refresh = 0;

  if (now - last_screen_switch >= 2500U)
  {
    screen_mode = !screen_mode;
    last_screen_switch = now;
  }

  /* Anti-Gibberish Recovery: Refresh LCD display registers every 5s to eliminate corrupt characters */
  if (now - last_anti_gibberish_refresh >= 5000U)
  {
    LCD_Command(0x0C); /* Enforce Display ON, Cursor OFF */
    last_anti_gibberish_refresh = now;
  }

  if (screen_mode == 0)
  {
    /* Screen 1: Temperature & Occupancy / Fan Status */
    if (temp_valid)
    {
      int temp_int = (int)temp;
      int temp_dec = (int)((temp >= 0.0f ? (temp - (float)temp_int) : ((float)temp_int - temp)) * 10.0f);
      snprintf(line1, sizeof(line1), "Temp: %d.%d C", temp_int, temp_dec);
    }
    else
    {
      snprintf(line1, sizeof(line1), "Temp: NO SENSOR");
    }

    snprintf(line2, sizeof(line2), "PIR:%-3s  Fan:%-3s", motion_detected ? "YES" : "NO ", fan_state ? "ON " : "OFF");
  }
  else
  {
    /* Screen 2: Detailed Sensor & Wi-Fi Cloud Status */
    snprintf(line1, sizeof(line1), "PIR:%-4s FAN:%-3s", motion_detected ? "DET" : "CLR", fan_state ? "ON" : "OFF");
    snprintf(line2, sizeof(line2), "WF:%-13s", Blynk_GetStateStr());
  }

  /* Only update line on LCD if content changed to eliminate LCD flicker */
  if (strcmp(line1, prev_line1) != 0)
  {
    LCD_PrintLine(0, line1);
    strncpy(prev_line1, line1, sizeof(prev_line1));
  }

  if (strcmp(line2, prev_line2) != 0)
  {
    LCD_PrintLine(1, line2);
    strncpy(prev_line2, line2, sizeof(prev_line2));
  }
}
