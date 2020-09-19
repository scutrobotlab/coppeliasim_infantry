#pragma once
/* Includes ------------------------------------------------------------------*/
#include <math.h>
#include "infantry_def.h"
#include "PID.h"
/* Exported types ------------------------------------------------------------*/
enum GimbalPIDType_Enumdef
{
	PITCH_ANGLE = 0U,
	PITCH_SPEED,
	YAW_ANGLE_ENC,
	YAW_ANGLE_IMU,
	YAW_SPEED

};

class GimbalCtrl_Classdef
{
public:
	GimbalCtrl_Classdef();
	~GimbalCtrl_Classdef();
	void initPID(GimbalPIDType_Enumdef pid_type, float kp, float ki, float kd, float i_max, float out_max);
	void Change_YawTarget(int sys_time);
	void Change_YawSpeedTarget(int sys_time);
	void singleStepControl(Mode_Enumdef mode, float yaw_delta = 0, float pitch_delta = 0);
	void resetGimbal(void);
	myPID pitchAngle, pitchSpeed, yawAngleENC, chassisYawAngle, yawSpeed, yawAngleIMU;
	void writeMotorData(float, float, float, float);
	void writeIMUData(float);
	void writeCtrlData(float, float);
	float yawOut() const { return this->yawOutData; }
	float pitchOut() const { return this->pitchOutData; }

private:
	float axisSpeed[2]{};
	float yawOutData = 0;
	float pitchOutData = 0;
	float yawIMUAccu = 0;
	float yawScale, pitchScale;

};
/************************ COPYRIGHT(C) SCUT-ROBOTLAB **************************/
