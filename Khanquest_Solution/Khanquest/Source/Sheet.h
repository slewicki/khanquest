#pragma once
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
// File: “Sheet.h” 
// Author: Dennis Wallace II (DWII) 
// Purpose: This file describes the sheet class that holds a vector of animations for later use 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 

#include "Animation.h"
class CSheet
{
	vector<CAnimation> m_vAnimations;
	int m_szUnitType;

public:
	CSheet(void);
	~CSheet(void);

#pragma region Accessors / Mutators
	//Accessors and Mutators
	int GetUnitType(){return m_szUnitType;}
	void SetUnitType(int value){m_szUnitType = value;}
	void SetPlayerImageID(int value)
	{
		for(unsigned int i = 0; i < m_vAnimations.size();i++)
			m_vAnimations[i].SetPlayerAnimationID(value);
	}
	void SetAIImageID(int value)
	{
		for(unsigned int i = 0; i < m_vAnimations.size(); i++)
			m_vAnimations[i].SetAIAnimationID(value);
	}
	// Last Modified: July 27, 2008 
	vector<CAnimation>& GetAnimations(){return m_vAnimations;}
#pragma endregion

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
// Function: “AddAnimation” 
// Last Modified: July 21, 2008 
// Purpose: This is an example file that lists what should be expected in a standard file. 
//This comment goes at the prototype of the function. 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
	void AddAnimation(CAnimation toAdd)
    {
		m_vAnimations.push_back(toAdd);
    }
};
