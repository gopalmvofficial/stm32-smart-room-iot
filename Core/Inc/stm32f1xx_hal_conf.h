/**
  ******************************************************************************
  * @file    stm32f1xx_hal_conf.h
  * @brief   HAL configuration file.
  ******************************************************************************
  */

#ifndef __STM32F1xx_HAL_CONF_H
#define __STM32F1xx_HAL_CONF_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Module Selection */
#define HAL_MODULE_ENABLED
#define HAL_CORTEX_MODULE_ENABLED
#define HAL_GPIO_MODULE_ENABLED
#define HAL_RCC_MODULE_ENABLED
#define HAL_FLASH_MODULE_ENABLED
#define HAL_PWR_MODULE_ENABLED

/* Oscillator Values */
#if !defined  (HSI_VALUE)
  #define HSI_VALUE    ((uint32_t)8000000) /* Value of the Internal oscillator in Hz*/
#endif

#if !defined  (HSE_VALUE)
  #define HSE_VALUE    ((uint32_t)8000000) /* Value of the External oscillator in Hz */
#endif

#if !defined  (HSE_STARTUP_TIMEOUT)
  #define HSE_STARTUP_TIMEOUT    ((uint32_t)100)   /* Time out for HSE start up, in ms */
#endif

#if !defined  (LSI_VALUE)
  #define LSI_VALUE    ((uint32_t)40000)   /* LSI Typical Value in Hz */
#endif

#if !defined  (LSE_VALUE)
  #define LSE_VALUE    ((uint32_t)32768)   /* Value of the External Low Speed oscillator in Hz */
#endif

#if !defined  (LSE_STARTUP_TIMEOUT)
  #define LSE_STARTUP_TIMEOUT    ((uint32_t)5000)   /* Time out for LSE start up, in ms */
#endif

/* System Configuration */
#define  VDD_VALUE                    ((uint32_t)3300) /* Value of VDD in mv */
#define  TICK_INT_PRIORITY            ((uint32_t)15)   /* tick interrupt priority */
#define  USE_RTOS                     0
#define  PREFETCH_ENABLE              1

/* Includes */
#ifdef HAL_RCC_MODULE_ENABLED
  #include "stm32f1xx_hal_rcc.h"
#endif

#ifdef HAL_GPIO_MODULE_ENABLED
  #include "stm32f1xx_hal_gpio.h"
#endif

#ifdef HAL_CORTEX_MODULE_ENABLED
  #include "stm32f1xx_hal_cortex.h"
#endif

/* Assert Selection */
#define USE_FULL_ASSERT    0U

#ifdef __cplusplus
}
#endif

#endif /* __STM32F1xx_HAL_CONF_H */
