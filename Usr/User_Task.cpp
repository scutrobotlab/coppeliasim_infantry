/**
******************************************************************************
* @file   User_Task.cpp
* @brief  User task running file. Please add your task in this file, and create
*         it in "System_Config.cpp"
******************************************************************************
* @note
*  - Before running your task, just do what you want ~
*  - More devices or using other classification is decided by yourself ~
===============================================================================
								Task List
===============================================================================
* <table>
* <tr><th>Task Name     <th>Priority          <th>Frequency/Hz    <th>Stack/Byte
* <tr><td>              <td>                  <td>                <td>
* </table>
*
*/
/* Includes ------------------------------------------------------------------*/
#include "User_Task.h"
#include <conio.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include "simulation.h"
#include "win32_support.h"
/* Private define ------------------------------------------------------------*/
TaskHandle_t Infantry_Handle;
TaskHandle_t Referee_Handle;
TaskHandle_t Transmit_Handle;
TaskHandle_t VisionRecv_Handle;
/* Private variables ---------------------------------------------------------*/
bool gIgnoreMouseInputFlag = false; // P键启停鼠标控制云台
/* Private function declarations ---------------------------------------------*/
void Task_Infantry(void* arg); // 步兵云台、底盘解算控制
void Task_Referee(void* arg);  // 神符模块运动、打击检测
void Task_Transmit(void* arg); // vrep和vs进行数据交互
void Recv_Vision(void* arg);   // 解算视觉发送过来的数据包
/* Exported devices ----------------------------------------------------------*/
/* Function prototypes -------------------------------------------------------*/
/**
  * @brief  Initialization of Tasks
  * @param  None.
  * @return None.
  */
void User_Tasks_Init(void)
{
	xTaskCreate(Task_Infantry, "Infantry",    Huge_Stack_Size,   NULL, PrioritySuperHigh, &Infantry_Handle);
	xTaskCreate(Task_Referee,  "Referee",	  Huge_Stack_Size,   NULL, PrioritySuperHigh, &Referee_Handle);
	xTaskCreate(Task_Transmit, "Transmit",    Normal_Stack_Size, NULL, PrioritySuperHigh, &Transmit_Handle);
	xTaskCreate(Recv_Vision,   "Recv.Vision", Normal_Stack_Size, NULL, PrioritySuperHigh, &VisionRecv_Handle);
}

void Task_Infantry(void* arg)
{
	/* Cache for Task */
	TickType_t xLastWakeTime_t;
	const TickType_t xBlockTime = pdMS_TO_TICKS(5);
	/* Pre-Load for task */
	xLastWakeTime_t = xTaskGetTickCount();
	(void)arg;
	/* Initialization part */
	Sleep(2000);
	/* Infinite loop */
	for (;;)
	{
		static Mode_Enumdef mode;
		/* Keymouse input */
		Check_Key();
		Check_Mouse();
		Convert_To_DR16Pack();
		dr16.DataCapture(&g_DR16Pack);

		/* Chassis kinematic synthesis */
		static float vel_mixer[4];
		dr16.IsKeyPress(_W) ? (vel_mixer[0] < 9000 / 19 / 9.55 ? vel_mixer[0] += 5 : 0) : vel_mixer[0] = 0;/* ��λ��rad/s,1 rad = 57deg/s */
		dr16.IsKeyPress(_S) ? (vel_mixer[1] < 9000 / 19 / 9.55 ? vel_mixer[1] += 5 : 0) : vel_mixer[1] = 0;
		dr16.IsKeyPress(_A) ? (vel_mixer[2] < 9000 / 19 / 9.55 ? vel_mixer[2] += 5 : 0) : vel_mixer[2] = 0;
		dr16.IsKeyPress(_D) ? (vel_mixer[3] < 9000 / 19 / 9.55 ? vel_mixer[3] += 5 : 0) : vel_mixer[3] = 0;
		chassis.writeCtrlData(vel_mixer[3] - vel_mixer[2], vel_mixer[0] - vel_mixer[1]);
		/* Kinematics resolving */
		chassis.singleStepControl(mode);
		vision.yawPID.Current = -vision.pcRxPack.yawDelta;
		vision.yawPID.Adjust();
		vision.pitchPID.Current = -vision.pcRxPack.pitchDelta;
		vision.pitchPID.Adjust();
		/* Gimbal motion synthesis & Shooting controlling **/
		if (vision.pcStatus)
		{
			mode = (Mode_Enumdef)vision.pcRxPack.ctrlMode;
			gimbal.singleStepControl(mode, vision.yawPID.Out, vision.pitchPID.Out);
			FireCmd->target = vision.pcRxPack.fireCmd ? 1 : 0;
			BulletSpeed->target = vision.pcRxPack.bulletSpeed;
		}
		else
		{
			mode = NORMAL;
			gimbal.singleStepControl(mode);
			if (gIgnoreMouseInputFlag == false)
			{
				int temp = dr16.Get_MouseX_Norm();
				gimbal.writeCtrlData(dr16.Get_MouseX_Norm(), -dr16.Get_MouseY_Norm());
				FireCmd->target = dr16.GetPress_L() ? 1 : 0;
				BulletSpeed->target = 30;
			}
			else
			{
				gimbal.writeCtrlData(0, 0);
				FireCmd->target = BulletSpeed->target = 0;
			}
		}
		/* Send Vision data */
		vision.dataTX(0, mode, imuYawVel->data, imuPitchAngle->data, Gimbal[PITCH]->obj_Data.angVelocity_f, Gimbal[YAW]->obj_Data.angVelocity_f);
		SerialPort_Vision.BuffSend((uint8_t*)&vision.pcTxPack, sizeof(vision.pcTxPack));
		/* Pass control to the next task */
		vTaskDelayUntil(&xLastWakeTime_t, xBlockTime);
	}
}

