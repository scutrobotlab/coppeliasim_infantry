/**
  ******************************************************************************
  * @file   System_DataPool.h
  * @brief  All used resources are contained in this file.
  ******************************************************************************
  * @note
  *  - User can define datas including variables ,structs ,and arrays in
  *    this file, which are used in deffrient tasks or services.
**/
#ifndef _DATA_POOL_H_
#define _DATA_POOL_H_

/* Includes ------------------------------------------------------------------*/
/* Middlewares & Drivers Support */
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <CoppeliaSim.h>
#include "SRML/dr16.h"
#include "SRML/filters.h"
#include "SRML/PID.h"
#include "RUNE/rune_ctrl.h"
#include <win32_support.h>
#include "Infantry/infantry_def.h"
#include "Infantry/chassis_ctrl.h"
#include "Infantry/gimbal_ctrl.h"
#include "Infantry/vision.h"

/* Macro Definitions ---------------------------------------------------------*/
#define Tiny_Stack_Size       64
#define Small_Stack_Size      128
#define Normal_Stack_Size     256
#define Large_Stack_Size      512
#define Huge_Stack_Size       1024
#define PriorityVeryLow       1
#define PriorityLow           2
#define PriorityBelowNormal   3
#define PriorityNormal        4
#define PriorityAboveNormal   5
#define PriorityHigh          6
#define PrioritySuperHigh     7
#define PriorityRealtime      8

/* RTOS Resources ------------------------------------------------------------*/
/* Queues */
/* Semaphores */
/* Mutexes */
/* Notifications */
/* Other Resources -----------------------------------------------------------*/
extern _simObjectHandle_Type* Infantry;
extern _simObjectHandle_Type* Wheel[WHEEL_NUM];
extern _simObjectHandle_Type* Gimbal[GIMBAL_DIMENSION];
extern _simObjectHandle_Type* RuneMotorRed;
extern _simCollisHandle_Type* Collision[RUNE_ARMOR_NUM];
extern _simSignalHandle_Type* RuneLight[RUNE_ARMOR_NUM];
extern _simSignalHandle_Type* FireCmd, * BulletSpeed, * imuYawVel, * imuPitchVel, * imuYawAngle, * imuPitchAngle;
//extern _simObjectHandle_Type* InfantryCam;

extern DR16_Classdef dr16;
extern RuneCtrl_Classdef rune;
extern ChassisCtrl_Classdef chassis;
extern GimbalCtrl_Classdef gimbal;
extern Vision_Classdef vision;

/*
  @brief Unified communication struct.
*/
typedef struct
{
  uint8_t *pData;
  uint16_t len;
}_Unified_COB;
#endif
/************************ COPYRIGHT(C) SCUT-ROBOTLAB **************************/
