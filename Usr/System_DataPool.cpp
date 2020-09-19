/**
  ******************************************************************************
  * @file   System_DataPool.c
  * @brief  All used resources are contained in this file.
  ******************************************************************************
  * @note
  *  - User can define datas including variables ,structs ,and arrays in
  *    this file, which are used in deffrient tasks or services.
**/
#include "System_DataPool.h"

/* RTOS Resources ------------------------------------------------------------*/
/* Queues */
/* Semaphores */
/* Mutexes */
/* Notifications */

/* Other Resources -----------------------------------------------------------*/
_simObjectHandle_Type* Infantry;
_simObjectHandle_Type* Wheel[WHEEL_NUM];
_simObjectHandle_Type* Gimbal[GIMBAL_DIMENSION];
_simObjectHandle_Type* RuneMotorRed;
_simCollisHandle_Type* Collision[RUNE_ARMOR_NUM];
_simSignalHandle_Type* RuneLight[RUNE_ARMOR_NUM];
_simSignalHandle_Type* FireCmd, * BulletSpeed, * imuYawVel, * imuPitchVel, * imuYawAngle, * imuPitchAngle;
//_simObjectHandle_Type* InfantryCam;

DR16_Classdef dr16;
RuneCtrl_Classdef rune;
ChassisCtrl_Classdef chassis;
GimbalCtrl_Classdef gimbal;
Vision_Classdef vision;

/************************ COPYRIGHT(C) SCUT-ROBOTLAB **************************/



