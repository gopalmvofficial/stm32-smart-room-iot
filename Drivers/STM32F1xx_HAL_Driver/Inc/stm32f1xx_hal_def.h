/**
  ******************************************************************************
  * @file    stm32f1xx_hal_def.h
  * @brief   This file contains HAL common defines, enumeration, macros and
  *          structures definitions.
  ******************************************************************************
  */

#ifndef __STM32F1xx_HAL_DEF
#define __STM32F1xx_HAL_DEF

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f1xx.h"
#include <stddef.h>

typedef enum
{
  HAL_OK       = 0x00U,
  HAL_ERROR    = 0x01U,
  HAL_BUSY     = 0x02U,
  HAL_TIMEOUT  = 0x03U
} HAL_StatusTypeDef;

typedef enum
{
  HAL_UNLOCKED = 0x00U,
  HAL_LOCKED   = 0x01U
} HAL_LockTypeDef;

#define HAL_MAX_DELAY      0xFFFFFFFFU

#define __HAL_LINKDMA(__HANDLE__, __PPP_DMA_FIELD__, __DMA_HANDLE__)               \
                        do{                                                      \
                              (__HANDLE__)->__PPP_DMA_FIELD__ = &(__DMA_HANDLE__); \
                              (__DMA_HANDLE__).Parent = (__HANDLE__);             \
                          } while(0U)

#define UNUSED(X) (void)X      /* To avoid gcc/g++ warnings */

#ifdef __cplusplus
}
#endif

#endif /* __STM32F1xx_HAL_DEF */
