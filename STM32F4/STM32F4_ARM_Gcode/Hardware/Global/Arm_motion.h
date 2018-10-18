#ifndef __ARM_MOTION_H_
#define __ARM_MOTION_H_

#include "util.h"

#define ARM //定义机械臂
#define ARM_Init_SCRIPT //初始化G代码

#define ARM_STATE_LOCK_MOTOR         				BIT(0) // In alarm state. Locks out all g-code processes. Allows settings access.
//#define ARM_STATE_CHECK_MODE    				BIT(1) // G-code check mode. Locks out planner and motion only.
//#define ARM_STATE_HOMING        				BIT(2) // Performing homing cycle
//#define ARM_STATE_CYCLE         				BIT(3) // Cycle is running or motions are being executed.
//#define ARM_STATE_HOLD          				BIT(4) // Active feed hold
//#define ARN_STATE_JOG           				BIT(5) // Jogging mode.
//#define ARMN_STATE_SAFETY_DOOR   				BIT(6) // Safety door is ajar. Feed holds and de-energizes system.
//#define ARM_STATE_SLEEP         				BIT(7) // Sleep state.

#ifdef ARM
typedef struct{
  float arm[N_AXIS];
	uint8_t transfer_state;
}ARM_Motion_s;

ARM_Motion_s calculate_forward(const float *cartesian);
ARM_Motion_s calculate_arm(const float *cartesian);

void Arm_motion_reset(void);

extern char ARM_init_script[];
extern volatile uint8_t Arm_Motion_state;

#endif

#endif
