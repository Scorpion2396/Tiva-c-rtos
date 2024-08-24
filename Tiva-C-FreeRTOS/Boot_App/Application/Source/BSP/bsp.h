#ifndef __BSP_H__
#define __BSP_H__

#include "TM4C123GH6PM.h"
/* Board Support Package for the EK-TM4C123GXL board */

/* system clock setting [Hz] */
#define SYS_CLOCK_HZ 16000000U
#define BSP_TICK_PER_SEC 1000U

void BSP_init();

#endif // __BSP_H__
