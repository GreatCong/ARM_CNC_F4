/**
 *************************************************************************************
 * @file Arm_motion.c
 * @author lcj
 * @version 1.0
 * @date 2018-10-10
 * @brief 机械手的运动正反解
 *************************************************************************************
*/
#include "Arm_motion.h"
#include <string.h>

#include "grbl_advance.h"

#include "print.h"

//#define BIG_ARM_LENGTH 148				//	@大臂长度	148mm
//#define SMALL_ARM_LENGTH 160			//	@小臂长度	160mm
//#define HEAD_OFFSET   50				//	@头偏移		50mm    
//#define CENCER_OFFSET 32				//	@中心偏移	32mm

//#define ARM_OFFSET_X 100 //mm   		//	@X偏移	100mm
//#define ARM_OFFSET_Y -56 //mm			//	@Y偏移	-56mm
//#define ARM_OFFSET_Z -56 //mm			//	@Z偏移	-56mm

//原点的设置是三个步进电机的中心位置的交点
#define BIG_ARM_LENGTH 135				//	@大臂长度	135mm
#define SMALL_ARM_LENGTH 170			//	@小臂长度	170mm
#define HEAD_OFFSET   50				//	@头偏移		50mm    
#define CENCER_OFFSET 47				//	@中心偏移	47mm

#define ARM_HIGH_OFFSET 110 // @ 原点到水平的高度110mm

#define ARM_OFFSET_X 100 //mm   		//	@X偏移	100mm
#define ARM_OFFSET_Y -56 //mm			//	@Y偏移	-56mm
#define ARM_OFFSET_Z -56 //mm			//	@Z偏移	-56mm
#define ARM_RAD2DEG 57.2957795			//	@转角	57.2957795度

extern int32_t sys_position[N_AXIS];

char ARM_init_script[] = "G00X267.0Y0Z135.0F100";//初始位置的正解
volatile uint8_t Arm_Motion_state = 0;

#ifdef ARM
ARM_Motion_s arm_position_init;//笛卡尔坐标系为(0,0,0)的角度
#endif

#ifdef ARM
//  @ fuction:  
//  @ description:  
//  @ input:
//  @ output:
//  @ note: 余弦定理计算角度
float return_angle(float a, float b, float c) {
  // cosine rule for angle between c and a
  return acos((a * a + c * c - b * b) / (2 * a * c));
}

//  @ fuction:  
//  @ description:  
//  @ input:
//  @ output:
//  @ note: 余弦定理检查
uint8_t check_angle(float a, float b, float c){
  float data;
	data = (a * a + c * c - b * b) - (2 * a * c);
	
	if(data > 0){
	  return 1;
	}
	else{
	  return 0;
	}
}

//  @ fuction:  
//  @ description: 计算位置 
//  @ input:
//  @ output:
//  @ note: 关节角度到空间位置
ARM_Motion_s calculate_forward(const float *cartesian_theta)
{
	float x1,y1;
	float x2,y2;
	
	ARM_Motion_s arm_motion_temp;
	
	y1=sin(cartesian_theta[X_AXIS]/ARM_RAD2DEG)*BIG_ARM_LENGTH-sin(cartesian_theta[Y_AXIS]/ARM_RAD2DEG)*SMALL_ARM_LENGTH;
	
	x1=cos(cartesian_theta[X_AXIS]/ARM_RAD2DEG)*BIG_ARM_LENGTH+cos(cartesian_theta[Y_AXIS]/ARM_RAD2DEG)*SMALL_ARM_LENGTH;
	x1=x1+HEAD_OFFSET+CENCER_OFFSET;
	
	x2 = x1 *cos(cartesian_theta[Z_AXIS]/ARM_RAD2DEG);
	y2 = x1 *sin(cartesian_theta[Z_AXIS]/ARM_RAD2DEG);
	
	arm_motion_temp.arm[X_AXIS]=x2;
	arm_motion_temp.arm[Y_AXIS]=y2;
	arm_motion_temp.arm[Z_AXIS]=y1;
	
	arm_motion_temp.transfer_state = 0;//no error
	
  return arm_motion_temp;
}
  

