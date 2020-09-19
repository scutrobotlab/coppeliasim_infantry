/* Includes ------------------------------------------------------------------*/
#include "chassis_ctrl.h"

/* Function prototypes -------------------------------------------------------*/
/**
	* @brief Constructor
	*/
ChassisCtrl_Classdef::ChassisCtrl_Classdef() :xScale(1.f), yScale(1.f) {}

/**
	* @brief Destructor
	*/
ChassisCtrl_Classdef::~ChassisCtrl_Classdef() {}

/**
	* @brief  Write PID parameters
	* @param	kp, ki, kd, i_max, out_max: Refer to PID.h
	* @retval None
	*/
void ChassisCtrl_Classdef::initPID(float kp, float ki, float kd, float i_max, float out_max)
{
	chassisYawAngle.SetPIDParam(kp, ki, kd, i_max, out_max);
}

/**
	* @brief  Single step follow-up kinematic solution to the chassis
	* @param	mode: Current control mode
	* @retval None
	*/
void ChassisCtrl_Classdef::singleStepControl(Mode_Enumdef mode)
{
	float speed_raw[3] = {};
	float speed_calc[3] = {};
	switch (mode)
	{
	case NORMAL:

	case ASSIST:
		speed_raw[X] = axisSpeed[X] * xScale;
		speed_raw[Y] = axisSpeed[Y] * yScale;
		chassisYawAngle.Current = yawMotorAng;
		chassisYawAngle.Target = 0;
		chassisYawAngle.Adjust();
		speed_raw[Z] = chassisYawAngle.Out;
		break;
	case ROTATE:
		speed_raw[X] = speed_raw[Y] = 0;
		speed_raw[Z] = 80;
		break;
	case RUNE:
	case STATIC:
	case RUNAWAY:
		speed_raw[X] = speed_raw[Y] = speed_raw[Z] = 0;
		break;
	case TUNE_PID:
		speed_raw[X] = axisSpeed[X] * xScale;
		speed_raw[Y] = axisSpeed[Y] * yScale;
		//chassisYawAngle.Current = fmod(yawMotorAng, 2 * 3.1415926);
		chassisYawAngle.Current = yawMotorAng;
		chassisYawAngle.Target = 0;
		chassisYawAngle.Adjust();
		speed_raw[Z] = chassisYawAngle.Out;
	}
	yawMotorAng = 0;
	speed_calc[X] = speed_raw[X] * cos(yawMotorAng * 3.1415926 / 180.0) + speed_raw[Y] * sin(yawMotorAng * 3.1415926 / 180.0);
	speed_calc[Y] = speed_raw[Y] * cos(yawMotorAng * 3.1415926 / 180.0) - speed_raw[X] * sin(yawMotorAng * 3.1415926 / 180.0);
	speed_calc[Z] = speed_raw[Z];

	this->wheelSpeed[0] = -speed_calc[X] - speed_calc[Y] - speed_calc[Z];
	this->wheelSpeed[1] = -speed_calc[X] + speed_calc[Y] - speed_calc[Z];
	this->wheelSpeed[2] = +speed_calc[X] + speed_calc[Y] - speed_calc[Z];
	this->wheelSpeed[3] = +speed_calc[X] - speed_calc[Y] - speed_calc[Z];
}

/* Here is emporarily */
void ChassisCtrl_Classdef::writeCtrlData(float x, float y)
{
	this->axisSpeed[X] = x;
	this->axisSpeed[Y] = y;
}

void ChassisCtrl_Classdef::writeMotorData(float yaw_ang)
{
	this->yawMotorAng = yaw_ang;
}
/************************ COPYRIGHT(C) SCUT-ROBOTLAB **************************/
