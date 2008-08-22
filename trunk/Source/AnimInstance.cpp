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
	STOP("CAnimInstance::CAnimInstance(int)");

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
		STOP("CAnimInstance::Play(int, int)");
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
		STOP("CAnimInstance::StartFadeTimer(int, int)");
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
	STOP("CAnimInstance::Update(float)");
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
			m_Sheet.GetAnimations()[i].Render(x,y,1,0,m_bisFliped,0, m_bisPlayer);
	}
	STOP("CAnimInstance::Render()");
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
	STOP("CAnimInstance::Stop(int, int)");
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
// Function: “GetFrameHeight” 
// Last Modified: Aug 25, 2008
// Purpose: Gets the height of the frame.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
int CAnimInstance::GetFrameHeight(int Direction, int Action)
{
	PROFILE("CAnimInstance::GetFrameHeight(int, int)");
	string name = GetName(Direction,Action);

	for(unsigned int i = 0; i < m_Sheet.GetAnimations().size();i++)
		if(name == m_Sheet.GetAnimations()[i].GetName())
		{
			STOP("CAnimInstance::Stop(int, int)");
			return m_Sheet.GetAnimations()[i].GetFrames()[m_Sheet.GetAnimations()[i].GetCurrentFrame()].rRender.bottom - m_Sheet.GetAnimations()[i].GetFrames()[m_Sheet.GetAnimations()[i].GetCurrentFrame()].rRender.top;
		}
		STOP("CAnimInstance::Stop(int, int)");
	return -1;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
// Function: “IsPlaying” 
// Last Modified: Aug 25, 2008
// Purpose: Sees if the animation passed in is playing.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
bool CAnimInstance::IsPlaying(int Direction, int Action)
{
	PROFILE("CAnimInstance::IsPlaying(int, int)");
	string name = GetName(Direction,Action);

	for(unsigned int i = 0; i < m_Sheet.GetAnimations().size();i++)
		if(name == m_Sheet.GetAnimations()[i].GetName())
		{
			STOP("CAnimInstance::IsPlaying(int, int)");
			return m_Sheet.GetAnimations()[i].IsPlaying();
		}
		STOP("CAnimInstance::IsPlaying(int, int)");
	return false;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
// Function: “GetFrameWidth” 
// Last Modified: Aug 20, 2008
// Purpose: Gets the width of the current Frame.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
int CAnimInstance::GetFrameWidth(int Direction, int Action)
{
	PROFILE("CAnimInstance::GetFrameWidth(int, int)");
	string name = GetName(Direction,Action);

	for(unsigned int i = 0; i < m_Sheet.GetAnimations().size();i++)
		if(name == m_Sheet.GetAnimations()[i].GetName())
		{
			STOP("CAnimInstance::GetFrameWidth(int, int)");
			return m_Sheet.GetAnimations()[i].GetFrames()[m_Sheet.GetAnimations()[i].GetCurrentFrame()].rRender.right - m_Sheet.GetAnimations()[i].GetFrames()[m_Sheet.GetAnimations()[i].GetCurrentFrame()].rRender.left;
		}
		STOP("CAnimInstance::GetFrameWidth(int, int)");
	return -1;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
// Function: “GetRenderRect” 
// Last Modified: Aug 20, 2008
// Purpose: Gets the Animations render rect.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

		STOP("CAnimInstance::GetRenderRect(int, int)");
	return r;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
// Function: “GetOffsetX” 
// Last Modified: Aug 20, 2008
// Purpose: Gets the x Offset
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CAnimInstance::GetOffsetX(int Direction, int Action)
{
	PROFILE("CAnimInstance::GetOffsetX(int, int)");
	string name = GetName(Direction,Action);

	for(unsigned int i = 0; i < m_Sheet.GetAnimations().size();i++)
		if(name == m_Sheet.GetAnimations()[i].GetName())
		{
			STOP("CAnimInstance::GetOffsetX(int, int)");
			return m_Sheet.GetAnimations()[i].GetFrames()[m_Sheet.GetAnimations()[i].GetCurrentFrame()].ptAnchorX;
		}
		STOP("CAnimInstance::GetOffsetX(int, int)");
	return -1;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
// Function: “GetOffsetY” 
// Last Modified: Aug 20, 2008
// Purpose: Gets the y Offset.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CAnimInstance::GetOffsetY(int Direction, int Action)
{
	PROFILE("CAnimInstance::GetOffsetY(int, int)");
	string name = GetName(Direction,Action);

	for(unsigned int i = 0; i < m_Sheet.GetAnimations().size();i++)
		if(name == m_Sheet.GetAnimations()[i].GetName())
			return m_Sheet.GetAnimations()[i].GetFrames()[m_Sheet.GetAnimations()[i].GetCurrentFrame()].ptAnchorY;
	return -1;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
// Function: “SetLooping” 
// Last Modified: Aug 20, 2008
// Purpose: Sets the Animation to looping.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
// Function: “StopAllAnimations” 
// Last Modified: Aug 20, 2008
// Purpose: Stops the Animation from playing.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAnimInstance::StopAllAnimations()
{
	PROFILE("CAnimInstance::StopAllAnimations()");
	for(unsigned int i = 0; i < m_Sheet.GetAnimations().size(); i++)
	{
		m_Sheet.GetAnimations()[i].Stop();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
// Function: “SetPlayer” 
// Last Modified: Aug 20, 2008
// Purpose: Sets the Animations player bool.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
// Function: “SetPlayer” 
// Last Modified: Aug 20, 2008
// Purpose: retruns the name for the animation to compair to.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
	STOP("CAnimInstance::GetName(int, int)");
	return name;
}