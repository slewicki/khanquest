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
	if (vEmitterList.size() > 0)
	{
		m_pTM = CSGD_TextureManager::GetInstance();
		m_pD3D = CSGD_Direct3D::GetInstance();	


		for (unsigned int i = 0; i < vEmitterList.size(); ++i )
		{

			for (unsigned int j = 0; j < vEmitterList[i].vParticleList.size(); ++j )
			{

				if (vEmitterList[i].vParticleList[j].m_bAlive == true)
				{
					// Set blend mode
					m_pD3D->GetDirect3DDevice()->GetRenderState(D3DRS_SRCBLEND, &source);
					m_pD3D->GetDirect3DDevice()->GetRenderState(D3DRS_DESTBLEND, &dest);

					m_pD3D->GetDirect3DDevice()->SetRenderState(D3DRS_SRCBLEND,  GetBlendMode(vEmitterList[i].vParticleList[j].m_szSourceBlend) );
					m_pD3D->GetDirect3DDevice()->SetRenderState(D3DRS_DESTBLEND, GetBlendMode(vEmitterList[i].vParticleList[j].m_szDestBlend) );

					m_pD3D->GetDirect3DDevice()->SetRenderState(D3DRS_ZWRITEENABLE, false);
					m_pD3D->GetDirect3DDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
					m_pD3D->GetDirect3DDevice()->SetRenderState (D3DRS_ALPHATESTENABLE, true);

					for (unsigned int k = 0; k < m_dwPartTimer ; ++k)
					{

						m_pTM->Draw(vEmitterList[i].vParticleList[j].m_nImageID,
							(int)vEmitterList[i].vParticleList[j].m_fPartLocX,
							(int)vEmitterList[i].vParticleList[j].m_fPartLocY,
							(float)vEmitterList[i].vParticleList[j].m_fCurrentScaleX,
							(float)vEmitterList[i].vParticleList[j].m_fCurrentScaleY,
							0, 0, 0, 0,
							D3DCOLOR_ARGB((int)vEmitterList[i].vParticleList[j].m_cCurrentColor.a, (int)vEmitterList[i].vParticleList[j].m_cCurrentColor.r, (int)vEmitterList[i].vParticleList[j].m_cCurrentColor.g, (int)vEmitterList[i].vParticleList[j].m_cCurrentColor.b)
							);
					}
					// reset blend mode to default
					m_pD3D->GetDirect3DDevice()->SetRenderState(D3DRS_SRCBLEND, source );
					m_pD3D->GetDirect3DDevice()->SetRenderState(D3DRS_DESTBLEND, dest );
				}

			}
		}
	}

}

void CParticleEngine::Update(float fElapsedTime)
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
				if (vEmitterList[i].m_bIsRunning)
				{
					for (unsigned int j = 0; j < vEmitterList[i].vParticleList.size(); ++j )
					{

						if (m_dwPartTimer < vEmitterList.size())
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
			}
			m_dwFrameTimer = 0;
		}
	}
}



int CParticleEngine::LoadXmlEmitter(char* emitterFileName, float locX, float locY)
{
	// load an emitter
	m_ceEmitter.LoadXMLEmitter(emitterFileName, locX, locY);

	// load the emitter to the list
	vEmitterList.push_back( m_ceEmitter );
	return (int)vEmitterList.size() - 1;


}
int CParticleEngine::LoadBineryEmitter(char* emitterFileName, float locX, float locY)
{
	m_ceEmitter.m_nEmitterXLoc = locX;
	m_ceEmitter.m_nEmitterYLoc = locY;

	// load an emitter
	m_ceEmitter.LoadBinaryEmitter(emitterFileName, locX, locY);

	// load the emitter to the list
	vEmitterList.push_back( m_ceEmitter );
	m_ceEmitter.ClearList();
	return (int)vEmitterList.size() - 1;

}
void CParticleEngine::SetEmitterVel(float XVel, float YVel)
{

}

