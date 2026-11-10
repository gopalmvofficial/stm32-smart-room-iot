#ifndef __CORE_CM3_H_GENERIC
#define __CORE_CM3_H_GENERIC

#include <stdint.h>
#include "cmsis_version.h"
#include "cmsis_compiler.h"

#ifdef __cplusplus
 extern "C" {
#endif

#ifndef __IO
#define __IO volatile
#endif
#ifndef __I
#define __I volatile const
#endif
#ifndef __O
#define __O volatile
#endif

typedef struct
{
  __IO uint32_t CTRL;                  /*!< Offset: 0x000 (R/W)  SysTick Control and Status Register */
  __IO uint32_t LOAD;                  /*!< Offset: 0x004 (R/W)  SysTick Reload Value Register */
  __IO uint32_t VAL;                   /*!< Offset: 0x008 (R/ )  SysTick Current Value Register */
  __I  uint32_t CALIB;                 /*!< Offset: 0x00C (R/ )  SysTick Calibration Register */
} SysTick_Type;

#define SysTick_BASE        (0xE000E010UL)                            /*!< SysTick Base Address */
#define SysTick             ((SysTick_Type *) SysTick_BASE)           /*!< SysTick configuration struct */

#define SysTick_CTRL_CLKSOURCE_Pos          2U                                            /*!< SysTick CTRL: CLKSOURCE Position */
#define SysTick_CTRL_CLKSOURCE_Msk          (1UL << SysTick_CTRL_CLKSOURCE_Pos)           /*!< SysTick CTRL: CLKSOURCE Mask */

#define SysTick_CTRL_TICKINT_Pos            1U                                            /*!< SysTick CTRL: TICKINT Position */
#define SysTick_CTRL_TICKINT_Msk            (1UL << SysTick_CTRL_TICKINT_Pos)             /*!< SysTick CTRL: TICKINT Mask */

#define SysTick_CTRL_ENABLE_Pos             0U                                            /*!< SysTick CTRL: ENABLE Position */
#define SysTick_CTRL_ENABLE_Msk             (1UL << SysTick_CTRL_ENABLE_Pos)              /*!< SysTick CTRL: ENABLE Mask */

#define SysTick_LOAD_RELOAD_Pos             0U                                            /*!< SysTick LOAD: RELOAD Position */
#define SysTick_LOAD_RELOAD_Msk             (0xFFFFFFUL << SysTick_LOAD_RELOAD_Pos)       /*!< SysTick LOAD: RELOAD Mask */

typedef struct
{
  __I  uint32_t CPUID;                  /*!< Offset: 0x000 (R/ )  CPUID Base Register */
  __IO uint32_t ICSR;                   /*!< Offset: 0x004 (R/W)  Interrupt Control and State Register */
  __IO uint32_t VTOR;                   /*!< Offset: 0x008 (R/W)  Vector Table Offset Register */
  __IO uint32_t AIRCR;                  /*!< Offset: 0x00C (R/W)  Application Interrupt and Reset Control Register */
  __IO uint32_t SCR;                    /*!< Offset: 0x010 (R/W)  System Control Register */
  __IO uint32_t CCR;                    /*!< Offset: 0x014 (R/W)  Configuration Control Register */
  __IO uint8_t  SHP[12U];               /*!< Offset: 0x018 (R/W)  System Handlers Priority Registers (4-7, 8-11, 12-15) */
  __IO uint32_t SHCSR;                  /*!< Offset: 0x024 (R/W)  System Handler Control and State Register */
  __IO uint32_t CFSR;                   /*!< Offset: 0x028 (R/W)  Configurable Fault Status Register */
  __IO uint32_t HFSR;                   /*!< Offset: 0x02C (R/W)  HardFault Status Register */
  __IO uint32_t DFSR;                   /*!< Offset: 0x030 (R/W)  Debug Fault Status Register */
  __IO uint32_t MMFAR;                  /*!< Offset: 0x034 (R/W)  MemManage Fault Address Register */
  __IO uint32_t BFAR;                   /*!< Offset: 0x038 (R/W)  BusFault Address Register */
  __IO uint32_t AFSR;                   /*!< Offset: 0x03C (R/W)  Auxiliary Fault Status Register */
} SCB_Type;

#define SystemHome_BASE      (0xE000E000UL)
#define SCB_BASE            (SystemHome_BASE +  0x0D00UL)
#define SCB                 ((SCB_Type       *)     SCB_BASE      )

typedef struct
{
  __IO uint32_t ISER[8U];               /*!< Offset: 0x000 (R/W)  Interrupt Set Enable Register */
        uint32_t RESERVED0[24U];
  __IO uint32_t ICER[8U];               /*!< Offset: 0x080 (R/W)  Interrupt Clear Enable Register */
        uint32_t RSERVED1[24U];
  __IO uint32_t ISPR[8U];               /*!< Offset: 0x100 (R/W)  Interrupt Set Pending Register */
        uint32_t RESERVED2[24U];
  __IO uint32_t ICPR[8U];               /*!< Offset: 0x180 (R/W)  Interrupt Clear Pending Register */
        uint32_t RESERVED3[24U];
  __IO uint32_t IABR[8U];               /*!< Offset: 0x200 (R/W)  Interrupt Active bit Register */
        uint32_t RESERVED4[56U];
  __IO uint8_t  IP[240U];               /*!< Offset: 0x300 (R/W)  Interrupt Priority Register (8Bit wide) */
        uint32_t RESERVED5[644U];
  __O  uint32_t STIR;                   /*!< Offset: 0xE00 ( /W)  Software Trigger Interrupt Register */
}  NVIC_Type;

#define NVIC_BASE           (SystemHome_BASE +  0x0100UL)
#define NVIC                ((NVIC_Type      *)     NVIC_BASE     )

typedef struct
{
  __IO uint32_t CTRL;                  /*!< Offset: 0x000 (R/W)  Control Register */
  __IO uint32_t CYCCNT;                /*!< Offset: 0x004 (R/W)  Cycle Count Register */
  __IO uint32_t CPICNT;                /*!< Offset: 0x008 (R/W)  CPI Count Register */
  __IO uint32_t EXCCNT;                /*!< Offset: 0x00C (R/W)  Exception Count Register */
  __IO uint32_t SLEEPCNT;              /*!< Offset: 0x010 (R/W)  Sleep Count Register */
  __IO uint32_t LSUCNT;                /*!< Offset: 0x014 (R/W)  LSU Count Register */
  __IO uint32_t FOLDCNT;               /*!< Offset: 0x018 (R/W)  Fold Count Register */
  __IO uint32_t PCSR;                  /*!< Offset: 0x01C (R/ )  Program Counter Sample Register */
} DWT_Type;

#define DWT_BASE            (0xE0001000UL)
#define DWT                 ((DWT_Type *) DWT_BASE)
#define DWT_CTRL_CYCCNTENA_Pos             0U
#define DWT_CTRL_CYCCNTENA_Msk             (1UL << DWT_CTRL_CYCCNTENA_Pos)

typedef struct
{
  __IO uint32_t DHCSR;                 /*!< Offset: 0x000 (R/W)  Debug Halting Control and Status Register */
  __O  uint32_t DCRSR;                 /*!< Offset: 0x004 ( /W)  Debug Core Register Selector Register */
  __IO uint32_t DCRDR;                 /*!< Offset: 0x008 (R/W)  Debug Core Register Data Register */
  __IO uint32_t DEMCR;                 /*!< Offset: 0x00C (R/W)  Debug Exception and Monitor Control Register */
} CoreDebug_Type;

#define CoreDebug_BASE      (0xE000EDF0UL)
#define CoreDebug           ((CoreDebug_Type *) CoreDebug_BASE)
#define CoreDebug_DEMCR_TRCENA_Pos         24U
#define CoreDebug_DEMCR_TRCENA_Msk         (1UL << CoreDebug_DEMCR_TRCENA_Pos)

/* Forward declaration */
__STATIC_INLINE void NVIC_SetPriority(IRQn_Type IRQn, uint32_t priority);

__STATIC_INLINE uint32_t SysTick_Config(uint32_t ticks)
{
  if ((ticks - 1UL) > SysTick_LOAD_RELOAD_Msk)
  {
    return (1UL);                                                   /* Reload value impossible */
  }

  SysTick->LOAD  = (uint32_t)(ticks - 1UL);                         /* set reload register */
  NVIC_SetPriority (SysTick_IRQn, (1UL << __NVIC_PRIO_BITS) - 1UL); /* set Priority for Systick Interrupt */
  SysTick->VAL   = 0UL;                                             /* Load the SysTick Counter Value */
  SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |
                   SysTick_CTRL_TICKINT_Msk   |
                   SysTick_CTRL_ENABLE_Msk;                         /* Enable SysTick IRQ and SysTick Timer */
  return (0UL);                                                     /* Function successful */
}

__STATIC_INLINE void NVIC_SetPriority(IRQn_Type IRQn, uint32_t priority)
{
  if ((int32_t)(IRQn) < 0)
  {
    SCB->SHP[(((uint32_t)IRQn) & 0xFUL)-4UL] = (uint8_t)((priority << (8 - __NVIC_PRIO_BITS)) & (uint32_t)0xFFUL);
  }
  else
  {
    NVIC->IP[(uint32_t)IRQn] = (uint8_t)((priority << (8 - __NVIC_PRIO_BITS)) & (uint32_t)0xFFUL);
  }
}

__STATIC_INLINE void NVIC_EnableIRQ(IRQn_Type IRQn)
{
  if ((int32_t)(IRQn) >= 0)
  {
    NVIC->ISER[(((uint32_t)IRQn) >> 5UL)] = (uint32_t)(1UL << (((uint32_t)IRQn) & 0x1FUL));
  }
}

__STATIC_INLINE void NVIC_SetPriorityGrouping(uint32_t PriorityGroup)
{
  uint32_t reg_value;
  uint32_t PriorityGroupTmp = (PriorityGroup & (uint32_t)0x07UL);             /* only values 0..7 are used          */

  reg_value  =  SCB->AIRCR;                                                   /* read old register value            */
  reg_value &= ~((uint32_t)(0xFFFFUL << 16U) | (uint32_t)(7UL << 8U));        /* clear bits to change               */
  reg_value  =  (reg_value                                 |
                ((uint32_t)0x05FAUL << 16U)                |
                (PriorityGroupTmp << 8U)                    );                /* insert write key and prio group    */
  SCB->AIRCR =  reg_value;
}

#ifdef __cplusplus
}
#endif

#endif
