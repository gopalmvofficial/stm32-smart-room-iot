#ifndef __CMSIS_GCC_H
#define __CMSIS_GCC_H

#include <stdint.h>

#ifndef __COMPILER_BARRIER
  #define __COMPILER_BARRIER()                   __asm__ __volatile__("":::"memory")
#endif

#ifndef __NOP
  #define __NOP()                                __asm__ __volatile__ ("nop")
#endif
#ifndef __WFI
  #define __WFI()                                __asm__ __volatile__ ("wfi")
#endif
#ifndef __WFE
  #define __WFE()                                __asm__ __volatile__ ("wfe")
#endif
#ifndef __SEV
  #define __SEV()                                __asm__ __volatile__ ("sev")
#endif
#ifndef __ISB
  #define __ISB()                                __asm__ __volatile__ ("isb 0xF":::"memory")
#endif
#ifndef __DSB
  #define __DSB()                                __asm__ __volatile__ ("dsb 0xF":::"memory")
#endif
#ifndef __DMB
  #define __DMB()                                __asm__ __volatile__ ("dmb 0xF":::"memory")
#endif

__STATIC_FORCEINLINE void __disable_irq(void)
{
  __asm__ __volatile__ ("cpsid i" : : : "memory");
}

__STATIC_FORCEINLINE void __enable_irq(void)
{
  __asm__ __volatile__ ("cpsie i" : : : "memory");
}

__STATIC_FORCEINLINE uint32_t __get_PRIMASK(void)
{
  uint32_t result;
  __asm__ __volatile__ ("MRS %0, primask" : "=r" (result) );
  return(result);
}

__STATIC_FORCEINLINE void __set_PRIMASK(uint32_t priMask)
{
  __asm__ __volatile__ ("MSR primask, %0" : : "r" (priMask) : "memory");
}

#endif
