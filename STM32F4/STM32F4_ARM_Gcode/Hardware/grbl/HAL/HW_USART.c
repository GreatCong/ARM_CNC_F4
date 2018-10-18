/*
  USART.c - USART implementation
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
#include <stdio.h>
#include "HW_USART.h"
#include "FIFO_USART.h"


static uint8_t FifoInit = 0;
uint8_t Uart_data_rsv = 0;//串口接收的数据

//  @ fuction:  
//  @ description:串口初始化  
//  @ input:
//  @ output:
//  @ note: 
void Usart_Init(UART_HandleTypeDef *husart, uint32_t baud)
{

	if(!FifoInit) {
		// Initialize fifo once
		FifoUsart_Init();
		FifoInit = 1;
	}
	
	HAL_UART_Receive_IT(husart,&Uart_data_rsv,1);//重新开启串口中断
	
}
//  @ fuction:  
//  @ description:  UART putc
//  @ input:
//  @ output:
//  @ note: 
void Usart_Put(UART_HandleTypeDef *husart, char c)
{
	if(husart == &STDOUT)
		FifoUsart_Insert(STDOUT_NUM, USART_DIR_TX, c);

	// Enable sending via interrupt
	Usart_TxInt(husart, true);
}

//  @ fuction:  
//  @ description:  UART put string
//  @ input:
//  @ output:
//  @ note: 
void Usart_Write(UART_HandleTypeDef *husart, uint8_t *data, uint8_t len)
{
	uint8_t i = 0;

	while(len--)
	{
		if(husart == &STDOUT)
			FifoUsart_Insert(STDOUT_NUM, USART_DIR_TX, data[i++]);
	}

	// Enable sending via interrupt
	Usart_TxInt(husart, true);
}


void Usart_TxInt(UART_HandleTypeDef *husart, bool enable)
{
//	if(enable)
//	{
//		USART_ITConfig(usart, USART_IT_TXE, ENABLE);
//	}
//	else
//	{
//		USART_ITConfig(usart, USART_IT_TXE, DISABLE);
//	}
}

void Usart_RxInt(UART_HandleTypeDef *husart, bool enable)
{
//	if(enable)
//	{
//		USART_ITConfig(usart, USART_IT_RXNE, ENABLE);
//	}
//	else
//	{
//		USART_ITConfig(usart, USART_IT_RXNE, DISABLE);
//	}
}



