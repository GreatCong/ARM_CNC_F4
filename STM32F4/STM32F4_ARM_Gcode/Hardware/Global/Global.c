/**
 *************************************************************************************
 * @file 
 * @author
 * @version
 * @date 
 * @brief 中断处理回调
 *************************************************************************************
*/
#include "stm32f4xx_hal.h"

#include "HW_USART.h"
#include "FIFO_USART.h"
#include "Limits.h"
#include "Stepper.h"
#include "System.h"
#include "Settings.h"
#include "Config.h"
#include "MotionControl.h"
#include "HW_TIM.h"

#include "print.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
volatile uint8_t DebounceCounterControl = 0;
volatile uint8_t DebounceCounterLimits = 0;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

extern void Limit_PinChangeISR(void);
extern void System_PinChangeISR(void);

extern uint8_t Uart_data_rsv;



//  @ fuction:  
//  @ description: 串口接收回调 
//  @ input:
//  @ output:
//  @ note: 
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	
	if(huart == &STDOUT){
			/* Read one byte from the receive data register */
//		unsigned char c = (USART_ReceiveData(USART2) & 0xFF);
    unsigned char c  = Uart_data_rsv;
//		printf("%c",Uart_data_rsv);//回显
		// Pick off realtime command characters directly from the serial stream. These characters are
		// not passed into the main buffer, but these set system state flag bits for realtime execution.
		switch(c)
		{
		case CMD_RESET:         MC_Reset(); break; // Call motion control reset routine.
		case CMD_STATUS_REPORT: System_SetExecStateFlag(EXEC_STATUS_REPORT);break;
		case CMD_CYCLE_START:   System_SetExecStateFlag(EXEC_CYCLE_START); break; // Set as true
		case CMD_FEED_HOLD:     System_SetExecStateFlag(EXEC_FEED_HOLD); break; // Set as true
		default:
			if(c > 0x7F) { // Real-time control characters are extended ACSII only.
				switch(c)
				{
				case CMD_SAFETY_DOOR: System_SetExecStateFlag(EXEC_SAFETY_DOOR); break; // Set as true
				case CMD_JOG_CANCEL:
					if(sys.state & STATE_JOG) { // Block all other states from invoking motion cancel.
						System_SetExecStateFlag(EXEC_MOTION_CANCEL);
					}
					break;

				case CMD_FEED_OVR_RESET: System_SetExecMotionOverrideFlag(EXEC_FEED_OVR_RESET); break;
				case CMD_FEED_OVR_COARSE_PLUS: System_SetExecMotionOverrideFlag(EXEC_FEED_OVR_COARSE_PLUS); break;
				case CMD_FEED_OVR_COARSE_MINUS: System_SetExecMotionOverrideFlag(EXEC_FEED_OVR_COARSE_MINUS); break;
				case CMD_FEED_OVR_FINE_PLUS: System_SetExecMotionOverrideFlag(EXEC_FEED_OVR_FINE_PLUS); break;
				case CMD_FEED_OVR_FINE_MINUS: System_SetExecMotionOverrideFlag(EXEC_FEED_OVR_FINE_MINUS); break;
				case CMD_RAPID_OVR_RESET: System_SetExecMotionOverrideFlag(EXEC_RAPID_OVR_RESET); break;
				case CMD_RAPID_OVR_MEDIUM: System_SetExecMotionOverrideFlag(EXEC_RAPID_OVR_MEDIUM); break;
				case CMD_RAPID_OVR_LOW: System_SetExecMotionOverrideFlag(EXEC_RAPID_OVR_LOW); break;
				case CMD_SPINDLE_OVR_RESET: System_SetExecAccessoryOverrideFlag(EXEC_SPINDLE_OVR_RESET); break;
				case CMD_SPINDLE_OVR_COARSE_PLUS: System_SetExecAccessoryOverrideFlag(EXEC_SPINDLE_OVR_COARSE_PLUS); break;
				case CMD_SPINDLE_OVR_COARSE_MINUS: System_SetExecAccessoryOverrideFlag(EXEC_SPINDLE_OVR_COARSE_MINUS); break;
				case CMD_SPINDLE_OVR_FINE_PLUS: System_SetExecAccessoryOverrideFlag(EXEC_SPINDLE_OVR_FINE_PLUS); break;
				case CMD_SPINDLE_OVR_FINE_MINUS: System_SetExecAccessoryOverrideFlag(EXEC_SPINDLE_OVR_FINE_MINUS); break;
				case CMD_SPINDLE_OVR_STOP: System_SetExecAccessoryOverrideFlag(EXEC_SPINDLE_OVR_STOP); break;
				case CMD_COOLANT_FLOOD_OVR_TOGGLE: System_SetExecAccessoryOverrideFlag(EXEC_COOLANT_FLOOD_OVR_TOGGLE); break;
#ifdef ENABLE_M7
				case CMD_COOLANT_MIST_OVR_TOGGLE: System_SetExecAccessoryOverrideFlag(EXEC_COOLANT_MIST_OVR_TOGGLE); break;
#endif
				}
			// Throw away any unfound extended-ASCII character by not passing it to the serial buffer.
			}
			else {
				// Write character to buffer
				FifoUsart_Insert(STDOUT_NUM, USART_DIR_RX, c);
			}
		}
		
				 HAL_UART_Receive_IT(&STDOUT,&Uart_data_rsv,1);//重新开启串口中断
	}
			
}

