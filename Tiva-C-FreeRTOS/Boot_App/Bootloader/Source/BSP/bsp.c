/* Board Support Package */
#include "TM4C123GH6PM.h"
#include "bsp.h"

#include "GPIO_Driver_Interface.h"

static uint32_t volatile l_tickCtr;

__attribute__((naked)) void assert_failed (char const *file, int line)
{
    /* TBD: damage control */
    NVIC_SystemReset(); /* reset the system */
}

