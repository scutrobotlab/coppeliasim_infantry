/**
	******************************************************************************
	* Copyright (c) 2019 - ~, SCUT-RobotLab Development Team
	* @file    rune_ctrl.h
	* @author  sigma hyper_chen@163.com
	* @brief   Header file of rune_ctrl.cpp
	******************************************************************************
	*/
#pragma once
/* Includes ------------------------------------------------------------------*/
#include <vector>
#include <iostream>
#include <algorithm>
#include "CoppeliaSim.h"
using namespace std;
/* Private macros ------------------------------------------------------------*/
#define RESET_TIME_MS 2000
#define RUNE_ARMOR_NUM 5
/* Exported types ------------------------------------------------------------*/
typedef struct
{
	int armorNum;
	int activeState;
	long collidingTick;
}ArmorState_Typedef;

class RuneCtrl_Classdef
{
public:
	RuneCtrl_Classdef();
	~RuneCtrl_Classdef();
	template<int num> void scan(const int(&arr)[num]);
	template<int num> void light(int (&arr)[num]) const;
	double motor(); 

private:
	vector<ArmorState_Typedef*> armorList;
	bool isAvailable;
	bool isAdvanced;
	bool isSucceed;
	double angularVelocity;
	void reset();
	void generate();
	int rrand() const;
	uint32_t getTick() const;
};

/**
	* @brief	Record collision data and calculate rune logic.
	* @param	arr:	The array which contained the return value of the collision detection.
	* @param	num:	The amount of armor modules on the rune.
	* @retval	None.
	*/
template<int num> void RuneCtrl_Classdef::scan(const int(&arr)[num])
{
	/* Very bad here */
	if (num != armorList.capacity())
		return;
	/* If not active */
	if (!isAvailable)
		return;
	/* Iterate all armor modules */
	for (auto& elem : arr)
	{
		/* Do nothing without collision detected */
		if (!elem)
			continue;
		/* Collision detected */
		ArmorState_Typedef* p_Temp_Armor = new ArmorState_Typedef;
		if (p_Temp_Armor == nullptr)
			return;
		p_Temp_Armor->armorNum = &elem -arr;
		p_Temp_Armor->activeState = 2;
		p_Temp_Armor->collidingTick = getTick();
		if (p_Temp_Armor->armorNum == armorList.back()->armorNum && p_Temp_Armor->collidingTick - armorList.back()->collidingTick <= RESET_TIME_MS)
		{
			armorList.pop_back();
			armorList.push_back(p_Temp_Armor);
			/* If it succeeds */
			if (armorList.size() == armorList.capacity())
				this->isSucceed = true;
			/* If it has to go on */
			else
				this->generate();
		}
		/* If it failed */
		else
		{
			delete p_Temp_Armor;
			this->reset();
		}
	}
	/* If it's too slow */
	if (getTick() - armorList.back()->collidingTick > RESET_TIME_MS)
		this->reset();
	else {}
	return;
}

/**
	* @brief	Change the light of corresponding armor modules.
	* @param	arr:	0, not activated;
						1, waiting to be activated;
						2, has been activated.
	* @param	num:	The amount of armor modules on the rune.
	* @retval	None.
	*/
template<int num> void RuneCtrl_Classdef::light(int(&arr)[num]) const
{
	/* Very bad here */
	if (num != armorList.capacity())
		return;
	/* If it succeeds */
	if (isSucceed)
	{
		for_each(begin(arr), end(arr), [](int& elem) { elem = 2; });
		/* More operations can be written here */
	}
	/* If it have to go on */
	else
	{
		for_each(begin(arr), end(arr), [](int& elem) { elem = 0; });
		for_each(armorList.cbegin(), armorList.cend(), [&arr](auto armor) {arr[armor->armorNum] = armor->activeState; });
	}
	return;
}
/************************ COPYRIGHT(C) SCUT-ROBOTLAB **************************/
