#pragma once
/* Includes ------------------------------------------------------------------*/
#include <math.h>
#include "PID.h"
#include "infantry_def.h"
/* Exported types ------------------------------------------------------------*/
class ChassisCtrl_Classdef
{
public:
	ChassisCtrl_Classdef();
	~ChassisCtrl_Classdef();
	void initPID(float kp, float ki, float kd, float i_max, float out_max);
	void singleStepControl(Mode_Enumdef mode);
	int wheelSpeed[4]{};
	myPID chassisYawAngle;
	void writeCtrlData(float x, float y);
	void writeMotorData(float);

private:
	float axisSpeed[3] = { 0 };
	float yawMotorAng = 0;
	float	xScale, yScale;
};


#define WHEEL_NUM             4
enum _chassis_WheelType
{
	LF = 0U,
	RF,
	RB,
	LB
};

/************************ COPYRIGHT(C) SCUT-ROBOTLAB **************************/
