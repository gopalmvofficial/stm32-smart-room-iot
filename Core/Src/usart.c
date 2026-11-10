/**
  ******************************************************************************
  * @file           : usart.c
  * @brief          : Hardware USART1 Driver (PA9 = TX, PA10 = RX)
  * @author         : Gopal M V
  ******************************************************************************
  */

#include "usart.h"

#define RX_BUFFER_SIZE 512

static char rx_buffer[RX_BUFFER_SIZE];
static volatile uint16_t rx_head = 0;
static volatile uint16_t rx_tail = 0;

void USART1_Init(uint32_t baudrate)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Enable Clocks for GPIOA and USART1 */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_USART1_CLK_ENABLE();

  /* PA9 -> USART1_TX (Alternate Function Push-Pull) */
  GPIO_InitStruct.Pin = GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* PA10 -> USART1_RX (Input Floating / Pull-up) */
  GPIO_InitStruct.Pin = GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* Disable USART1 before config */
  USART1->CR1 = 0;

  /* Calculate Baud Rate Dividend */
  uint32_t pclk2 = SystemCoreClock;
  USART1->BRR = (pclk2 + (baudrate / 2U)) / baudrate;

  /* Enable TX, RX, and USART1 */
  USART1->CR1 = USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;

  rx_head = 0;
  rx_tail = 0;
}

void USART1_WriteChar(char c)
{
  while (!(USART1->SR & USART_SR_TXE));
  USART1->DR = (uint16_t)c;
}

void USART1_WriteString(const char *str)
{
  while (*str)
  {
    USART1_WriteChar(*str++);
  }
}

void USART1_WriteBytes(const uint8_t *data, uint16_t len)
{
  for (uint16_t i = 0; i < len; i++)
  {
    USART1_WriteChar((char)data[i]);
  }
}

uint8_t USART1_Available(void)
{
  /* Check hardware RXNE flag and buffer in incoming byte */
  if (USART1->SR & USART_SR_RXNE)
  {
    char c = (char)(USART1->DR & 0xFF);
    uint16_t next_head = (rx_head + 1) % RX_BUFFER_SIZE;
    if (next_head != rx_tail)
    {
      rx_buffer[rx_head] = c;
      rx_head = next_head;
    }
  }

  return (rx_head != rx_tail);
}

int USART1_ReadChar(void)
{
  if (USART1_Available())
  {
    char c = rx_buffer[rx_tail];
    rx_tail = (rx_tail + 1) % RX_BUFFER_SIZE;
    return (int)(unsigned char)c;
  }
  return -1;
}

void USART1_Flush(void)
{
  rx_head = 0;
  rx_tail = 0;
}
