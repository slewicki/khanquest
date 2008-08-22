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
	PROFILE("CAnimInstance::CAnimInstance(int)");
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
	PROFILE("CAnimInstance::Play(int, int)");
	string name = GetName(Direction,Action);

	for(unsigned int i = 0; i < m_Sheet.GetAnimations().size();i++)
		if(name == m_Sheet.GetAnimations()[i].GetName())
		{
			m_Sheet.GetAnimations()[i].Play();
			break;
		}
}

void CAnimInstance::StartFadeTimer(int Direction, int Action)
{	
	PROFILE("CAnimInstance::StartFadeTimer(int, int)");
	string name = GetName(Direction,Action);

	for(unsigned int i = 0; i < m_Sheet.GetAnimations().size();i++)
		if(name == m_Sheet.GetAnimations()[i].GetName())
		{
			m_Sheet.GetAnimations()[i].StartFadeTimer();
			break;
		}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
// Function: “Update” 
// Last Modified: July 27, 2008
// Purpose: Updates the direction the animation is facing.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
void CAnimInstance::Update(float fElapsedTime)
{
	PROFILE("CAnimInstance::Update(float)");
	for(unsigned int i = 0; i < m_Sheet.GetAnimations().size(); i++)
	{
		string name = m_szAction + "_" + m_szDirection;
		//if(m_Sheet.GetAnimations()[i].IsPlaying())
		//{
			if(name == m_Sheet.GetAnimations()[i].GetName())
				m_Sheet.GetAnimations()[i].Update(fElapsedTime);
		//}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
// Function: “Render” 
// Last Modified: July 27, 2008
// Purpose: Render the current Animation.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
void CAnimInstance::Render()
{
	PROFILE("CAnimInstance::Render()");
	for(unsigned int i = 0; i < m_Sheet.GetAnimations().size(); i++)
	{
		string name = m_szAction + "_" + m_szDirection;
		if(name == m_Sheet.GetAnimations()[i].GetName())
			m_Sheet.GetAnimations()[i].Render(x,y,1,0,m_bisFliped,0);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
// Function: “Stop” 
// Last Modified: July 27, 2008
// Purpose: Stops the Animation from playing.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
void CAnimInstance::Stop(int Direction,int Action)
{
	PROFILE("CAnimInstance::Stop(int, int)");
	string name = GetName(Direction,Action);

	for(unsigned int i = 0; i < m_Sheet.GetAnimations().size();i++)
		if(name == m_Sheet.GetAnimations()[i].GetName())
			m_Sheet.GetAnimations()[i].Stop();
}

int CAnimInstance::GetFrameHeight(int Direction, int Action)
{
	PROFILE("CAnimInstance::GetFrameHeight(int, int)");
	string name = GetName(Direction,Action);

	for(unsigned int i = 0; i < m_Sheet.GetAnimations().size();i++)
		if(name == m_Sheet.GetAnimations()[i].GetName())
			return m_Sheet.GetAnimations()[i].GetFrames()[m_Sheet.GetAnimations()[i].GetCurrentFrame()].rRender.bottom - m_Sheet.GetAnimations()[i].GetFrames()[m_Sheet.GetAnimations()[i].GetCurrentFrame()].rRender.top;
	return -1;
}
bool CAnimInstance::IsPlaying(int Direction, int Action)
{
	PROFILE("CAnimInstance::IsPlaying(int, int)");
	string name = GetName(Direction,Action);

	for(unsigned int i = 0; i < m_Sheet.GetAnimations().size();i++)
		if(name == m_Sheet.GetAnimations()[i].GetName())
			return m_Sheet.GetAnimations()[i].IsPlaying();
	return false;
}

int CAnimInstance::GetFrameWidth(int Direction, int Action)
{
	PROFILE("CAnimInstance::GetFrameWidth(int, int)");
	string name = GetName(Direction,Action);

	for(unsigned int i = 0; i < m_Sheet.GetAnimations().size();i++)
		if(name == m_Sheet.GetAnimations()[i].GetName())
			return m_Sheet.GetAnimations()[i].GetFrames()[m_Sheet.GetAnimations()[i].GetCurrentFrame()].rRender.right - m_Sheet.GetAnimations()[i].GetFrames()[m_Sheet.GetAnimations()[i].GetCurrentFrame()].rRender.left;
	return -1;
}

RECT CAnimInstance::GetRenderRect(int Direction, int Action)
{
	PROFILE("CAnimInstance::GetRenderRect(int, int)");
	string name = GetName(Direction,Action);
	RECT r = {0,0,0,0};
	for(unsigned int i = 0; i < m_Sheet.GetAnimations().size();i++)
		if(name == m_Sheet.GetAnimations()[i].GetName())
		{
			r = m_Sheet.GetAnimations()[i].GetFrames()[m_Sheet.GetAnimations()[i].GetCurrentFrame()].rRender;
			int width = r.right - r.left;
			int height = r.bottom - r.top;
			r.left = x+(int)(width*.5f) - m_Sheet.GetAnimations()[i].GetFrames()[m_Sheet.GetAnimations()[i].GetCurrentFrame()].ptAnchorX;
			r.top =  y- m_Sheet.GetAnimations()[i].GetFrames()[m_Sheet.GetAnimations()[i].GetCurrentFrame()].ptAnchorY;
			r.right = r.left + width;
			r.bottom = r.top + height;
		
		}

	return r;
}

int CAnimInstance::GetOffsetX(int Direction, int Action)
{
	PROFILE("CAnimInstance::GetOffsetX(int, int)");
	string name = GetName(Direction,Action);

	for(unsigned int i = 0; i < m_Sheet.GetAnimations().size();i++)
		if(name == m_Sheet.GetAnimations()[i].GetName())
			return m_Sheet.GetAnimations()[i].GetFrames()[m_Sheet.GetAnimations()[i].GetCurrentFrame()].ptAnchorX;
	return -1;
}

int CAnimInstance::GetOffsetY(int Direction, int Action)
{
	PROFILE("CAnimInstance::GetOffsetY(int, int)");
	string name = GetName(Direction,Action);

	for(unsigned int i = 0; i < m_Sheet.GetAnimations().size();i++)
		if(name == m_Sheet.GetAnimations()[i].GetName())
			return m_Sheet.GetAnimations()[i].GetFrames()[m_Sheet.GetAnimations()[i].GetCurrentFrame()].ptAnchorY;
	return -1;
}

void CAnimInstance::SetLooping(bool value)
{
	PROFILE("CAnimInstance::SetLooping(bool)");
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
void CAnimInstance::StopAllAnimations()
{
	PROFILE("CAnimInstance::StopAllAnimations()");
	for(unsigned int i = 0; i < m_Sheet.GetAnimations().size(); i++)
	{
		m_Sheet.GetAnimations()[i].Stop();
	}
}

void CAnimInstance::SetPlayer(bool value)
{
	PROFILE("CAnimInstance::SetPlayer(bool)");
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
	PROFILE("CAnimInstance::GetName(int, int)");
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
	case 4:
		m_szAction = "Move";
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