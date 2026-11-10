/**
  ******************************************************************************
  * @file           : usart.h
  * @brief          : USART1 Hardware Driver Header for STM32F103 (PA9=TX, PA10=RX)
  ******************************************************************************
  */

#ifndef __USART_H
#define __USART_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

/* USART Register Struct & Definitions */
typedef struct
{
  __IO uint32_t SR;
  __IO uint32_t DR;
  __IO uint32_t BRR;
  __IO uint32_t CR1;
  __IO uint32_t CR2;
  __IO uint32_t CR3;
  __IO uint32_t GTPR;
} USART_TypeDef;

#ifndef USART1_BASE
#define USART1_BASE          (APB2PERIPH_BASE + 0x00003800UL)
#endif

#ifndef USART1
#define USART1               ((USART_TypeDef *) USART1_BASE)
#endif

#ifndef USART_SR_RXNE
#define USART_SR_RXNE        (1U << 5U)
#endif

#ifndef USART_SR_TXE
#define USART_SR_TXE         (1U << 7U)
#endif

#ifndef USART_CR1_RE
#define USART_CR1_RE         (1U << 2U)
#endif

#ifndef USART_CR1_TE
#define USART_CR1_TE         (1U << 3U)
#endif

#ifndef USART_CR1_UE
#define USART_CR1_UE         (1U << 13U)
#endif

#ifndef RCC_APB2ENR_USART1EN
#define RCC_APB2ENR_USART1EN (1U << 14U)
#endif

#ifndef __HAL_RCC_USART1_CLK_ENABLE
#define __HAL_RCC_USART1_CLK_ENABLE() (RCC->APB2ENR |= RCC_APB2ENR_USART1EN)
#endif

#ifndef GPIO_MODE_AF_PP
#define GPIO_MODE_AF_PP      0x00000010U
#endif

/* Function Prototypes */
void USART1_Init(uint32_t baudrate);
void USART1_WriteChar(char c);
void USART1_WriteString(const char *str);
void USART1_WriteBytes(const uint8_t *data, uint16_t len);
int USART1_ReadChar(void);
uint8_t USART1_Available(void);
void USART1_Flush(void);

#ifdef __cplusplus
}
#endif

#endif /* __USART_H */
