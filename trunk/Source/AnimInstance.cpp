#include "AnimInstance.h"
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
// File: “CAnimInstance.cpp” 
// Author: Dennis Wallace II (DWII)
// Purpose: Interface for the units to talk to about animations.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
#include "AnimationManager.h"
#include "Sheet.h"

CAnimInstance::CAnimInstance(int UnitType)
{
	m_pAM = CAnimationManager::GetInstance();
	m_Sheet = m_pAM->LookUp(UnitType);
	m_nUnitType = UnitType;
	x = 0;
	y = 0;
}

CAnimInstance::~CAnimInstance(void)
{

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
// Function: “Play” 
// Last Modified: July 27, 2008
// Purpose: Looks up the specified sheet using the unit type.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
void CAnimInstance::Play(int Direction, int Action)
{	
	switch(Action)
	{
	case 0:
		m_szAction = "Move";
		break;
	case 1:
		m_szAction = "Attack";
		break;
	case 2:
		m_szAction = "Dying";
		break;
	};

	switch(Direction)
	{
	case 0:
		m_szDirection = "North";
		break;
	case 1:
		m_szDirection = "South";
		break;
	case 2:
		m_szDirection = "West";
		break;
	case 3:
		m_szDirection = "North_West";
		break;
	case 4:
		m_szDirection = "South_West";
		break;
	};

	string name = m_szAction + "_" + m_szDirection;
	for(int i = 0; i < m_Sheet.GetAnimations().size();i++)
		if(name == m_Sheet.GetAnimations()[i].GetName())
			m_Sheet.GetAnimations()[i].Play();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
// Function: “LookUp” 
// Last Modified: July 27, 2008
// Purpose: Looks up the specified sheet using the unit type.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
void CAnimInstance::Update(float fElapsedTime)
{
	
	for(int i = 0; i < m_Sheet.GetAnimations().size();i++)
	{
		
		/*if(m_nCounter > m_Sheet.GetAnimations().size() - 1)
			m_nCounter = 0;*/

		string name = m_szAction + "_" + m_szDirection;
		if(name == m_Sheet.GetAnimations()[i].GetName())
			m_Sheet.GetAnimations()[i].Update(fElapsedTime);

	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
// Function: “LookUp” 
// Last Modified: July 27, 2008
// Purpose: Looks up the specified sheet using the unit type.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
void CAnimInstance::Render()
{
	for(int i = 0; i < m_Sheet.GetAnimations().size();i++)
	{
		string name = m_szAction + "_" + m_szDirection;
		if(name == m_Sheet.GetAnimations()[i].GetName())
			m_Sheet.GetAnimations()[i].Render(x,y,1,0,false,0);
	}
}