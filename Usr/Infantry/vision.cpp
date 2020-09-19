/* Includes ------------------------------------------------------------------*/
#include "vision.h"
/* Function prototypes -------------------------------------------------------*/
Vision_Classdef::Vision_Classdef() :pcStatus(PC_OFFLINE) {}

void Vision_Classdef::dataCapture(PC_RX_DataPack_Typedef* pc_data)
{
	if (pc_data->SOF == 0x33)
	{
		lastCheckTime = 0;
		pcStatus = PC_ONLINE;
		pcRxPack = *pc_data;
	}
}

void Vision_Classdef::checkLink(TickType_t current_check_time)
{
	static TickType_t dt = 0;
	if (lastCheckTime == 0) lastCheckTime = current_check_time;
	else
	{
		dt = current_check_time - lastCheckTime;
		if (dt > 100)
		{
			pcStatus = PC_OFFLINE;
		}
		else;
	}
}

void Vision_Classdef::dataTX(uint8_t red_or_blue, Mode_Enumdef mode, float pitch_angle, float yaw_angle, float pitch_speed, float yaw_speed)
{
	pcTxPack.SOF = 0x44;
	pcTxPack.robotColor = red_or_blue;
	pcTxPack.ctrlMode = (uint8_t)mode;
	memcpy((void*)&pcTxPack.pitchAngle, &pitch_angle, sizeof(pitch_angle));
	memcpy((void*)&pcTxPack.yawAngle, &yaw_angle, sizeof(yaw_angle));
	memcpy((void*)&pcTxPack.pitchSpeed, &pitch_speed, sizeof(pitch_speed));
	memcpy((void*)&pcTxPack.yawSpeed, &yaw_speed, sizeof(yaw_speed));
	pcTxPack.END = 0x55;
	//HAL_UART_Transmit_DMA(huart, (uint8_t *)&pcTxPack, sizeof(pcTxPack));
}
/************************ COPYRIGHT(C) SCUT-ROBOTLAB **************************/
