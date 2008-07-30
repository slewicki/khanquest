//////////////////////////////////////////////////////////////////////////
// File CParticleEngine.cpp
//
// Author: Rodney Kite (RK)
//
// Purpose: Contains class definition for CParticleEngine Class, draws particles
//			to the screen using the CEmitter Class
//////////////////////////////////////////////////////////////////////////
#include "CParticleEngine.h"
#include "CEmitter.h"
#include "SGD_Math.h"
CParticleEngine::CParticleEngine(void)
{
	m_dwFrameTimer = 0;
	m_dwPartTimer = 0;
	m_bIsRunning = false;
	m_nBlendMode = 0;
}

CParticleEngine::~CParticleEngine(void)
{
}

void CParticleEngine::Exit(void)
{

}

void CParticleEngine::Render(float fElapsedTime)
{
	if (m_bIsRunning && vEmitterList.size() > 0)
	{
		m_pTM = CSGD_TextureManager::GetInstance();
		m_pD3D = CSGD_Direct3D::GetInstance();	
	
	
		for (unsigned int i = 0; i < vEmitterList.size(); ++i )
		{
	
			for (unsigned int j = 0; j < vEmitterList[i].size(); ++j )
			{
	
				for (unsigned int k = 0; k < m_dwPartTimer ; ++k)
				{
	
					//m_pD3D->GetDirect3DDevice()->GetRenderState(D3DRS_SRCBLEND, &source);
					//m_pD3D->GetDirect3DDevice()->GetRenderState(D3DRS_DESTBLEND, &dest);

					//m_pD3D->GetDirect3DDevice()->SetRenderState(D3DRS_SRCBLEND,  GetBlendMode(vEmitterList[i][j].m_szSourceBlend) );
					//m_pD3D->GetDirect3DDevice()->SetRenderState(D3DRS_DESTBLEND, GetBlendMode(vEmitterList[i][j].m_szDestBlend) );

					//m_pD3D->GetDirect3DDevice()->SetRenderState(D3DRS_ZWRITEENABLE, false);
					//m_pD3D->GetDirect3DDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
					//m_pD3D->GetDirect3DDevice()->SetRenderState (D3DRS_ALPHATESTENABLE, true);

	
					if (vEmitterList[i][j].m_bAlive == true)
					{
						
						m_pTM->Draw(vEmitterList[i][j].m_nImageID,
							(int)vEmitterList[i][j].m_fLocX,
							(int)vEmitterList[i][j].m_fLocY,
							(float)vEmitterList[i][j].m_fCurrentScaleX,
							(float)vEmitterList[i][j].m_fCurrentScaleY,
							0, 0, 0, 0,
							D3DCOLOR_ARGB((int)vEmitterList[i][j].m_cCurrentColor.a, (int)vEmitterList[i][j].m_cCurrentColor.r, (int)vEmitterList[i][j].m_cCurrentColor.g, (int)vEmitterList[i][j].m_cCurrentColor.b)
							);
					}
				}
	
				//m_pD3D->GetDirect3DDevice()->SetRenderState(D3DRS_SRCBLEND, source );
				//m_pD3D->GetDirect3DDevice()->SetRenderState(D3DRS_DESTBLEND, dest );
			}
		}
	}

}

void CParticleEngine::Update(float fElapsedTime)
{
	if (m_bIsRunning)
	{
		// timing
		m_dwPreviousTime = m_dwCurrentTime;
		m_dwCurrentTime = GetTickCount();
		m_dElapsedTime = (float)(m_dwCurrentTime - m_dwPreviousTime);// / 1000.0f);
	
		if (GetTickCount() - m_dwPartTimer > (unsigned int)( 8000 ) )
		{
			m_dwPartTimer = 0;
		}
	
		// do not update if empty
		if (vEmitterList.size() != 0)
		{
			m_dwFrameTimer += m_dElapsedTime; // advance timer
	
			if ( m_dwFrameTimer > (unsigned int)( 1 ) )
			{
				for (unsigned int i = 0; i < vEmitterList.size(); ++i )
				{
					for (unsigned int j = 0; j < vEmitterList[i].size(); ++j )
					{
	
						if (m_dwPartTimer < vEmitterList[i].size())
						{
							++m_dwPartTimer;
						}
						// Update location
						//////////////////////////////////////////////////////////////////////////
						UpdateLoc(i, j);
	
						// Update the scale
						//////////////////////////////////////////////////////////////////////////
						UpdateScale(i, j);
	
						// Update Age
						//////////////////////////////////////////////////////////////////////////
						UpdateAge(i, j);
	
						// Update Color
						//////////////////////////////////////////////////////////////////////////
						UpdateColor(i, j);
					}
				}
				m_dwFrameTimer = 0;
			}
		}
	}
	if (!m_bIsRunning)
	{
		if (vEmitterList.size() != 0)
		{
			vEmitterList.clear();
		}


	}
}


