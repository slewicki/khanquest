#include "LoseGameState.h"
#include "CSGD_DirectInput.h"
#include "CSGD_TextureManager.h"
#include "CSGD_WaveManager.h"
#include "CGame.h"
#include "MainMenuState.h"

CLoseGameState::CLoseGameState(void)
{
}

CLoseGameState::~CLoseGameState(void)
{
}
void CLoseGameState::Enter()
{
	m_pTM = CSGD_TextureManager::GetInstance();
	m_pDI = CSGD_DirectInput::GetInstance();
	m_pWM = CSGD_WaveManager::GetInstance();
	m_nImageID = m_pTM->LoadTexture("Resource/KQ_LoseGame.png");
	CMainMenuState::GetInstance()->SetPause(true);
	m_BF.InitBitmapFont(m_pTM->LoadTexture("Resource/KQ_FontLucidiaWhite.png"),' ',16,128,128);
	
	m_bAlpha = false;
	m_bEsc = false;
	m_fEscTimer = 0;
	m_fTimer = 0;
	m_nAlpha = 0;
	CGame::GetInstance()->SetSongPlay(LOSEGAME);


}

void CLoseGameState::Exit()
{
	CMainMenuState::GetInstance()->SetPause(false);
}

bool CLoseGameState::Input(float fElapsedTime)
{
	m_fEscTimer += fElapsedTime;

	if(m_bEsc)
		StartEsc();

	if(m_pDI->GetBufferedKey(DIK_RETURN) || m_pDI->GetBufferedKey(DIK_NUMPADENTER))
	{	
		m_bEsc = true;
	}
	
	return true;
}

void CLoseGameState::Render(float fElapsedTime)
{	
	CSGD_Direct3D::GetInstance()->Clear(0,0,0);
	m_pTM->Draw(m_nImageID,0,25,.80f,.80f,0,0,0,0,D3DCOLOR_ARGB(m_nAlpha,255,255,255));
	m_BF.DrawTextA("The War Is Over!/Unfortunately, You have been defeated.",25,25,.30f,.30f,D3DCOLOR_ARGB(m_nAlpha,0,0,0));	
	
}

void CLoseGameState::Update(float fElapsedTime)
{
	if(m_bEsc)
		return;
	m_fTimer += fElapsedTime;
	m_fEscTimer += fElapsedTime;
	if(!m_bAlpha)
		if(m_fTimer > .002f && m_nAlpha < 255)
		{
			m_fTimer = 0;
			m_nAlpha++;

			if(m_nAlpha == 255)
				m_bAlpha = true;
		}
	if(m_bAlpha)
	{
		if(m_fTimer > 2.f)
			if(m_fTimer > 6.f)
			{
				m_nAlpha--;
				m_fTimer = 0;
	
				if(m_nAlpha == 0)
					CGame::GetInstance()->PopCurrentState();
			}
	}
}
void CLoseGameState::StartEsc()
{
	if(m_fEscTimer > .001)
	{
		m_nAlpha--;
		m_fEscTimer = 0;

		if(m_nAlpha == 0)
		{
				CGame::GetInstance()->PopCurrentState();
		}
	}
}