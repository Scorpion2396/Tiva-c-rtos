/* Board Support Package */
#include "TM4C123GH6PM.h"
#include "bsp.h"

uint32_t SystemCoreClock = SYS_CLOCK_HZ;

/**************************************************************************/
__attribute__((naked)) void assert_failed (char const *file, int line)
{
    /* TBD: damage control */
    NVIC_SystemReset(); /* reset the system */
}

/**************************************************************************/
void systick_init()
{
    /* configuring systick irq time duration to be triggered */
    SysTick->LOAD = (SYS_CLOCK_HZ / BSP_TICK_PER_SEC) - 1U ;
    SysTick->VAL  = 0U;
    SysTick->CTRL = (1U << 2) | (1U << 1) | (1U << 0);
}


/**************************************************************************/
void BSP_init(void)
{
    systick_init();
}




/**************************************************************************/
void fpu_init()
{
    #if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
    SCB->CPACR |= ((3UL << 10 * 2) |  (3UL << 11 * 2));  /* set CP10 and CP11 Full Access */
    #endif
}

/**************************************************************************/
void SystemInit(void)
{
    fpu_init();
}




