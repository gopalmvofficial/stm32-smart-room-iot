/**
  ******************************************************************************
  * @file    stm32f103x6.h
  * @brief   CMSIS STM32F103x6 Device Peripheral Access Layer Header File.
  ******************************************************************************
  */

#ifndef __STM32F103x6_H
#define __STM32F103x6_H

#ifdef __cplusplus
 extern "C" {
#endif

typedef enum
{
/* ----------------------  Cortex-M3 Processor Exceptions Numbers  ------------------------ */
  NonMaskableInt_IRQn         = -14,    /*!< 2 Non Maskable Interrupt                         */
  MemoryManagement_IRQn       = -12,    /*!< 4 Cortex-M3 Memory Management Interrupt          */
  BusFault_IRQn               = -11,    /*!< 5 Cortex-M3 Bus Fault Interrupt                  */
  UsageFault_IRQn             = -10,    /*!< 6 Cortex-M3 Usage Fault Interrupt                */
  SVCall_IRQn                 = -5,     /*!< 11 Cortex-M3 SV Call Interrupt                   */
  DebugMonitor_IRQn           = -4,     /*!< 12 Cortex-M3 Debug Monitor Interrupt             */
  PendSV_IRQn                 = -2,     /*!< 14 Cortex-M3 Pend SV Interrupt                   */
  SysTick_IRQn                = -1,     /*!< 15 Cortex-M3 System Tick Interrupt               */

/* ----------------------  STM32F103x6 Specific Interrupt Numbers  ------------------------ */
  WWDG_IRQn                   = 0,      /*!< Window WatchDog Interrupt                        */
  PVD_IRQn                    = 1,      /*!< PVD through EXTI Line detection Interrupt        */
  TAMPER_IRQn                 = 2,      /*!< Tamper Interrupt                                 */
  RTC_IRQn                    = 3,      /*!< RTC global Interrupt                             */
  FLASH_IRQn                  = 4,      /*!< FLASH global Interrupt                             */
  RCC_IRQn                    = 5,      /*!< RCC global Interrupt                             */
  EXTI0_IRQn                  = 6,      /*!< EXTI Line0 Interrupt                             */
  EXTI1_IRQn                  = 7,      /*!< EXTI Line1 Interrupt                             */
  EXTI2_IRQn                  = 8,      /*!< EXTI Line2 Interrupt                             */
  EXTI3_IRQn                  = 9,      /*!< EXTI Line3 Interrupt                             */
  EXTI4_IRQn                  = 10,     /*!< EXTI Line4 Interrupt                             */
  DMA1_Channel1_IRQn          = 11,     /*!< DMA1 Channel 1 global Interrupt                  */
  DMA1_Channel2_IRQn          = 12,     /*!< DMA1 Channel 2 global Interrupt                  */
  DMA1_Channel3_IRQn          = 13,     /*!< DMA1 Channel 3 global Interrupt                  */
  DMA1_Channel4_IRQn          = 14,     /*!< DMA1 Channel 4 global Interrupt                  */
  DMA1_Channel5_IRQn          = 15,     /*!< DMA1 Channel 5 global Interrupt                  */
  DMA1_Channel6_IRQn          = 16,     /*!< DMA1 Channel 6 global Interrupt                  */
  DMA1_Channel7_IRQn          = 17,     /*!< DMA1 Channel 7 global Interrupt                  */
  ADC1_2_IRQn                 = 18,     /*!< ADC1 and ADC2 global Interrupt                   */
  USB_HP_CAN1_TX_IRQn         = 19,     /*!< USB High Priority or CAN1 TX Interrupts          */
  USB_LP_CAN1_RX0_IRQn        = 20,     /*!< USB Low Priority or CAN1 RX0 Interrupts          */
  CAN1_RX1_IRQn               = 21,     /*!< CAN1 RX1 Interrupt                               */
  CAN1_SCE_IRQn               = 22,     /*!< CAN1 SCE Interrupt                               */
  EXTI9_5_IRQn                = 23,     /*!< External Line[9:5] Interrupts                    */
  TIM1_BRK_IRQn               = 24,     /*!< TIM1 Break Interrupt                             */
  TIM1_UP_IRQn                = 25,     /*!< TIM1 Update Interrupt                            */
  TIM1_TRG_COM_IRQn           = 26,     /*!< TIM1 Trigger and Commutation Interrupt           */
  TIM1_CC_IRQn                = 27,     /*!< TIM1 Capture Compare Interrupt                   */
  TIM2_IRQn                   = 28,     /*!< TIM2 global Interrupt                            */
  TIM3_IRQn                   = 29,     /*!< TIM3 global Interrupt                            */
  I2C1_EV_IRQn                = 31,     /*!< I2C1 Event Interrupt                             */
  I2C1_ER_IRQn                = 32,     /*!< I2C1 Error Interrupt                             */
  SPI1_IRQn                   = 35,     /*!< SPI1 global Interrupt                            */
  USART1_IRQn                 = 37,     /*!< USART1 global Interrupt                          */
  USART2_IRQn                 = 38,     /*!< USART2 global Interrupt                          */
  EXTI15_10_IRQn              = 40,     /*!< External Line[15:10] Interrupts                  */
  RTC_Alarm_IRQn              = 41,     /*!< RTC Alarm through EXTI Line Interrupt            */
  USBWakeUp_IRQn              = 42      /*!< USB Wakeup from suspend interrupt                */
} IRQn_Type;

#define __NVIC_PRIO_BITS          4U

#include "core_cm3.h"
#include "system_stm32f1xx.h"

#define __IO    volatile
#define __I     volatile const
#define __O     volatile

/* Peripheral Base Addresses */
#define FLASH_BASE            0x08000000UL
#define SRAM_BASE             0x20000000UL
#define PERIPH_BASE           0x40000000UL

#define APB1PERIPH_BASE       PERIPH_BASE
#define APB2PERIPH_BASE       (PERIPH_BASE + 0x00010000UL)
#define AHBPERIPH_BASE        (PERIPH_BASE + 0x00020000UL)

#define AFIO_BASE             (APB2PERIPH_BASE + 0x00000000UL)
#define GPIOA_BASE            (APB2PERIPH_BASE + 0x00000800UL)
#define GPIOB_BASE            (APB2PERIPH_BASE + 0x00000C00UL)
#define GPIOC_BASE            (APB2PERIPH_BASE + 0x00001000UL)
#define GPIOD_BASE            (APB2PERIPH_BASE + 0x00001400UL)
#define RCC_BASE              (AHBPERIPH_BASE + 0x00001000UL)
#define FLASH_R_BASE          (AHBPERIPH_BASE + 0x00002000UL)
#define PWR_BASE              (APB1PERIPH_BASE + 0x00007000UL)

/* GPIO Structure */
typedef struct
{
  __IO uint32_t CRL;
  __IO uint32_t CRH;
  __IO uint32_t IDR;
  __IO uint32_t ODR;
  __IO uint32_t BSRR;
  __IO uint32_t BRR;
  __IO uint32_t LCKR;
} GPIO_TypeDef;

/* RCC Structure */
typedef struct
{
  __IO uint32_t CR;
  __IO uint32_t CFGR;
  __IO uint32_t CIR;
  __IO uint32_t APB2RSTR;
  __IO uint32_t APB1RSTR;
  __IO uint32_t AHBENR;
  __IO uint32_t APB2ENR;
  __IO uint32_t APB1ENR;
  __IO uint32_t BDCR;
  __IO uint32_t CSR;
} RCC_TypeDef;

/* AFIO Structure */
typedef struct
{
  __IO uint32_t EVCR;
  __IO uint32_t MAPR;
  __IO uint32_t EXTICR[4];
  uint32_t RESERVED0;
  __IO uint32_t MAPR2;
} AFIO_TypeDef;

/* Peripheral definitions */
#define GPIOA               ((GPIO_TypeDef *) GPIOA_BASE)
#define GPIOB               ((GPIO_TypeDef *) GPIOB_BASE)
#define GPIOC               ((GPIO_TypeDef *) GPIOC_BASE)
#define GPIOD               ((GPIO_TypeDef *) GPIOD_BASE)
#define RCC                 ((RCC_TypeDef *) RCC_BASE)
#define AFIO                ((AFIO_TypeDef *) AFIO_BASE)

/* RCC Register Bit Definitions */
#define RCC_CR_HSION_Pos                     (0U)
#define RCC_CR_HSION_Msk                     (0x1UL << RCC_CR_HSION_Pos)
#define RCC_CR_HSION                         RCC_CR_HSION_Msk
#define RCC_CR_HSIRDY_Pos                    (1U)
#define RCC_CR_HSIRDY_Msk                    (0x1UL << RCC_CR_HSIRDY_Pos)
#define RCC_CR_HSIRDY                        RCC_CR_HSIRDY_Msk
#define RCC_CR_HSITRIM_Pos                   (3U)
#define RCC_CR_HSITRIM_Msk                   (0x1FUL << RCC_CR_HSITRIM_Pos)
#define RCC_CR_HSEON_Pos                     (16U)
#define RCC_CR_HSEON_Msk                     (0x1UL << RCC_CR_HSEON_Pos)
#define RCC_CR_HSEON                         RCC_CR_HSEON_Msk
#define RCC_CR_HSERDY_Pos                    (17U)
#define RCC_CR_HSERDY_Msk                    (0x1UL << RCC_CR_HSERDY_Pos)
#define RCC_CR_HSERDY                        RCC_CR_HSERDY_Msk

#define RCC_CFGR_SW_Pos                      (0U)
#define RCC_CFGR_SW_Msk                      (0x3UL << RCC_CFGR_SW_Pos)
#define RCC_CFGR_SWS_Pos                     (2U)
#define RCC_CFGR_SWS_Msk                     (0x3UL << RCC_CFGR_SWS_Pos)
#define RCC_CFGR_SWS                         RCC_CFGR_SWS_Msk
#define RCC_CFGR_HPRE_Pos                    (4U)
#define RCC_CFGR_HPRE_Msk                    (0xFUL << RCC_CFGR_HPRE_Pos)
#define RCC_CFGR_HPRE                        RCC_CFGR_HPRE_Msk
#define RCC_CFGR_PPRE1_Pos                   (8U)
#define RCC_CFGR_PPRE1_Msk                   (0x7UL << RCC_CFGR_PPRE1_Pos)
#define RCC_CFGR_PPRE2_Pos                   (11U)
#define RCC_CFGR_PPRE2_Msk                   (0x7UL << RCC_CFGR_PPRE2_Pos)
#define RCC_CFGR_PLLSRC_Pos                  (16U)
#define RCC_CFGR_PLLSRC                      (0x1UL << RCC_CFGR_PLLSRC_Pos)
#define RCC_CFGR_PLLXTPRE_Pos                (17U)
#define RCC_CFGR_PLLXTPRE                    (0x1UL << RCC_CFGR_PLLXTPRE_Pos)
#define RCC_CFGR_PLLMULL_Pos                 (18U)
#define RCC_CFGR_PLLMULL                     (0xFUL << RCC_CFGR_PLLMULL_Pos)

#define RCC_APB2ENR_AFIOEN_Pos               (0U)
#define RCC_APB2ENR_AFIOEN_Msk               (0x1UL << RCC_APB2ENR_AFIOEN_Pos)
#define RCC_APB2ENR_AFIOEN                   RCC_APB2ENR_AFIOEN_Msk
#define RCC_APB2ENR_IOPAEN_Pos               (2U)
#define RCC_APB2ENR_IOPAEN_Msk               (0x1UL << RCC_APB2ENR_IOPAEN_Pos)
#define RCC_APB2ENR_IOPAEN                   RCC_APB2ENR_IOPAEN_Msk
#define RCC_APB2ENR_IOPBEN_Pos               (3U)
#define RCC_APB2ENR_IOPBEN_Msk               (0x1UL << RCC_APB2ENR_IOPBEN_Pos)
#define RCC_APB2ENR_IOPBEN                   RCC_APB2ENR_IOPBEN_Msk
#define RCC_APB2ENR_IOPCEN_Pos               (4U)
#define RCC_APB2ENR_IOPCEN_Msk               (0x1UL << RCC_APB2ENR_IOPCEN_Pos)
#define RCC_APB2ENR_IOPCEN                   RCC_APB2ENR_IOPCEN_Msk
#define RCC_APB2ENR_IOPDEN_Pos               (5U)
#define RCC_APB2ENR_IOPDEN_Msk               (0x1UL << RCC_APB2ENR_IOPDEN_Pos)
#define RCC_APB2ENR_IOPDEN                   RCC_APB2ENR_IOPDEN_Msk
#define RCC_APB1ENR_PWREN_Pos                (28U)
#define RCC_APB1ENR_PWREN_Msk                (0x1UL << RCC_APB1ENR_PWREN_Pos)
#define RCC_APB1ENR_PWREN                    RCC_APB1ENR_PWREN_Msk

#define AFIO_MAPR_SWJ_CFG_Pos                (24U)
#define AFIO_MAPR_SWJ_CFG_Msk                (0x7UL << AFIO_MAPR_SWJ_CFG_Pos)
#define AFIO_MAPR_SWJ_CFG_NOJTAG             (0x2UL << AFIO_MAPR_SWJ_CFG_Pos)

#ifdef __cplusplus
}
#endif

#endif /* __STM32F103x6_H */
