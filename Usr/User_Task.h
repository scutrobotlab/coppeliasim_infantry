/**
******************************************************************************
* @file   User_Task.h
* @brief  Header file of User Tasks.
******************************************************************************
* @note
*  - Before running your task, just do what you want ~
*  - More devices or using other classification is decided by yourself ~
*/
#pragma once
/* Includes ------------------------------------------------------------------*/
#include "System_DataPool.h"
/* Private macros ------------------------------------------------------------*/
/* Private type --------------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
extern TaskHandle_t Infantry_Handle;
extern TaskHandle_t Referee_Handle;
extern TaskHandle_t DataDisplay_Handle;
extern TaskHandle_t Transmit_Handle;

/* Exported function declarations --------------------------------------------*/
void User_Tasks_Init(void);
void Task_DataDisplay(void *arg);
int VisionSerial_RxCpltCallback(unsigned char* Recv_Data, unsigned int ReceiveLen);

/************************ COPYRIGHT(C) SCUT-ROBOTLAB **************************/

