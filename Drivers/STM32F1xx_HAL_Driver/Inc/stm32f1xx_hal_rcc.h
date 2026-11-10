/**
  ******************************************************************************
  * @file    stm32f1xx_hal_rcc.h
  * @brief   Header file of RCC HAL module.
  ******************************************************************************
  */

#ifndef __STM32F1xx_HAL_RCC_H
#define __STM32F1xx_HAL_RCC_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f1xx_hal_def.h"

typedef struct
{
  uint32_t PLLState;      
  uint32_t PLLSource;     
  uint32_t PLLMUL;        
} RCC_PLLInitTypeDef;

typedef struct
{
  uint32_t OscillatorType;       
  uint32_t HSEState;             
  uint32_t HSIState;             
  uint32_t HSICalibrationValue;  
  uint32_t LSEState;             
  uint32_t LSIState;             
  RCC_PLLInitTypeDef PLL;        
} RCC_OscInitTypeDef;

typedef struct
{
  uint32_t ClockType;             
  uint32_t SYSCLKSource;          
  uint32_t AHBCLKDivider;         
  uint32_t APB1CLKDivider;        
  uint32_t APB2CLKDivider;        
} RCC_ClkInitTypeDef;

#define RCC_OSCILLATORTYPE_NONE            0x00000000U
#define RCC_OSCILLATORTYPE_HSE             0x00000001U
#define RCC_OSCILLATORTYPE_HSI             0x00000002U
#define RCC_OSCILLATORTYPE_LSE             0x00000004U
#define RCC_OSCILLATORTYPE_LSI             0x00000008U

#define RCC_HSI_OFF                        0x00000000U
#define RCC_HSI_ON                         0x00000001U
#define RCC_HSICALIBRATION_DEFAULT         0x10U

#define RCC_PLL_NONE                       0x00000000U
#define RCC_PLL_ON                         0x00000001U

#define RCC_CLOCKTYPE_SYSCLK               0x00000001U
#define RCC_CLOCKTYPE_HCLK                 0x00000002U
#define RCC_CLOCKTYPE_PCLK1                0x00000004U
#define RCC_CLOCKTYPE_PCLK2                0x00000008U

#define RCC_SYSCLKSOURCE_HSI               0x00000000U
#define RCC_SYSCLKSOURCE_HSE               0x00000001U
#define RCC_SYSCLKSOURCE_PLLCLK            0x00000002U

#define RCC_SYSCLK_DIV1                    0x00000000U
#define RCC_HCLK_DIV1                      0x00000000U

#define FLASH_LATENCY_0                    0x00000000U

#define __HAL_RCC_GPIOA_CLK_ENABLE()   do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPAEN); \
                                        tmpreg = READ_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPAEN); \
                                        UNUSED(tmpreg); \
                                      } while(0U)

#define __HAL_RCC_GPIOB_CLK_ENABLE()   do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPBEN); \
                                        tmpreg = READ_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPBEN); \
                                        UNUSED(tmpreg); \
                                      } while(0U)

#define __HAL_RCC_GPIOC_CLK_ENABLE()   do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPCEN); \
                                        tmpreg = READ_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPCEN); \
                                        UNUSED(tmpreg); \
                                      } while(0U)

#define __HAL_RCC_GPIOD_CLK_ENABLE()   do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPDEN); \
                                        tmpreg = READ_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPDEN); \
                                        UNUSED(tmpreg); \
                                      } while(0U)

#define __HAL_RCC_AFIO_CLK_ENABLE()    do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCC->APB2ENR, RCC_APB2ENR_AFIOEN); \
                                        tmpreg = READ_BIT(RCC->APB2ENR, RCC_APB2ENR_AFIOEN); \
                                        UNUSED(tmpreg); \
                                      } while(0U)

#define __HAL_RCC_PWR_CLK_ENABLE()     do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCC->APB1ENR, RCC_APB1ENR_PWREN); \
                                        tmpreg = READ_BIT(RCC->APB1ENR, RCC_APB1ENR_PWREN); \
                                        UNUSED(tmpreg); \
                                      } while(0U)

#define SET_BIT(REG, BIT)     ((REG) |= (BIT))
#define CLEAR_BIT(REG, BIT)   ((REG) &= ~(BIT))
#define READ_BIT(REG, BIT)    ((REG) & (BIT))

HAL_StatusTypeDef HAL_RCC_OscConfig(RCC_OscInitTypeDef  *RCC_OscInitStruct);
HAL_StatusTypeDef HAL_RCC_ClockConfig(RCC_ClkInitTypeDef  *RCC_ClkInitStruct, uint32_t FLatency);

#ifdef __cplusplus
}
#endif

#endif /* __STM32F1xx_HAL_RCC_H */
