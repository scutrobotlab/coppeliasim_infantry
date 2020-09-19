/**
******************************************************************************
* Copyright (c) 2019 - ~, SCUT-RobotLab Development Team
* @file    win32_support.h
* @brief   This file provide win32 services for MCU programm.
******************************************************************************
* @attention
*
* if you had modified this file, please make sure your code does not have any
* bugs, update the version Number, write dowm your name and the date. The most
* important thing is make sure the users will have clear and definite under-
* standing through your new brief.
*
* <h2><center>&copy; Copyright (c) 2019 - ~, SCUT-RobotLab Development Team.
* All rights reserved.</center></h2>
******************************************************************************
*/
#pragma once

#ifdef __cplusplus
/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "SerialPort.h"
#include <SRML/dr16.h>

/* Private macros ------------------------------------------------------------*/
/* Private type --------------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported variables ---------------------------------------------------------*/
extern CSerialPort SerialPort;
extern CSerialPort SerialPort_Vision;
extern DR16_DataPack_Typedef g_DR16Pack;/* ×îÖÕµÄ°ü */

/* Exported function declarations ---------------------------------------------*/
uint32_t GetMillSecond(void);
uint32_t GetMicroSecond(void);
void LogOutputBacken_Init(void);
uint32_t LogConsole_Output(uint8_t *msg, uint16_t len);
uint32_t LogFile_Output(uint8_t *msg, uint16_t len);
void Service_SerialPortCom_Init();
void Check_Key();
void Convert_To_DR16Pack();
void Check_Mouse();
bool Check_OneKey(char _key);

#endif

/************************ COPYRIGHT(C) SCUT-ROBOTLAB **************************/