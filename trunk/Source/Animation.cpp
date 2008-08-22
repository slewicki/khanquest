//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
// File: “Animation.cpp” 
// Author: Dennis Wallace II (DWII) 
// Purpose: This is the file containing the information for the	Animation class.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
#include "Animation.h"
#include "CSGD_TextureManager.h"
#include "SGD_Math.h"

CAnimation::CAnimation(void)
{
	PROFILE("CAnimation::CAnimation()");
	m_nCurrentFrame = 0;
	m_fFrameTimer = 0;
	m_nAlpha = 255;
	m_bIsLooping = true;
	m_bIsPlaying = false;
	m_bIsPlayer = true;
	m_bIsFading = false;
	m_fFadeTimer = 0.f;
	STOP("CAnimation::CAnimation()");
}

CAnimation::~CAnimation(void)
{
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
// Function: “Update” 
// Last Modified: July 27, 2008
// Purpose: Update the animation
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
bool CAnimation::Update(float fElapsedTime)
{
	PROFILE("CAnimation::Update(float)");
	if(m_bIsFading)
		m_fFadeTimer += fElapsedTime;
	if(!m_bIsPlaying)
	{
		STOP("CAnimation::Update(float)");
		return false;
	}
	
	m_fFrameTimer += fElapsedTime * m_fSpeed *5;
	if(m_fFrameTimer > m_vFrames[m_nCurrentFrame].fDuration)
	{
		m_nCurrentFrame++;
		m_fFrameTimer = 0.f;
		if(m_nCurrentFrame >= m_vFrames.size())
		{
			if(m_bIsLooping)
				m_nCurrentFrame = 0;
			else
			{
				m_nCurrentFrame = int(m_vFrames.size()) - 1;
				Stop();
			}
		}
		
		if(m_vFrames[m_nCurrentFrame].szTriggerName != "")
		{
//			if(m_vFrames[m_nCurrentFrame].ptAccessories.size())
//			{

				CEventSystem::GetInstance()->SendEvent(m_vFrames[m_nCurrentFrame].szTriggerName,(void*)&m_vFrames[m_nCurrentFrame]);
//			}
		}
	}
	STOP("CAnimation::Update(float)");
	return true;
}	
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
// Function: “Render” 
// Last Modified: July 27, 2008 
// Purpose: Renders the animation
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
void CAnimation::Render( int x, int y, float scale, DWORD color,  bool  isfliped, float rotation)
{
	PROFILE("CAnimation::Render(int, int, float, DWORD, bool, float)");
	int width, height;
	width = m_vFrames[m_nCurrentFrame].rRender.right - m_vFrames[m_nCurrentFrame].rRender.left;
	height = m_vFrames[m_nCurrentFrame].rRender.bottom - m_vFrames[m_nCurrentFrame].rRender.top;
	

	if(m_bIsFading)
	{
		if(m_fFadeTimer > 4.f)
			m_nAlpha  = (int)Lerp(255, 0, (m_fFadeTimer-4)*.333f);

	}


	if(!isfliped)
	{
		if(m_bIsPlayer)
			CSGD_TextureManager::GetInstance()->Draw(m_nPlayerAnimationID, x+(int)(width*.5f) - m_vFrames[m_nCurrentFrame].ptAnchorX,
													 y- m_vFrames[m_nCurrentFrame].ptAnchorY,
													 scale, scale, &m_vFrames[m_nCurrentFrame].rRender,
													 rotation, rotation, rotation, D3DCOLOR_ARGB(m_nAlpha,255,255,255));
		else
			CSGD_TextureManager::GetInstance()->Draw(m_nAIAnimationID, x+(int)(width*.5f)- m_vFrames[m_nCurrentFrame].ptAnchorX,
													 y-m_vFrames[m_nCurrentFrame].ptAnchorY,
													 scale, scale, &m_vFrames[m_nCurrentFrame].rRender,
													 rotation, rotation, rotation, D3DCOLOR_ARGB(m_nAlpha,255,255,255));
	}
	else
	{
		if(m_bIsPlayer)
			CSGD_TextureManager::GetInstance()->Draw(m_nPlayerAnimationID, x+(int)(width*.5f) - m_vFrames[m_nCurrentFrame].ptAnchorX+(m_vFrames[m_nCurrentFrame].rRender.right - m_vFrames[m_nCurrentFrame].rRender.left), y - m_vFrames[m_nCurrentFrame].ptAnchorY,-scale,scale,
													 &m_vFrames[m_nCurrentFrame].rRender,rotation,rotation,rotation, D3DCOLOR_ARGB(m_nAlpha,255,255,255));
		else
			CSGD_TextureManager::GetInstance()->Draw(m_nAIAnimationID, x+(int)(width*.5f) - m_vFrames[m_nCurrentFrame].ptAnchorX+(m_vFrames[m_nCurrentFrame].rRender.right - m_vFrames[m_nCurrentFrame].rRender.left), y - m_vFrames[m_nCurrentFrame].ptAnchorY,-scale,scale,
													 &m_vFrames[m_nCurrentFrame].rRender,rotation,rotation,rotation, D3DCOLOR_ARGB(m_nAlpha,255,255,255));
	}
	STOP("CAnimation::Render(int, int, float, DWORD, bool, float)");
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
// Function: “Play” 
// Last Modified: June 27, 2008 
// Purpose: Plays an animation
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
void CAnimation::Play()
{
	PROFILE("CAnimation::Play()");
	Reset();
	Resume();
	STOP("CAnimation::Play()");
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
// Function: “Stop” 
// Last Modified: July 21, 2008 
// Purpose: Stop an animation 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
void CAnimation::Stop()
{
	PROFILE("CAnimation::Stop()");
	m_bIsPlaying = false;
	STOP("CAnimation::Stop()");
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
// Function: “Resume” 
// Last Modified: July 21, 2008 
// Purpose: Resume an animation
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
void CAnimation::Resume()
{
	PROFILE("CAnimation::Resume()");
	m_bIsPlaying = true;
	STOP("CAnimation::Resume()");
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
// Function: “Reset” 
// Last Modified: July 21, 2008
// Purpose: Resets the animation
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
void CAnimation::Reset()
{
	PROFILE("CAnimation::Reset");
	m_nCurrentFrame = 0;
	m_fFrameTimer = 0.f;	
	STOP("CAnimation::Reset");
}
