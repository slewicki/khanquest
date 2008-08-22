//////////////////////////////////////////////////////////////////////////
// File CParticle.cpp
//
// Author: Rodney Kite (RK)
//
// Purpose: Contains class definition for CBoss class
//////////////////////////////////////////////////////////////////////////
#include "CParticle.h"

CParticle::CParticle(void)
{
	PROFILE("CParticle::CParticle()");
	m_bAlive = true;
	m_bContinuous =  true;
	m_fPartLocX  =  0;
	m_fPartLocY  =  0;
	m_nOffsetX  =  0;
	m_nOffsetY  =  0;
	m_fMaxParticles =  0;
	m_fLife =  0;
	m_fMaxLife =  0;
	m_fMinLife  =  0;
	m_fAge =  0;

	m_fVelX =  0;
	m_fVelY =  0;

	m_fStartAlpha =  0;
	m_fStartRed =  255;
	m_fStartGreen =  0;
	m_fStartBlue =  0;

	m_fCurrentAlpha =  0;
	m_fCurrentRed =  0;
	m_fCurrentGreen =  0;
	m_fCurrentBlue =  0;

	m_fEndAlpha =  0;
	m_fEndRed  =  255;
	m_fEndGreen =  0;
	m_fEndBlue =  0;

	m_fStartScaleX =  1;
	m_fCurrentScaleX =  1;
	m_fEndScaleX =  1;

	m_fStartScaleY =  1;
	m_fCurrentScaleY =  1;
	m_fEndScaleY =  1;

	m_fGravityX = 0;
	m_fGravityY = 0;

	m_szSourceBlend =  "";
	m_szDestBlend =  "";

	m_szFileName =  "Resource/KQ_puff.png";
	m_nImageID =  -1;

	m_szTrigger = "";
	m_cStartColor	=   D3DCOLOR_RGBA((int)m_fStartRed , (int)m_fStartGreen, (int)m_fStartBlue, (int)m_fStartAlpha);
	m_cEndColor		=	D3DCOLOR_RGBA((int)m_fEndRed , (int)m_fEndGreen, (int)m_fEndBlue, (int)m_fEndAlpha);
	m_cCurrentColor =	D3DCOLOR_RGBA((int)m_fCurrentRed , (int)m_fCurrentGreen, (int)m_fCurrentBlue, (int)m_fCurrentAlpha); 
	STOP("CParticle::CParticle()");
}

CParticle::~CParticle(void)
{
}