void CParticleEngine::UpdateAge(int i, int j)
{

	vEmitterList[i].vParticleList[j].m_fAge = vEmitterList[i].vParticleList[j].m_fAge +1;
	float percent = vEmitterList[i].vParticleList[j].m_fAge / vEmitterList[i].vParticleList[j].m_fLife;

	if (vEmitterList[i].vParticleList[j].m_fAge > vEmitterList[i].vParticleList[j].m_fLife)
	{
		if (vEmitterList[i].vParticleList[j].m_bAlive)
		{
			if (vEmitterList[i].vParticleList[j].m_bContinuous)
			{
				// reset age and life
				vEmitterList[i].vParticleList[j].m_fAge = 0;
				vEmitterList[i].vParticleList[j].m_fLife = RAND_FLOAT( vEmitterList[i].vParticleList[j].m_fMinLife , vEmitterList[i].vParticleList[j].m_fMaxLife );
				// reset loc
				vEmitterList[i].vParticleList[j].m_fPartLocX = RAND_FLOAT(vEmitterList[i].m_nEmitterXLoc - vEmitterList[i].vParticleList[j].m_nOffsetX, vEmitterList[i].m_nEmitterXLoc +  vEmitterList[i].vParticleList[j].m_nOffsetX);
				vEmitterList[i].vParticleList[j].m_fPartLocY = RAND_FLOAT(vEmitterList[i].m_nEmitterYLoc - vEmitterList[i].vParticleList[j].m_nOffsetX, vEmitterList[i].m_nEmitterYLoc +  vEmitterList[i].vParticleList[j].m_nOffsetY);
				// reset colors
				vEmitterList[i].vParticleList[j].m_cCurrentColor = vEmitterList[i].vParticleList[j].m_cStartColor;
				// reset scale
				vEmitterList[i].vParticleList[j].m_fCurrentScaleX = vEmitterList[i].vParticleList[j].m_fStartScaleX;
				vEmitterList[i].vParticleList[j].m_fCurrentScaleY = vEmitterList[i].vParticleList[j].m_fStartScaleY;
			}
			else
			{
				vEmitterList[i].vParticleList[j].m_bAlive = false;
			}
		}
	}

}

void CParticleEngine::UpdateColor(int i, int j)
{

	m_fColorPercent = (vEmitterList[i].vParticleList[j].m_fAge / vEmitterList[i].vParticleList[j].m_fLife) ;
	D3DXColorLerp(&vEmitterList[i].vParticleList[j].m_cCurrentColor, &vEmitterList[i].vParticleList[j].m_cStartColor, &vEmitterList[i].vParticleList[j].m_cEndColor, m_fColorPercent);
}

void CParticleEngine::UpdateScale(int i, int j)
{
	// update ScaleX
	m_fPercentScale = (vEmitterList[i].vParticleList[j].m_fAge / vEmitterList[i].vParticleList[j].m_fLife) ;

	vEmitterList[i].vParticleList[j].m_fCurrentScaleX = Lerp(vEmitterList[i].vParticleList[j].m_fStartScaleX, vEmitterList[i].vParticleList[j].m_fEndScaleX, m_fPercentScale );
	vEmitterList[i].vParticleList[j].m_fCurrentScaleY = Lerp(vEmitterList[i].vParticleList[j].m_fStartScaleY, vEmitterList[i].vParticleList[j].m_fEndScaleY, m_fPercentScale );
}

void CParticleEngine::UpdateLoc(int i, int j)
{
	vEmitterList[i].vParticleList[j].m_fPartLocX += vEmitterList[i].vParticleList[j].m_fVelX;
	vEmitterList[i].vParticleList[j].m_fPartLocY += vEmitterList[i].vParticleList[j].m_fVelY;
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

void CParticleEngine::SetPostion(float XPos, float YPos, int ID)
{
	if (vEmitterList.size() > 0 && vEmitterList[ID].m_bHasParts)
	{
		vEmitterList[ID].m_nEmitterXLoc = XPos;
		vEmitterList[ID].m_nEmitterYLoc = YPos;
	}
}
void CParticleEngine::SetIsRunning(int ID, bool isRunnig)
{
	vEmitterList[ID].m_bIsRunning = isRunnig;
}
void CParticleEngine::UnLoadEmitter(int ID)
{
	CEmitter empty;
	vEmitterList[ID].m_bIsRunning =  false;
	if (vEmitterList.size() > 0 && vEmitterList[ID].m_bHasParts)
	{
		vEmitterList[ID] = empty;
	}
}

