/**
 *************************************************************************************
 * @file Arm_motion.c
 * @author lcj
 * @version 1.0
 * @date 2018-10-10
 * @brief ��е�ֵ��˶�������
 *************************************************************************************
*/
#include "Arm_motion.h"
#include <string.h>

#include "grbl_advance.h"

#include "print.h"

//#define BIG_ARM_LENGTH 148				//	@��۳���	148mm
//#define SMALL_ARM_LENGTH 160			//	@С�۳���	160mm
//#define HEAD_OFFSET   50				//	@ͷƫ��		50mm    
//#define CENCER_OFFSET 32				//	@����ƫ��	32mm

//#define ARM_OFFSET_X 100 //mm   		//	@Xƫ��	100mm
//#define ARM_OFFSET_Y -56 //mm			//	@Yƫ��	-56mm
//#define ARM_OFFSET_Z -56 //mm			//	@Zƫ��	-56mm

//ԭ��������������������������λ�õĽ���
#define BIG_ARM_LENGTH 135				//	@��۳���	135mm
#define SMALL_ARM_LENGTH 170			//	@С�۳���	170mm
#define HEAD_OFFSET   50				//	@ͷƫ��		50mm    
#define CENCER_OFFSET 47				//	@����ƫ��	47mm

#define ARM_HIGH_OFFSET 110 // @ ԭ�㵽ˮƽ�ĸ߶�110mm

#define ARM_OFFSET_X 100 //mm   		//	@Xƫ��	100mm
#define ARM_OFFSET_Y -56 //mm			//	@Yƫ��	-56mm
#define ARM_OFFSET_Z -56 //mm			//	@Zƫ��	-56mm
#define ARM_RAD2DEG 57.2957795			//	@ת��	57.2957795��

extern int32_t sys_position[N_AXIS];

char ARM_init_script[] = "G00X267.0Y0Z135.0F100";//��ʼλ�õ�����
volatile uint8_t Arm_Motion_state = 0;

#ifdef ARM
ARM_Motion_s arm_position_init;//�ѿ�������ϵΪ(0,0,0)�ĽǶ�
#endif

#ifdef ARM
//  @ fuction:  
//  @ description:  
//  @ input:
//  @ output:
//  @ note: ���Ҷ������Ƕ�
float return_angle(float a, float b, float c) {
  // cosine rule for angle between c and a
  return acos((a * a + c * c - b * b) / (2 * a * c));
}

//  @ fuction:  
//  @ description:  
//  @ input:
//  @ output:
//  @ note: ���Ҷ�����
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
//  @ description: ����λ�� 
//  @ input:
//  @ output:
//  @ note: �ؽڽǶȵ��ռ�λ��
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
//  @ description: ����Ƕ� 
//  @ input:
//  @ output:
//  @ note: �ռ�λ�õ��ؽڽǶ�
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
	
	
	arm_motion_temp.arm[X_AXIS] = TempX*ARM_RAD2DEG;//����ת��Ϊ�Ƕ�
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
//  @ description: �Ƕȸ�λ 
//  @ input:
//  @ output:
//  @ note: ������ X(���) Y(����)
void Arm_motion_reset(void){
//	float init_cartesian[N_AXIS];
//	memset(init_cartesian, 0 , sizeof(init_cartesian));
//init_cartesian[X_AXIS] = 90;
	
//  arm_position_init = calculate_arm(init_cartesian);
//	  arm_position_init = calculate_forward(init_cartesian);
	arm_position_init.arm[X_AXIS] = 90;
	arm_position_init.arm[Y_AXIS] = 0;
	arm_position_init.arm[Z_AXIS] = 0;
	
//	//��ӡ��ʼλ�����꣬���ڵ���
//	ARM_Motion_s arm_position_debug;
//	arm_position_debug = calculate_forward(arm_position_init.arm);
//	Printf_MSG("[Arm=(%f,%f,%f)]\r\n",arm_position_debug.arm[X_AXIS],arm_position_debug.arm[Y_AXIS],arm_position_debug.arm[Z_AXIS]);
	
	
	for(int idx = 0; idx < 3; idx++) { //ֻ����3��
		 sys_position[idx] = lround(arm_position_init.arm[idx]*settings.steps_per_mm[idx]);
  }
}

#endif
