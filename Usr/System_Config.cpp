/**
  ******************************************************************************
  * @file   System_config.cpp
  * @brief  Deploy resources,tasks and services in this file.
  ******************************************************************************
  * @note
  *  - Before running your Task you should first include your headers and init-
  *    ialize used resources in "System_Resource_Init()". This function will be
  *    called before tasks Start.
  *
  *  - All tasks should be created in "System_Tasks_Init()".
  *  - FreeRTOS scheduler will be started after tasks are created.
  *
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
#include "System_Config.h"
#include "System_DataPool.h"

/* Service */
#include "simulation.h"
#include "win32_support.h"

/* User support package & SRML */
#include "User_Task.h"

/* Private variables ---------------------------------------------------------*/


/*Founctions------------------------------------------------------------------*/
/**
* @brief Load drivers ,modules, and data resources for tasks.
* @note  Edit this function to add Init-functions and configurations.
*/
void System_Resource_Init(void)
{
	/* Drivers Init ---------------------*/

	/* RTOS resources Init --------------*/

	/* Other resources Init -------------*/
	LogOutputBacken_Init();

	/* Modules Init ---------------------*/

	/* Service configurations -----------*/
	SerialPort_Vision.Register_RecvCallBack(VisionSerial_RxCpltCallback);
}
uint32_t getSimMicroSec(void) { return CoppeliaSim->GetSimMicroSec(); }
/**
* @brief Load and start User Tasks. This function run directly in "main()"
* @note  Edit this function to add tasks into the activated tasks list.
*/
void System_Tasks_Init(void)
{
	/* Syetem Service init --------------*/
	/*Serial Communication service*/
	Service_SerialPortCom_Init();

	/*CoppeliaSim service*/
	Service_CoppeliaSim_Init();

	/* -------- */
	// PID模块
	myPIDTimer::getMicroTick_regist(GetMicroSecond);
	gimbal.initPID(YAW_ANGLE_ENC, 1.0f, 0, 0, 0, 100.0f);
	gimbal.initPID(YAW_SPEED, 0.5f, 0, 0, 0, 1.6f);
	gimbal.initPID(YAW_ANGLE_IMU, 2.0f, 0, 0, 0, 100.0f);
	gimbal.initPID(PITCH_SPEED, 1.0f, 0.5f, 0, 1.6f, 1.6f);
	gimbal.initPID(PITCH_ANGLE, 2.0f, 0, 0, 0, 100.0f);
	gimbal.pitchSpeed.I_SeparThresh = 0.5f;
	chassis.initPID(1.0f, 0.0f, 0.0f, 100.0f, 1000.0f);
	vision.yawPID.SetPIDParam(5E-5, 0, 4E-6, 0, 1.0f);
	vision.yawPID.Target = 0;
	vision.pitchPID.SetPIDParam(8E-5, 0, 6E-6, 0, 1.0f);
	vision.pitchPID.Target = 0;

	// 步兵模块
	Infantry = CoppeliaSim->Add_Object("Infantry_1", OTHER_OBJECT, { SIM_ORIENTATION | CLIENT_RO, SIM_VELOCITY | CLIENT_RO });
	Wheel[LB] = CoppeliaSim->Add_Object("Wheel_LB", JOINT, { SIM_VELOCITY | CLIENT_RW, SIM_POSITION | CLIENT_RO });
	Wheel[LF] = CoppeliaSim->Add_Object("Wheel_LF", JOINT, { SIM_VELOCITY | CLIENT_RW, SIM_POSITION | CLIENT_RO });
	Wheel[RB] = CoppeliaSim->Add_Object("Wheel_RB", JOINT, { SIM_VELOCITY | CLIENT_RW, SIM_POSITION | CLIENT_RO });
	Wheel[RF] = CoppeliaSim->Add_Object("Wheel_RF", JOINT, { SIM_VELOCITY | CLIENT_RW, SIM_POSITION | CLIENT_RO });
	Gimbal[YAW] = CoppeliaSim->Add_Object("GimbalYaw", JOINT, { SIM_VELOCITY | CLIENT_RW, SIM_POSITION | CLIENT_RO, SIM_FORCE | CLIENT_WO });
	Gimbal[PITCH] = CoppeliaSim->Add_Object("GimbalPitch", JOINT, { SIM_VELOCITY | CLIENT_RW, SIM_POSITION | CLIENT_RO, SIM_FORCE | CLIENT_WO });
	imuYawAngle = CoppeliaSim->Add_Object("Infantry_1.YawAng", SIM_FLOAT_SIGNAL, { SIM_SIGNAL_OP | CLIENT_RO });
	imuPitchAngle = CoppeliaSim->Add_Object("Infantry_1.PitchAng", SIM_FLOAT_SIGNAL, { SIM_SIGNAL_OP | CLIENT_RO });
	imuYawVel = CoppeliaSim->Add_Object("Infantry_1.YawVel", SIM_FLOAT_SIGNAL, { SIM_SIGNAL_OP | CLIENT_RO });
	imuPitchVel = CoppeliaSim->Add_Object("Infantry_1.PitchVel", SIM_FLOAT_SIGNAL, { SIM_SIGNAL_OP | CLIENT_RO });
	FireCmd = CoppeliaSim->Add_Object("Infantry_1.FireCmd", SIM_INTEGER_SIGNAL, { SIM_SIGNAL_OP | CLIENT_WO });
	BulletSpeed = CoppeliaSim->Add_Object("Infantry_1.BulletSpeed", SIM_FLOAT_SIGNAL, { SIM_SIGNAL_OP | CLIENT_WO });

	// 神符模块
	RuneMotorRed = CoppeliaSim->Add_Object("BladeRed", JOINT, { SIM_VELOCITY | CLIENT_RW, SIM_POSITION | CLIENT_RO });
	for (unsigned int i = 0; i < RUNE_ARMOR_NUM; i++)
	{
		Collision[i] = CoppeliaSim->Add_Object("RuneCollision_" + to_string(i + 1));
		RuneLight[i] = CoppeliaSim->Add_Object("RuneLight_" + to_string(i + 1), SIM_INTEGER_SIGNAL, { SIM_SIGNAL_OP | CLIENT_WO });
	}

	/* Terrible memory check */
	string check_result = "";
	if (Infantry == nullptr)
		check_result.append("\n\rInfantry pointer exploded !");
	if (imuYawAngle == nullptr || imuPitchVel == nullptr || imuYawVel == nullptr || imuPitchVel == nullptr)
		check_result.append("\n\imuData pointer exploded !");
	if (FireCmd == nullptr)
		check_result.append("\n\rFireCmd pointer exploded !");
	if (BulletSpeed == nullptr)
		check_result.append("\n\rBulletSpeed pointer exploded !");
	if (RuneMotorRed == nullptr)
		check_result.append("\n\rRuneMotorRed pointer exploded !");
	for each (auto i in Wheel)
		if (i == nullptr)
			check_result.append("\n\rWheel pointer exploded !");
	for each (auto i in Gimbal)
		if (i == nullptr)
			check_result.append("\n\rGimbal pointer exploded !");
	for each (auto i in Collision)
		if (i == nullptr)
			check_result.append("\n\rCollision pointer exploded !");
	for each (auto i in RuneLight)
		if (i == nullptr)
			check_result.append("\n\rRuneLight pointer exploded !");

	// vrep中存在掉线模块
	if (!check_result.empty())
	{
		std::cout << "----------------------------> Objects add failed ! <----------------------------";
		std::cout << check_result << std::endl;
		std::cout << "------------------------> Process will be suspended ! <-------------------------" << std::endl;
		exit(0);
	}
	else
		std::cout << "------------------------> Objects added successfully ! <------------------------" << std::endl;
	/* Applications Init ----------------*/
	User_Tasks_Init();

	/* Start the tasks and timer running. */
	vTaskStartScheduler();
}

/************************ COPYRIGHT(C) SCUT-ROBOTLAB **************************/

