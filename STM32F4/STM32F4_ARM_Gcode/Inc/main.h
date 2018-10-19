/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H__
#define __MAIN_H__

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define ETHERCAT_CS_Pin GPIO_PIN_2
#define ETHERCAT_CS_GPIO_Port GPIOE
#define ETHERCAT_IRQ_Pin GPIO_PIN_3
#define ETHERCAT_IRQ_GPIO_Port GPIOE
#define SYNC0_Pin GPIO_PIN_1
#define SYNC0_GPIO_Port GPIOC
#define SYNC1_Pin GPIO_PIN_2
#define SYNC1_GPIO_Port GPIOC
#define GPIO_SPINDLE_ENA_Pin GPIO_PIN_0
#define GPIO_SPINDLE_ENA_GPIO_Port GPIOA
#define GPIO_SPINDLE_DIR_Pin GPIO_PIN_1
#define GPIO_SPINDLE_DIR_GPIO_Port GPIOA
#define GPIO_SPINDLE_PWM_Pin GPIO_PIN_2
#define GPIO_SPINDLE_PWM_GPIO_Port GPIOA
#define GPIO_COOL_MIST_Pin GPIO_PIN_4
#define GPIO_COOL_MIST_GPIO_Port GPIOA
#define GPIO_COOL_FLOOD_Pin GPIO_PIN_5
#define GPIO_COOL_FLOOD_GPIO_Port GPIOA
#define GPIO_PROBE_Pin GPIO_PIN_6
#define GPIO_PROBE_GPIO_Port GPIOA
#define GPIO_ENABLE_Pin GPIO_PIN_7
#define GPIO_ENABLE_GPIO_Port GPIOA
#define GPIO_DIR_X_Pin GPIO_PIN_7
#define GPIO_DIR_X_GPIO_Port GPIOE
#define GPIO_STEP_X_Pin GPIO_PIN_8
#define GPIO_STEP_X_GPIO_Port GPIOE
#define GPIO_DIR_Y_Pin GPIO_PIN_9
#define GPIO_DIR_Y_GPIO_Port GPIOE
#define GPIO_STEP_Y_Pin GPIO_PIN_10
#define GPIO_STEP_Y_GPIO_Port GPIOE
#define GPIO_DIR_Z_Pin GPIO_PIN_11
#define GPIO_DIR_Z_GPIO_Port GPIOE
#define GPIO_STEP_Z_Pin GPIO_PIN_12
#define GPIO_STEP_Z_GPIO_Port GPIOE
#define GPIO_LIM_X_Pin GPIO_PIN_13
#define GPIO_LIM_X_GPIO_Port GPIOE
#define GPIO_LIM_Y_Pin GPIO_PIN_14
#define GPIO_LIM_Y_GPIO_Port GPIOE
#define GPIO_LIM_Z_Pin GPIO_PIN_15
#define GPIO_LIM_Z_GPIO_Port GPIOE
#define LED_BOOT_Pin GPIO_PIN_12
#define LED_BOOT_GPIO_Port GPIOB
#define LED_STATE_Pin GPIO_PIN_13
#define LED_STATE_GPIO_Port GPIOB
#define GPIO_CTRL_RST_Pin GPIO_PIN_1
#define GPIO_CTRL_RST_GPIO_Port GPIOD
#define GPIO_CTRL_START_Pin GPIO_PIN_2
#define GPIO_CTRL_START_GPIO_Port GPIOD
#define GPIO_CTRL_FEED_Pin GPIO_PIN_3
#define GPIO_CTRL_FEED_GPIO_Port GPIOD
#define GPIO_DOOR_Pin GPIO_PIN_4
#define GPIO_DOOR_GPIO_Port GPIOD
#define ETHERCAT_SPI_SCK_Pin GPIO_PIN_3
#define ETHERCAT_SPI_SCK_GPIO_Port GPIOB
#define ETHERCAT_SPI_MISO_Pin GPIO_PIN_4
#define ETHERCAT_SPI_MISO_GPIO_Port GPIOB
#define ETHERCAT_SPI_MOSI_Pin GPIO_PIN_5
#define ETHERCAT_SPI_MOSI_GPIO_Port GPIOB

/* ########################## Assert Selection ############################## */
/**
  * @brief Uncomment the line below to expanse the "assert_param" macro in the 
  *        HAL drivers code
  */
/* #define USE_FULL_ASSERT    1U */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
 extern "C" {
#endif
void _Error_Handler(char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)
#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