//  @ fuction:  
//  @ description: 计算角度 
//  @ input:
//  @ output:
//  @ note: 空间位置到关节角度
ARM_Motion_s calculate_arm(const float *cartesian)
{
	float TempXY,TempXYZ;
	float TempX,TempY,TempZ;
	
  ARM_Motion_s arm_motion_temp;
	
	TempXY=sqrt(cartesian[X_AXIS]*cartesian[X_AXIS]
				+cartesian[Y_AXIS]*cartesian[Y_AXIS]);
	TempXYZ=sqrt((TempXY-CENCER_OFFSET-HEAD_OFFSET)*(TempXY-CENCER_OFFSET-HEAD_OFFSET)		
				+cartesian[Z_AXIS]*cartesian[Z_AXIS]);
	
	if(check_angle(BIG_ARM_LENGTH,SMALL_ARM_LENGTH,TempXYZ)){
	  arm_motion_temp.transfer_state = 1;//error
	}
	else{
	 	arm_motion_temp.transfer_state = 0;//no error
	}
	
	TempX = asin(cartesian[Z_AXIS]/(TempXYZ))+return_angle(BIG_ARM_LENGTH,SMALL_ARM_LENGTH,TempXYZ);
	TempY = return_angle(BIG_ARM_LENGTH,TempXYZ,SMALL_ARM_LENGTH);
	TempZ = atan(cartesian[Y_AXIS]/cartesian[X_AXIS]);
	
	
	arm_motion_temp.arm[X_AXIS] = TempX*ARM_RAD2DEG;//弧度转化为角度
	arm_motion_temp.arm[Y_AXIS] = 180-arm_motion_temp.arm[X_AXIS]-TempY*ARM_RAD2DEG;
	arm_motion_temp.arm[Z_AXIS] = TempZ*ARM_RAD2DEG;
	
	return arm_motion_temp;
	
  /*
  SERIAL_ECHOPGM("cartesian x="); SERIAL_ECHO(cartesian[X_AXIS]);
  SERIAL_ECHOPGM(" y="); SERIAL_ECHO(cartesian[Y_AXIS]);
  SERIAL_ECHOPGM(" z="); SERIAL_ECHOLN(cartesian[Z_AXIS]);

  SERIAL_ECHOPGM("delta x="); SERIAL_ECHO(delta[X_AXIS]);
  SERIAL_ECHOPGM(" y="); SERIAL_ECHO(delta[Y_AXIS]);
  SERIAL_ECHOPGM(" z="); SERIAL_ECHOLN(delta[Z_AXIS]);
  */
}

//  @ fuction:  
//  @ description: 角度复位 
//  @ input:
//  @ output:
//  @ note: 正方向 X(向后) Y(向下)
void Arm_motion_reset(void){
//	float init_cartesian[N_AXIS];
//	memset(init_cartesian, 0 , sizeof(init_cartesian));
//init_cartesian[X_AXIS] = 90;
	
//  arm_position_init = calculate_arm(init_cartesian);
//	  arm_position_init = calculate_forward(init_cartesian);
	arm_position_init.arm[X_AXIS] = 90;
	arm_position_init.arm[Y_AXIS] = 0;
	arm_position_init.arm[Z_AXIS] = 0;
	
//	//打印初始位置坐标，便于调试
//	ARM_Motion_s arm_position_debug;
//	arm_position_debug = calculate_forward(arm_position_init.arm);
//	Printf_MSG("[Arm=(%f,%f,%f)]\r\n",arm_position_debug.arm[X_AXIS],arm_position_debug.arm[Y_AXIS],arm_position_debug.arm[Z_AXIS]);
	
	
	for(int idx = 0; idx < 3; idx++) { //只用了3轴
		 sys_position[idx] = lround(arm_position_init.arm[idx]*settings.steps_per_mm[idx]);
  }
}

#endif
