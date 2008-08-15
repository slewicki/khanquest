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
	m_bisFliped = false;

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
	string name = GetName(Direction,Action);

	for(unsigned int i = 0; i < m_Sheet.GetAnimations().size();i++)
		if(name == m_Sheet.GetAnimations()[i].GetName())
		{
			m_Sheet.GetAnimations()[i].Play();
			break;
		}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
// Function: “LookUp” 
// Last Modified: July 27, 2008
// Purpose: Looks up the specified sheet using the unit type.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
void CAnimInstance::Update(float fElapsedTime)
{
	for(unsigned int i = 0; i < m_Sheet.GetAnimations().size(); i++)
	{
		string name = m_szAction + "_" + m_szDirection;
		if(m_Sheet.GetAnimations()[i].IsPlaying())
		{
			if(name == m_Sheet.GetAnimations()[i].GetName())
				m_Sheet.GetAnimations()[i].Update(fElapsedTime);
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
// Function: “LookUp” 
// Last Modified: July 27, 2008
// Purpose: Looks up the specified sheet using the unit type.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
void CAnimInstance::Render()
{
	for(unsigned int i = 0; i < m_Sheet.GetAnimations().size(); i++)
	{
		string name = m_szAction + "_" + m_szDirection;
		if(name == m_Sheet.GetAnimations()[i].GetName())
			m_Sheet.GetAnimations()[i].Render(x,y,1,0,m_bisFliped,0);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
// Function: “LookUp” 
// Last Modified: July 27, 2008
// Purpose: Looks up the specified sheet using the unit type.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
void CAnimInstance::Stop(int Direction,int Action)
{
	string name = GetName(Direction,Action);

	for(unsigned int i = 0; i < m_Sheet.GetAnimations().size();i++)
		if(name == m_Sheet.GetAnimations()[i].GetName())
			m_Sheet.GetAnimations()[i].Stop();
}

int CAnimInstance::GetFrameHeight(int Direction, int Action)
{
	string name = GetName(Direction,Action);

	for(unsigned int i = 0; i < m_Sheet.GetAnimations().size();i++)
		if(name == m_Sheet.GetAnimations()[i].GetName())
			return m_Sheet.GetAnimations()[i].GetFrames()[m_Sheet.GetAnimations()[i].GetCurrentFrame()].rRender.bottom - m_Sheet.GetAnimations()[i].GetFrames()[m_Sheet.GetAnimations()[i].GetCurrentFrame()].rRender.top;
	return -1;
}

int CAnimInstance::GetFrameWidth(int Direction, int Action)
{
	string name = GetName(Direction,Action);

	for(unsigned int i = 0; i < m_Sheet.GetAnimations().size();i++)
		if(name == m_Sheet.GetAnimations()[i].GetName())
			return m_Sheet.GetAnimations()[i].GetFrames()[m_Sheet.GetAnimations()[i].GetCurrentFrame()].rRender.right - m_Sheet.GetAnimations()[i].GetFrames()[m_Sheet.GetAnimations()[i].GetCurrentFrame()].rRender.left;
	return -1;
}

RECT CAnimInstance::GetRenderRect(int Direction, int Action)
{
	string name = GetName(Direction,Action);
	RECT r = {0,0,0,0};
	for(unsigned int i = 0; i < m_Sheet.GetAnimations().size();i++)
		if(name == m_Sheet.GetAnimations()[i].GetName())
		{
			r = m_Sheet.GetAnimations()[i].GetFrames()[m_Sheet.GetAnimations()[i].GetCurrentFrame()].rRender;
			int width = r.right - r.left;
			int height = r.bottom - r.top;
			r.left = x+width/2 - m_Sheet.GetAnimations()[i].GetFrames()[m_Sheet.GetAnimations()[i].GetCurrentFrame()].ptAnchorX;
			r.top =  y- m_Sheet.GetAnimations()[i].GetFrames()[m_Sheet.GetAnimations()[i].GetCurrentFrame()].ptAnchorY;
			r.right = r.left + width;
			r.bottom = r.top + height;
		
		}

	return r;
}

int CAnimInstance::GetOffsetX(int Direction, int Action)
{
	string name = GetName(Direction,Action);

	for(unsigned int i = 0; i < m_Sheet.GetAnimations().size();i++)
		if(name == m_Sheet.GetAnimations()[i].GetName())
			return m_Sheet.GetAnimations()[i].GetFrames()[m_Sheet.GetAnimations()[i].GetCurrentFrame()].ptAnchorX;
	return -1;
}

int CAnimInstance::GetOffsetY(int Direction, int Action)
{
	string name = GetName(Direction,Action);

	for(unsigned int i = 0; i < m_Sheet.GetAnimations().size();i++)
		if(name == m_Sheet.GetAnimations()[i].GetName())
			return m_Sheet.GetAnimations()[i].GetFrames()[m_Sheet.GetAnimations()[i].GetCurrentFrame()].ptAnchorY;
	return -1;
}

void CAnimInstance::SetLooping(bool value)
{
	for(unsigned int i = 0; i < m_Sheet.GetAnimations().size(); i++)
	{
		string name = m_szAction + "_" + m_szDirection;
		if(m_Sheet.GetAnimations()[i].IsPlaying())
		{
			if(name == m_Sheet.GetAnimations()[i].GetName())
				m_Sheet.GetAnimations()[i].IsLooping(value);
		}
	}
}
void CAnimInstance::SetPlayer(bool value)
{
	m_bisPlayer = value;
	for(unsigned int i = 0; i < m_Sheet.GetAnimations().size(); i++)
	{
		string name = m_szAction + "_" + m_szDirection;
		if(m_Sheet.GetAnimations()[i].IsPlaying())
		{
			if(name == m_Sheet.GetAnimations()[i].GetName())
				m_Sheet.GetAnimations()[i].SetPlayer(value);
		}
	}
}

string CAnimInstance::GetName(int Direction, int Action)
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
	case 3:
		m_szAction = "Idle";
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
	return name;
}