void Task_Referee(void* arg)
{
	/* Cache for Task */
	TickType_t xLastWakeTime_t;
	const TickType_t xBlockTime = pdMS_TO_TICKS(5);
	/* Pre-Load for task */
	xLastWakeTime_t = xTaskGetTickCount();
	(void)arg;
	/* Infinite loop */
	for (;;)
	{
		static int armor_state[RUNE_ARMOR_NUM]{};
		static int light_state[RUNE_ARMOR_NUM]{};

		/* Input */
		for (int i = 0; i < RUNE_ARMOR_NUM; i++)
			if (Collision[i]->collision_state > 1)
				break;
			else
				armor_state[i] = Collision[i]->collision_state;

		/* Process */
		rune.scan(armor_state);
		rune.light(light_state);

		/* Ouput */
		for (int i = 0; i < RUNE_ARMOR_NUM; i++)
			RuneLight[i]->target = light_state[i];
		RuneMotorRed->obj_Target.angVelocity_f = rune.motor();

		/* Pass control to the next task */
		vTaskDelayUntil(&xLastWakeTime_t, xBlockTime);
	}
}


/**
* @brief  Transmit data form Infanty to V-REP
* @param  None.
* @return None.
*/
void Task_Transmit(void* arg)
{
	/* Cache for Task */
	const TickType_t xBlockTime = pdMS_TO_TICKS(5);
	TickType_t xLastWakeTime_t;
	/* Pre-Load for task */
	xLastWakeTime_t = xTaskGetTickCount();
	(void)arg;
	static int width = GetSystemMetrics(SM_CXFULLSCREEN);
	static int height = GetSystemMetrics(SM_CYFULLSCREEN);
	/* Infinite loop */
	for (;;)
	{
		/* Input */
		chassis.writeMotorData(Gimbal[YAW]->obj_Data.angle_f * 360 / (2 * 3.14));  //debug
		gimbal.writeMotorData(Gimbal[YAW]->obj_Data.angle_f, Gimbal[PITCH]->obj_Data.angle_f, imuYawVel->data, imuPitchVel->data);
		gimbal.writeIMUData(imuYawAngle->data);

		/* Output */
		Wheel[LF]->obj_Target.angVelocity_f = -chassis.wheelSpeed[LF];
		Wheel[RF]->obj_Target.angVelocity_f = chassis.wheelSpeed[RF];
		Wheel[RB]->obj_Target.angVelocity_f = chassis.wheelSpeed[RB];
		Wheel[LB]->obj_Target.angVelocity_f = -chassis.wheelSpeed[LB];

		// 防止程序启动时通信线程发了几个未初始化的数据
		if (fabs(gimbal.pitchOut()) <= 2)
		{
			Gimbal[PITCH]->obj_Target.angVelocity_f = (gimbal.pitchOut() > 0) ? 10000 : -10000;
			Gimbal[PITCH]->obj_Target.torque_f = fabs(gimbal.pitchOut());
		}
		if (fabs(gimbal.yawOut()) <= 2)
		{
			Gimbal[YAW]->obj_Target.angVelocity_f = (gimbal.yawOut() > 0) ? 10000 : -10000;
			Gimbal[YAW]->obj_Target.torque_f = fabs(gimbal.yawOut());
		}

		// 检测启停鼠标控制云台
		if (Check_OneKey('P'))
		{
			gIgnoreMouseInputFlag = !gIgnoreMouseInputFlag;
			Sleep(1000);
		}
		if (gIgnoreMouseInputFlag == false)
			SetCursorPos(width / 2, height / 2);

		/* Pass control to the next task */
		vTaskDelayUntil(&xLastWakeTime_t, xBlockTime);
	}
}

void Recv_Vision(void* arg)
{
	/* Pre-Load for task */
	static PC_RX_DataPack_Typedef* pc_pack;
	static TickType_t xLastWakeTime_t = xTaskGetTickCount();
	/* Infinite loop */
	for (;;)
	{
		/* Communicate with PC */
		if (xTaskNotifyWait(0x00000000, 0xFFFFFFFF, (uint32_t*)&pc_pack, 0) == pdTRUE)
		{
			vision.dataCapture(pc_pack);
		}
		/* Pass control to the next task */
		vTaskDelayUntil(&xLastWakeTime_t, 10);
	}
}



/*---------------------------------------------- USART Manager --------------------------------------------*/
/*
  Vision
*/
int VisionSerial_RxCpltCallback(unsigned char* Recv_Data, unsigned int ReceiveLen)
{
	static BaseType_t* pxHigherPriorityTaskWoken;
	if (VisionRecv_Handle != NULL && ReceiveLen >= sizeof(PC_RX_DataPack_Typedef))
	{
		xTaskNotifyFromISR(VisionRecv_Handle, (uint32_t)Recv_Data, eSetValueWithOverwrite, pxHigherPriorityTaskWoken);
	}
	return 0;
}

/************************ COPYRIGHT(C) SCUT-ROBOTLAB **************************/
