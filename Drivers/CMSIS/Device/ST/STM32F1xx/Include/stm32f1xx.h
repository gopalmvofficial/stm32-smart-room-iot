/**
  ******************************************************************************
  * @file    stm32f1xx.h
  * @brief   CMSIS STM32F1xx Device Peripheral Access Layer Header File.
  ******************************************************************************
  */

#ifndef __STM32F1xx_H
#define __STM32F1xx_H

#ifdef __cplusplus
 extern "C" {
#endif

#if !defined (STM32F103x6)
  #define STM32F103x6
#endif

#if defined(STM32F103x6)
  #include "stm32f103x6.h"
#else
 #error "Please select first the target STM32F1xx device used in your application"
#endif

typedef enum
{
  RESET = 0U,
  SET = !RESET
} FlagStatus, ITStatus;

typedef enum
{
  DISABLE = 0U,
  ENABLE = !DISABLE
} FunctionalState;

typedef enum
{
  SUCCESS = 0U,
  ERROR = !SUCCESS
} ErrorStatus;

#ifdef __cplusplus
}
#endif

#endif /* __STM32F1xx_H */
