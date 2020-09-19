/* Includes ------------------------------------------------------------------*/
#include "gimbal_ctrl.h"
#include "System_Config.h"

/* Private function declarations ---------------------------------------------*/
template<typename Type>
void _angle_Constrain(Type* x, Type Min, Type Max)
{
	if (*x < Min) *x = Min;
	else if (*x > Max) *x = Max;
	else { ; }
}
/* Function prototypes -------------------------------------------------------*/
GimbalCtrl_Classdef::GimbalCtrl_Classdef() :yawScale(0.5f), pitchScale(0.5f) {}

GimbalCtrl_Classdef::~GimbalCtrl_Classdef() {}

void GimbalCtrl_Classdef::singleStepControl(Mode_Enumdef mode, float yaw_delta, float pitch_delta)
{
	switch (mode)
	{
	case RUNE:
	case STATIC:
		yawAngleENC.Target -= axisSpeed[YAW] * yawScale;
		yawSpeed.Target = yawAngleENC.Adjust();
		break;
	case ASSIST:
		yawAngleIMU.Target -= yaw_delta;
		pitchAngle.Target -= pitch_delta;
	case ROTATE:
	case NORMAL:
		yawAngleIMU.Target -= axisSpeed[YAW] * yawScale;
		yawAngleIMU.Current = yawIMUAccu;
		yawSpeed.Target = yawAngleIMU.Adjust();
		break;
	case RUNAWAY:
		this->yawOutData = this->pitchOutData = 0;
		break;
	}
	if (mode != RUNAWAY)
	{
		this->yawOutData = yawSpeed.Adjust();
		pitchAngle.Target -= axisSpeed[PITCH] * pitchScale;
		_angle_Constrain(&pitchAngle.Target, -0.35f, 0.35f);
		pitchSpeed.Target = pitchAngle.Adjust();
		this->pitchOutData = pitchSpeed.Adjust();
	}
}

void GimbalCtrl_Classdef::resetGimbal(void)
{
	float yaw_mod = fmod(yawAngleENC.Current, 2 * 3.1415926);
	if (yaw_mod < 3.1415926) yawAngleENC.Target = -yaw_mod;
	else yawAngleENC.Target = yawAngleENC.Current - yaw_mod + 2 * 3.1415926;
	yawAngleIMU.Target = yawIMUAccu;
	pitchAngle.Target = 0;
}

void GimbalCtrl_Classdef::initPID(GimbalPIDType_Enumdef pid_type, float kp, float ki, float kd, float i_max, float out_max)
{
	switch (pid_type)
	{
	case PITCH_ANGLE:
		pitchAngle.SetPIDParam(kp, ki, kd, i_max, out_max);
		break;
	case PITCH_SPEED:
		pitchSpeed.SetPIDParam(kp, ki, kd, i_max, out_max);
		break;
	case YAW_ANGLE_ENC:
		yawAngleENC.SetPIDParam(kp, ki, kd, i_max, out_max);
		break;
	case YAW_ANGLE_IMU:
		yawAngleIMU.SetPIDParam(kp, ki, kd, i_max, out_max);
		break;
	case YAW_SPEED:
		yawSpeed.SetPIDParam(kp, ki, kd, i_max, out_max);
		break;
	}
}

/* Here is emporarily */
void GimbalCtrl_Classdef::Change_YawTarget(int sys_time) {

	static float angle_1 = 2;
	static float angle_2 = 0;
	static int init_flag = false;
	static int last_change_time = sys_time;

	if (init_flag == false) {
		init_flag = true;
		yawAngleIMU.Target = angle_1;
	}

	if (sys_time >= last_change_time + 2000) {
		yawAngleIMU.Target == angle_1 ? yawAngleIMU.Target = angle_2 : yawAngleIMU.Target = angle_1;
		last_change_time = sys_time;
	}
}

void GimbalCtrl_Classdef::Change_YawSpeedTarget(int sys_time) {

	static float speed_1 = 20;
	static float speed_transition = 0;
	static float speed_2 = -20;
	static int state_flag = false;
	static int last_change_time = sys_time;

	if (sys_time >= last_change_time + 800) {
		switch (state_flag) {
		case 0:
			yawSpeed.Target = speed_1;
			break;
		case 1:
			yawSpeed.Target = speed_transition;
			break;
		case 2:
			yawSpeed.Target = speed_2;
			break;
		case 3:
			yawSpeed.Target = speed_transition;
			break;
		default:break;
		}
		last_change_time = sys_time;
		state_flag == 3 ? state_flag = 0 : state_flag++;
	}
}

void GimbalCtrl_Classdef::writeMotorData(float yaw_ang, float pitch_ang, float yaw_v, float pitch_v)
{
	this->yawAngleENC.Current = yaw_ang;
	this->pitchAngle.Current = pitch_ang;
	this->yawSpeed.Current = yaw_v;
	this->pitchSpeed.Current = pitch_v;
}

void GimbalCtrl_Classdef::writeIMUData(float yaw_ang)
{
	static int round_cnt;
	static float yaw_ang_;
	if (yaw_ang - yaw_ang_ > 2 * 3.1415926) round_cnt--;
	else if (yaw_ang - yaw_ang_ < -2 * 3.1415926) round_cnt++;
	yaw_ang_ = yaw_ang;
	yawIMUAccu = yaw_ang + round_cnt * 2 * 3.1415926;
}

void GimbalCtrl_Classdef::writeCtrlData(float yaw, float pitch)
{
	this->axisSpeed[YAW] = yaw;
	this->axisSpeed[PITCH] = pitch;
}
/************************ COPYRIGHT(C) SCUT-ROBOTLAB **************************/
