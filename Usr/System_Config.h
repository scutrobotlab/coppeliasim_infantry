/**
  ******************************************************************************
  * @file   : System_config.h
  * @brief  : Header for System_config.c
  * @author : Mentos Seetoo
  ****************************************************************************** 
**/

#ifndef SYS_CONFIG_H
#define SYS_CONFIG_H

/* Includes ------------------------------------------------------------------*/

/* Exported function declarations --------------------------------------------*/
#ifdef  __cplusplus
extern "C"{
#endif


#define DEBUG_Yaw_Speed 0
#define DEBUG_Yaw_Angle 0
#define DEBUG_Pitch_Speed 0
#define DEBUG_Pitch_Angle 0
#define DEBUG_Yaw_Follow 0
#define DEBUG_Yaw_Vision 0
#define DEBUG_Pitch_Vision 0

void System_Tasks_Init(void);                                       
void System_Resource_Init(void);   
    
#ifdef  __cplusplus
}
#endif                                


#endif
/************************ COPYRIGHT(C) SCUT-ROBOTLAB **************************/
