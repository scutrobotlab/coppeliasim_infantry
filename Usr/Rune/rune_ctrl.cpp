/**
	****************************************************************************
	* Copyright (c) 2019 - ~, SCUT-RobotLab Development Team
	* @file    rune_ctrl.cpp
	* @author  Sigma hyper_chen@163.com
	* @brief   
	* @date    2020-2-16
	* @version 1.2
	* @par Change Log：
	* Date        	Version 	Author    	Description
	* 2020-2-16		1.0			Sigma		Creator
	* 2020-5-23		1.1			Sigma		Fixed an infinite loop bug
	* 2020-5-30		1.2			Sigma		Now can get the correct timestamp
	*
	============================================================================
										##### How to use this driver #####
	============================================================================
		@note
		-#	Modify macro < RUNE_ARMOR_NUM > in header file.
		-#	Call scan(), light(), motor() in the task. 
	******************************************************************************
	*/
/* Includes ------------------------------------------------------------------*/
#include "rune_ctrl.h"
#include <time.h>
/* Function prototypes -------------------------------------------------------*/
/**
	* @brief Constructor
	*/
RuneCtrl_Classdef::RuneCtrl_Classdef():isAvailable(true), isAdvanced(true), isSucceed(false), angularVelocity(0)
{
	this->reset();
}

/**
	* @brief Destructor
	*/
RuneCtrl_Classdef::~RuneCtrl_Classdef() {}

/**
	* @brief	Calculate motor speed and output.
	* @param	None.
	* @retval	Motor angular velocity.
	*/
double RuneCtrl_Classdef::motor()
{
	/* Speed of low-level energy mechanism */
	static constexpr double stable_speed = 10 * 2 * 3.14159265358979323846 / 60.0f;
	/* Select motor speed */
	this->angularVelocity = this->isAdvanced ? 0.785 * sin(1.884 * getTick()) + 1.305 : stable_speed;
	/* Output */
	return this->angularVelocity;
}

/**
	* @brief	Functions to generate random numbers within the number of armor modules.
	* @param	None.
	* @retval	Random value.
	*/
int RuneCtrl_Classdef::rrand() const 
{
	srand(time(NULL));
	return rand() % armorList.capacity();
}

/**
	* @brief	Functions to get current timestamp.
	* @param	None.
	* @retval	Simulation time that has passed since the program started.
	*/
uint32_t RuneCtrl_Classdef::getTick() const
{
	static uint32_t startTick = CoppeliaSim->GetSimMilliSec();
	return CoppeliaSim->GetSimMilliSec() - startTick;
}

/**
	* @brief	Functions to reset the rune.
	* @param	None.
	* @retval	None.
	*/
void RuneCtrl_Classdef::reset() 
{
	/* All over again */
	for each (auto armor in armorList)
		delete armor;
	armorList.clear();
	armorList.reserve(RUNE_ARMOR_NUM);
	this->generate();
	return;
}

/**
	* @brief	Light up the next leaf randomly.
	* @param	None.
	* @retval	None.
	*/
void RuneCtrl_Classdef::generate()
{
	unsigned int exp_num;
	vector<ArmorState_Typedef*>::iterator i_Armor;
	/* Find the next possible armor module */
	do
	{
		exp_num = this->rrand();
		i_Armor = find_if(armorList.begin(), armorList.end(), [exp_num](ArmorState_Typedef* armor)->bool { return armor->armorNum == exp_num; });
	} while (i_Armor != armorList.end());

	ArmorState_Typedef* p_Next_Armor = new ArmorState_Typedef;
	if (p_Next_Armor == nullptr)
		return;
	p_Next_Armor->armorNum = exp_num;
	p_Next_Armor->activeState = 1;
	p_Next_Armor->collidingTick = getTick();
	armorList.push_back(p_Next_Armor);
	return;
}
/************************ COPYRIGHT(C) SCUT-ROBOTLAB **************************/