void CParticleEngine::LoadXmlEmitter(char* emitterFileName, float locX, float locY)
{
	m_fLocX = locX;
	m_fLocY = locX;
	// load an emitter
	m_ceEmitter.LoadXMLEmitter(emitterFileName, locX, locY, vParticleList);

	// load the emitter to the list
	vEmitterList.push_back( vParticleList );
	vParticleList.clear();

}
void CParticleEngine::LoadBineryEmitter(char* emitterFileName, float locX, float locY)
{
	m_fLocX = locX;
	m_fLocY = locX;
	// load an emitter
	m_ceEmitter.LoadBinaryEmitter(emitterFileName, locX, locY, vParticleList);

	// load the emitter to the list
	vEmitterList.push_back( vParticleList );
	vParticleList.clear();

}
void CParticleEngine::SetEmitterVel(float XVel, float YVel)
{

}

void CParticleEngine::UpdateAge(int i, int j)
{

	vEmitterList[i][j].m_fAge = vEmitterList[i][j].m_fAge +1;
	float percent = vEmitterList[i][j].m_fAge / vEmitterList[i][j].m_fLife;

	if (vEmitterList[i][j].m_fAge > vEmitterList[i][j].m_fLife)
	{
		if (vEmitterList[i][j].m_bContinuous)
		{
			// reset age and life
			vEmitterList[i][j].m_fAge = 0;
			vEmitterList[i][j].m_fLife = RAND_FLOAT( vEmitterList[i][j].m_fMinLife , vEmitterList[i][j].m_fMaxLife );
			// reset loc
			vEmitterList[i][j].m_fLocX = RAND_FLOAT(m_fLocX - vEmitterList[i][j].m_nOffsetX, m_fLocX +  vEmitterList[i][j].m_nOffsetX);
			vEmitterList[i][j].m_fLocY = RAND_FLOAT(m_fLocY - vEmitterList[i][j].m_nOffsetX, m_fLocY +  vEmitterList[i][j].m_nOffsetY);
			// reset colors
			vEmitterList[i][j].m_cCurrentColor = vEmitterList[i][j].m_cStartColor;
			// reset scale
			vEmitterList[i][j].m_fCurrentScaleX = vEmitterList[i][j].m_fStartScaleX;
			vEmitterList[i][j].m_fCurrentScaleY = vEmitterList[i][j].m_fStartScaleY;
		}
		else
		{
			vEmitterList[i][j].m_bAlive = false;
		}
	}

}

void CParticleEngine::UpdateColor(int i, int j)
{

	m_fColorPercent = (vEmitterList[i][j].m_fAge / vEmitterList[i][j].m_fLife) ;
	D3DXColorLerp(&vEmitterList[i][j].m_cCurrentColor, &vEmitterList[i][j].m_cStartColor, &vEmitterList[i][j].m_cEndColor, m_fColorPercent);
}

void CParticleEngine::UpdateScale(int i, int j)
{
	// update ScaleX
	m_fPercentScale = (vEmitterList[i][j].m_fAge / vEmitterList[i][j].m_fLife) ;

	vEmitterList[i][j].m_fCurrentScaleX = Lerp(vEmitterList[i][j].m_fStartScaleX, vEmitterList[i][j].m_fEndScaleX, m_fPercentScale );
	vEmitterList[i][j].m_fCurrentScaleY = Lerp(vEmitterList[i][j].m_fStartScaleY, vEmitterList[i][j].m_fEndScaleY, m_fPercentScale );
}

void CParticleEngine::UpdateLoc(int i, int j)
{
	vEmitterList[i][j].m_fLocX += vEmitterList[i][j].m_fVelX;
	vEmitterList[i][j].m_fLocY += vEmitterList[i][j].m_fVelY;
}

int CParticleEngine::GetBlendMode(string mode)
{

	if (mode == "Zero")
	{
		m_nBlendMode = D3DBLEND_ZERO;
	}
	if (mode == "One")
	{
		m_nBlendMode = D3DBLEND_ONE;
	}
	if (mode == "SourceColor")
	{
		m_nBlendMode = D3DBLEND_SRCCOLOR;
	}
	if (mode == "InvSourceColor")
	{
		m_nBlendMode = D3DBLEND_INVSRCCOLOR;
	}
	if (mode == "SourceAlpha")
	{
		m_nBlendMode = D3DBLEND_SRCALPHA;
	}
	if (mode == "InvSourceAlpha")
	{
		m_nBlendMode = D3DBLEND_INVSRCALPHA;
	}
	if (mode == "DestinationAlpha")
	{
		m_nBlendMode = D3DBLEND_DESTALPHA;
	}
	if (mode == "InvDestinationAlpha")
	{
		m_nBlendMode = D3DBLEND_INVDESTALPHA;
	}
	if (mode == "InvDestinationColor")
	{
		m_nBlendMode = D3DBLEND_DESTCOLOR;
	}
	if (mode == "DestinationColor")
	{
		m_nBlendMode = D3DBLEND_INVDESTCOLOR;
	}
	if (mode == "SourceAlphaSat")
	{
		m_nBlendMode = D3DBLEND_SRCALPHASAT;
	}
	if (mode == "BothSourceAlpha")
	{
		m_nBlendMode = D3DBLEND_BOTHSRCALPHA;
	}
	if (mode == "BothInvSourceAlpha")
	{
		m_nBlendMode = D3DBLEND_BOTHINVSRCALPHA;
	}
	if (mode == "BlendFactor")
	{
		m_nBlendMode = D3DBLEND_BLENDFACTOR;
	}		
	if (mode == "InvBlendFactor")
	{
		m_nBlendMode = D3DBLEND_INVBLENDFACTOR;
	}

	return m_nBlendMode;
}

