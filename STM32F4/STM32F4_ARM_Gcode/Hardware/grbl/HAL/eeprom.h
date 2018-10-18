/*
  eeprom.h - Eeprom Header
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
#ifndef EEPROM_H
#define EEPROM_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
//#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"


#define EEPROM_SIZE				1024

/* Device voltage range supposed to be [2.7V to 3.6V], the operation will be done by word  */
#define VOLTAGE_RANGE			(uint8_t)VoltageRange_3

/* EEPROM start address in Flash */
//#define EEPROM_START_ADDRESS	((uint32_t)0x08060000) /* EEPROM emulation start address (last sector): 384 Kb */

//从标准库中copy
//#define FLASH_Sector_7     ((uint16_t)0x0038) /*!< Sector Number 7   */
//#define FLASH_SECTOR			FLASH_Sector_7
//FLASH起始地址
#define STM32_FLASH_SIZE 1024
#define STM32_FLASH_BASE ((uint32_t)0x08000000) 	//STM32 FLASH的起始地址 
#define EEPROM_START_ADDRESS (uint32_t)(STM32_FLASH_BASE+1024*(STM32_FLASH_SIZE-2)) //最后一个扇区地址存储
//#define EEPROM_START_ADDRESS	((uint32_t)0x08007000) //stm32 64k rom的最后一个扇区地址


void EE_Init(void);

uint8_t EE_ReadByte(uint16_t VirtAddress);
void EE_WriteByte(uint16_t VirtAddress, uint8_t Data);

uint8_t EE_ReadByteArray(uint8_t *DataOut, uint16_t VirtAddress, uint16_t size);
void EE_WriteByteArray(uint16_t VirtAddress, uint8_t *DataIn, uint16_t size);

void EE_Program(void);
void EE_Erase(void);


#endif /* EEPROM_H */
