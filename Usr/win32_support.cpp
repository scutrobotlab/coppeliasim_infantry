/**
  ******************************************************************************
  * Copyright (c) 2019 - ~, SCUT-RobotLab Development Team
  * @file    win32_support.cpp
  * @brief   This file provide win32 services for MCU programm.
  * @date    2019-12-17
  * @version 1.0
  *
  ==============================================================================
                            How to use this library
  ==============================================================================
    @note
      The following Service is provied by this lib: \n
      - Program run timer.
      - IO stream for SysLog.
      - Serial port communication.
    @warning
      - Using object "SerialPort" to communication by COM1 in default.
        Also user can set other COM port by rewriting "Service_SerialPortCom_Init()".
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
/* Includes ------------------------------------------------------------------*/
#include "atltypes.h"
#include "WinUser.h"
#include <stdbool.h>
#include "stdint.h"
#include <iostream>
#include <fstream>
#include <time.h>
#include <win32_support.h>

/* Private define ------------------------------------------------------------*/
using namespace std;
CSerialPort SerialPort;/* 上位机使用COM3-COM4 */
CSerialPort SerialPort_Vision;/* 视觉使用使用COM1-COM2 */
#define KEY_DOWN(VK_NONAME) ((GetKeyState(VK_NONAME) & 0x8000) ? 1:0) 

/* Private variables ---------------------------------------------------------*/
DR16_DataPack_Typedef g_DR16Pack;//最终的包
bool g_key_down[16];             //按键标志位
int g_mouse[4];                  //鼠标信息 x,y偏移量 + left,right

ofstream outFile;
char outMsg[256];
/* Private type --------------------------------------------------------------*/
/* Private function declarations ---------------------------------------------*/
void Convert_To_DR16Pack();
void Check_Key();
void Check_Mouse();
bool Check_OneKey(char _key);

/**
*  @brief Time Service 
*/
uint32_t GetMillSecond(void)
{
  return (uint32_t)clock();
}

uint32_t GetMicroSecond(void)
{
  return (uint32_t)(clock() * 1000);
}

/**
*  @brief IO stream for SysLog.
*         Output a log file named "sys_log.txt" or display in console.
*/
void LogOutputBacken_Init(void)
{
  /*Console init*/
  SetConsoleTitleA("System Log Record");

  /*Creat file*/
  outFile.open("sys_log.txt", ios::trunc);
}

uint32_t LogConsole_Output(uint8_t *msg, uint16_t len)
{
  memcpy(outMsg, msg,(size_t)len);
  outMsg[len] = 0;

  cout << outMsg;

  return 0;
}

uint32_t LogFile_Output(uint8_t *msg, uint16_t len)
{
  memcpy(outMsg, msg, len);
  outMsg[len - 1] = 0;

  outFile.open("sys_log.txt", ios::app);
  if(outFile.is_open())
    outFile << outMsg;
  else {};
  outFile.close();

  return 0;
}

void LogOutputBacken_DeInit(void)
{
  outFile.close();
}


/**
*  @brief Serial port communication service
*/
void Service_SerialPortCom_Init()
{
  if (!SerialPort.InitPort(3))
  {
    std::cout << "Communication service : UpperMonitor Serial Port init fail !" << std::endl;
  }
  else
  {
    std::cout << "Communication service : UpperMonitor Serial Port init success !" << std::endl;
  }

  /* init Serial Port for vision  */ 
  if (!SerialPort_Vision.InitPort(5))
  {
    std::cout << "Communication service : Vision Serial Port init fail !" << std::endl;
  }
  else
  {
    std::cout << "Communication service : Vision Serial Port init success !" << std::endl;
  }

  if (!SerialPort.OpenListenThread())
  {
    std::cout << "Communication service : Open Listen Thread fail !" << std::endl;
  }
  else
  {
    std::cout << "Communication service : Open Listen Thread success !" << std::endl;
  }

  if (!SerialPort_Vision.OpenListenThread())
  {
    std::cout << "Communication service : Open Vision Listen Thread fail !" << std::endl;
  }
  else
  {
    std::cout << "Communication service : Open Vision Listen Thread success !" << std::endl;
  }

}
/**
  * @brief  把键鼠信息更新到DR16的包
  * @param  None
  * @retval None
  * @author
  */
void Convert_To_DR16Pack() {
    memset(&g_DR16Pack, 0, sizeof(g_DR16Pack));
    for (int i = 0; i < 16; i++)
        g_DR16Pack.key |= (int)g_key_down[i] << i;
    g_DR16Pack.press_l = g_mouse[2];
    g_DR16Pack.press_r = g_mouse[3];
    g_DR16Pack.x = g_mouse[0];
    g_DR16Pack.y = g_mouse[1];
}

/**
  * @brief  检测操作手按键是否按下
  * @param  None
  * @retval None
  * @author
  */
void Check_Key() {
    int cnt = 0;
    g_key_down[cnt++] = (bool)KEY_DOWN('W');
    g_key_down[cnt++] = (bool)KEY_DOWN('S');
    g_key_down[cnt++] = (bool)KEY_DOWN('A');
    g_key_down[cnt++] = (bool)KEY_DOWN('D');
    g_key_down[cnt++] = (bool)KEY_DOWN(16);//SHIFT
    g_key_down[cnt++] = (bool)KEY_DOWN(17);//CTRL
    g_key_down[cnt++] = (bool)KEY_DOWN('Q');
    g_key_down[cnt++] = (bool)KEY_DOWN('E');
    g_key_down[cnt++] = (bool)KEY_DOWN('R');
    g_key_down[cnt++] = (bool)KEY_DOWN('F');
    g_key_down[cnt++] = (bool)KEY_DOWN('G');
    g_key_down[cnt++] = (bool)KEY_DOWN('Z');
    g_key_down[cnt++] = (bool)KEY_DOWN('X');
    g_key_down[cnt++] = (bool)KEY_DOWN('C');
    g_key_down[cnt++] = (bool)KEY_DOWN('V');
    g_key_down[cnt++] = (bool)KEY_DOWN('B');
}

/**
    * @brief  检测任意一个按键
    * @param  None
    * @retval None
    * @author
    */
bool Check_OneKey(char _key) {
    return KEY_DOWN(_key);
}

/**
  * @brief  检测鼠标偏移量及点击
  * @param  None
  * @retval None
  * @author
  */
void Check_Mouse() {
    static const int width = GetSystemMetrics(SM_CXFULLSCREEN);
    static const int height = GetSystemMetrics(SM_CYFULLSCREEN);

    CPoint current_point = 0;
    GetCursorPos(&current_point);

    g_mouse[0] = current_point.x - width / 2;
    g_mouse[1] = current_point.y - height / 2;
    g_mouse[2] = KEY_DOWN(VK_LBUTTON);
    g_mouse[3] = KEY_DOWN(VK_RBUTTON);
}

/************************ COPYRIGHT(C) SCUT-ROBOTLAB **************************/
