#include "Motor_setting.h"
#include "gpio.h"

#define STEP_SM1_X(state) HAL_GPIO_WritePin(STEP_SM1_X_GPIO_Port,STEP_SM1_X_Pin,(GPIO_PinState)state)
#define STEP_SM2_X(state) HAL_GPIO_WritePin(STEP_SM2_X_GPIO_Port,STEP_SM2_X_Pin,(GPIO_PinState)state)
#define STEP_SM3_X(state) HAL_GPIO_WritePin(STEP_SM3_X_GPIO_Port,STEP_SM3_X_Pin,(GPIO_PinState)state)

#define STEP_SM1_Y(state) HAL_GPIO_WritePin(STEP_SM1_Y_GPIO_Port,STEP_SM1_Y_Pin,(GPIO_PinState)state)
#define STEP_SM2_Y(state) HAL_GPIO_WritePin(STEP_SM2_Y_GPIO_Port,STEP_SM2_Y_Pin,(GPIO_PinState)state)
#define STEP_SM3_Y(state) HAL_GPIO_WritePin(STEP_SM3_Y_GPIO_Port,STEP_SM3_Y_Pin,(GPIO_PinState)state)

#define STEP_NUM_X 0
#define STEP_NUM_Y 1
#define STEP_NUM_Z 2

static void Motor_set(uint8_t num,uint8_t mode);

//  @ fuction:  
//  @ description:  设置电机模块的分频
//  @ input:
//  @ output:
//  @ note: mode暂时不用
static void Motor_set(uint8_t num,uint8_t mode){
  switch(num){
	  case STEP_NUM_X:
			switch(mode){
			  case 0:
				case 1:
				default:
						STEP_SM1_X(1);
		        STEP_SM2_X(1);
		        STEP_SM3_X(1);
					break;
			}
			break;
		case STEP_NUM_Y:
			switch(mode){
			  case 0:
				case 1:
				default:
					 STEP_SM1_Y(1);
		       STEP_SM2_Y(1);
		       STEP_SM3_Y(1);
					break;
			}
			break;
		default:
			break;
	}

}

//  @ fuction:  
//  @ description: 设置电机模块的分频 初始化
//  @ input:
//  @ output:
//  @ note: 
void Motor_Setting_init(void){
  Motor_set(STEP_NUM_X,16);//X 16分频
	Motor_set(STEP_NUM_Y,16);//Y 16分频
}
