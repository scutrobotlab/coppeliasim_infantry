#pragma once

#define GIMBAL_DIMENSION 2

enum Mode_Enumdef
{
	NORMAL = 0U,
	RUNE,
	ASSIST,
	ROTATE,
	STATIC,
	TUNE_PID,
	RUNAWAY = 99
};

enum Chassis_Enumdef
{
	X = 0U,
	Y,
	Z
};

enum Gimbal_Enumdef
{
	YAW = 0U,
	PITCH
};