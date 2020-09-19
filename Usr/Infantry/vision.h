#pragma once
/* Includes ------------------------------------------------------------------*/
#include <string>
#include "FreeRTOS.h"
#include "task.h"
#include "infantry_def.h"
#include "PID.h"
/* Exported types ------------------------------------------------------------*/
#pragma pack(push)
#pragma pack(1)
struct PC_RX_DataPack_Typedef
{
	uint8_t SOF;							/* Start of Frame, 0x33 */
	uint8_t ctrlMode;						/* Required control mode */
	uint8_t fireCmd;						/* Firing command */
	uint16_t friRPM;						/* Friction wheel speed */
	float bulletSpeed;						/* Bullet velocity calculation */
	float pitchDelta;						/* Pitch axis control amount */
	float yawDelta;							/* Yaw axis control amount */
};

struct PC_TX_DataPack_Typedef
{
	uint8_t SOF;							/* Start of Frame, 0x44 */
	uint8_t robotColor;						/* Red is zero and blue is one */
	uint8_t ctrlMode;						/* Current control mode */
	float pitchAngle;						/* Pitch axis angle */
	float yawAngle;							/* Yaw axis angle */
	float pitchSpeed;						/* Pitch axis angular velocity */
	float yawSpeed;							/* Yaw axis angular velocity */
	uint8_t END;							/* End of Frame, 0x55 */
};
#pragma pack(pop)

enum PC_LinkageStatus_Enumdef
{
	PC_OFFLINE = 0,
	PC_ONLINE = 1
};

class Vision_Classdef
{
public:
	Vision_Classdef();
	void dataCapture(PC_RX_DataPack_Typedef* pc_data);
	void checkLink(TickType_t current_check_time);
	void dataTX(uint8_t red_or_blue, Mode_Enumdef mode, float yaw_angle, float pitch_angle, float pitch_speed, float yaw_speed);
	myPID yawPID, pitchPID;
	PC_LinkageStatus_Enumdef pcStatus;
	PC_RX_DataPack_Typedef pcRxPack;
	PC_TX_DataPack_Typedef pcTxPack;

private:
	TickType_t lastCheckTime;
};
/************************ COPYRIGHT(C) SCUT-ROBOTLAB **************************/
