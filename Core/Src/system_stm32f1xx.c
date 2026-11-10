/**
  ******************************************************************************
  * @file    system_stm32f1xx.c
  * @brief   CMSIS Cortex-M3 Device Peripheral Access Layer System Source File.
  ******************************************************************************
  */

#include "stm32f1xx.h"

#ifndef HSI_VALUE
  #define HSI_VALUE    ((uint32_t)8000000U) /* Value of Internal oscillator in Hz */
#endif

#ifndef HSE_VALUE
  #define HSE_VALUE    ((uint32_t)8000000U) /* Value of External oscillator in Hz */
#endif

uint32_t SystemCoreClock = 8000000;
const uint8_t AHBPrescTable[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};
const uint8_t APBPrescTable[8] = {0, 0, 0, 0, 1, 2, 3, 4};

void SystemInit (void)
{
  /* Reset the RCC clock configuration to default reset state */
  RCC->CR |= (uint32_t)0x00000001;
  RCC->CFGR &= (uint32_t)0xF0FF0000;
  RCC->CR &= (uint32_t)0xFEF6FFFF;
  RCC->CR &= (uint32_t)0xFFFBFFFF;
  RCC->CFGR &= (uint32_t)0xFF80FFFF;
  RCC->CIR = 0x009F0000;
}

void SystemCoreClockUpdate (void)
{
  uint32_t tmp = 0, pllmull = 0, pllsource = 0;

  tmp = RCC->CFGR & RCC_CFGR_SWS;

  switch (tmp)
  {
    case 0x00:  /* HSI used as system clock */
      SystemCoreClock = HSI_VALUE;
      break;
    case 0x04:  /* HSE used as system clock */
      SystemCoreClock = HSE_VALUE;
      break;
    case 0x08:  /* PLL used as system clock */
      pllmull = RCC->CFGR & RCC_CFGR_PLLMULL;
      pllsource = RCC->CFGR & RCC_CFGR_PLLSRC;

      pllmull = ( pllmull >> 18) + 2;

      if (pllsource == 0x00)
      {
        /* HSI oscillator clock divided by 2 selected as PLL clock entry */
        SystemCoreClock = (HSI_VALUE >> 1) * pllmull;
      }
      else
      {
        /* HSE selected as PLL clock entry */
        if ((RCC->CFGR & RCC_CFGR_PLLXTPRE) != (uint32_t)RESET)
        {/* HSE oscillator clock divided by 2 */
          SystemCoreClock = (HSE_VALUE >> 1) * pllmull;
        }
        else
        {
          SystemCoreClock = HSE_VALUE * pllmull;
        }
      }
      break;

    default:
      SystemCoreClock = HSI_VALUE;
      break;
  }

  /* Compute HCLK clock frequency --------------------------------------------*/
  tmp = AHBPrescTable[((RCC->CFGR & RCC_CFGR_HPRE) >> 4)];
  SystemCoreClock >>= tmp;
}