//  @ fuction:  
//  @ description: 串口发送回调  
//  @ input:
//  @ output:
//  @ note: 
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){
	
//	if(huart == &huart1){
//	char c;

//		if(FifoUsart_Get(USART2_NUM, USART_DIR_TX, &c) == 0) {
//			/* Write one byte to the transmit data register */
//			while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
//			USART_SendData(USART2, c);
//		}
//		else {
//			// Nothing to transmit - disable interrupt
//			USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
//		}
//		
//	}	

}

//  @ fuction:  
//  @ description: 滴答定时器回调 
//  @ input:
//  @ output:
//  @ note: 
void HAL_SYSTICK_Callback(void){
   /*
	 * Because of the board layout, we cant attach all pins to interrupts.
	 * Therefore we just poll them in this 1ms task, which is hopefully fast
	 * enough for critical events. Debouncing pins is also implemented here.
	 */
	uint8_t limits = Limits_GetState();
	if(limits) {
		// X-Y-Z Limit
		if((DebounceCounterLimits == 0) && (settings.system_flags & BITFLAG_ENABLE_LIMITS)) {
			//Delay_ms(5);
//			if(Limits_GetState()){
			DebounceCounterLimits = 20;
			Limit_PinChangeISR();			
			//Printf_MSG("[limit = %d]\r\n",Limits_GetState());//[]表示debug
//			}

		}
				
	}

	uint8_t controls = System_GetControlState();
	if(controls) {
		// System control
		if(DebounceCounterControl == 0) {
			DebounceCounterControl = 20;
			System_PinChangeISR();
		}
	}

	if(DebounceCounterLimits && !limits) {
		DebounceCounterLimits--;
	}
	if(DebounceCounterControl && !controls) {
		DebounceCounterControl--;
	}
}

//  @ fuction:  
//  @ description: TIM4 IRQ 
//  @ input:
//  @ output:
//  @ note: 这里没有采用HAL库的回调
void TIM4_IRQHandler(void){
	/* Capture compare 1 event */
	if(__HAL_TIM_GET_FLAG(&htim4, TIM_FLAG_CC1) != RESET)
  {
    if(__HAL_TIM_GET_IT_SOURCE(&htim4, TIM_IT_CC1) !=RESET){ //TIM3 CH1        
		    Stepper_MainISR();// OC			
        __HAL_TIM_CLEAR_IT(&htim4, TIM_IT_CC1);
    }
  }
	/* TIM Update event */
	if(__HAL_TIM_GET_FLAG(&htim4, TIM_FLAG_UPDATE) != RESET){
		if(__HAL_TIM_GET_IT_SOURCE(&htim4, TIM_IT_UPDATE) !=RESET){ //定时器溢出中断				
				Stepper_PortResetISR();// OVF				
				__HAL_TIM_CLEAR_IT(&htim4, TIM_IT_UPDATE);				
		}
	}
	
}

//void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim){
//	if(htim == &htim3){
//	    Stepper_MainISR();
//	}
//}

//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
//	if(htim == &htim3){
//    Stepper_PortResetISR();
//	}
//}

