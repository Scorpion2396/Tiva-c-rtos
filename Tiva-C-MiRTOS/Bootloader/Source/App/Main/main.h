/*
 * main.h
 *
 *  Created on: 16-May-2023
 *      Author: saksh
 */

#ifndef SOURCE_APP_MAIN_MAIN_H_
#define SOURCE_APP_MAIN_MAIN_H_

#include "TM4C123GH6PM.h"
#include "GPIO_Driver_Interface.h"
#include "Common_Datatype.h"
#include "Common_Macros.h"
#include "I2C_Interface.h"
#include "UART_Interface.h"
#include "SSD1306_Interface.h"
#include "FLASH_Interface.h"

/******************************************************************************/

#define APPS_FLASH_START_ADDR    (unsigned int)0x00020000
#define APPS_STACK_PTR_ADDR      (*((unsigned int*)(APPS_FLASH_START_ADDR)))
#define APPS_RESET_HANDLER_ADDR  APPS_FLASH_START_ADDR + 4
#define APPS_INVALID             0xFFFFFFFF


/******************************************************************************/
typedef void (*fun_ptr)(void);

/******************************************************************************/

/******************************************************************************/
void Delay_ms(unsigned int delay_value);
unsigned char verify_apps(void);
unsigned char is_key_pressed(GPIO_PORT key);
unsigned char is_key_pressed_sw1();
unsigned char is_key_pressed_sw2();

uint8_t downloadNflashApps();


/******************************************************************************/


#endif /* SOURCE_APP_MAIN_MAIN_H_ */
