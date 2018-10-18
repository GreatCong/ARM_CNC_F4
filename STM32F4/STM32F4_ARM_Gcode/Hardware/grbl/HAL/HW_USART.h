/*
  USART.h - USART Header
  Part of STM32F4_HAL

  Copyright (c)	2017 Patrick F.

  STM32F4_HAL is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  STM32F4_HAL is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  You should have received a copy of the GNU General Public License
  along with STM32F4_HAL.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef _HW_USART_H_INCLUDED_
#define _HW_USART_H_INCLUDED_

#include <stdbool.h>
//#include "stm32f4xx_usart.h"
#include "usart.h"

// Number of USARTs on this device
#define USART_NUM			3

// Numerate available USARTs in ascending order
#define USART1_NUM			0
#define USART2_NUM			1
#define USART6_NUM			2

// Usart used for Printf(...)
#define STDOUT				huart1
#define STDOUT_NUM			USART1_NUM

// Direction definitions
#define USART_DIR_RX		0
#define USART_DIR_TX		1


#ifdef __cplusplus
extern "C" {
#endif


void Usart_Init(UART_HandleTypeDef *husart, uint32_t baud);

void Usart_Put(UART_HandleTypeDef *husart, char c);
void Usart_Write(UART_HandleTypeDef *husart, uint8_t *data, uint8_t len);

void Usart_TxInt(UART_HandleTypeDef *husart, bool enable);
void Usart_RxInt(UART_HandleTypeDef *husart, bool enable);


#ifdef __cplusplus
}
#endif


#endif /* USART_H_INCLUDED */
