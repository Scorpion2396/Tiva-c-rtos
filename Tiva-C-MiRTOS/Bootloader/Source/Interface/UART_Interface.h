/*
 * SPI_Interface.h
 *
 *  Created on: 18-Jul-2021
 *      Author: saksham
 */

/******************************************************************************************************************************/

#ifndef UART_INTERFACE_H
#define UART_INTERFACE_H

#include "Common_Datatype.h"

extern void UART_print(uint8_t *data);
extern void UART_init(UART_Channel UART_Ch_Index, uint32_t UART_BaudRate);
extern uint8_t UART_Receive();
extern uint32_t UART_Receive_Long();
extern void UART_Transmit(uint8_t data);

#endif /* DRIVER_SPI_SPI_H_ */