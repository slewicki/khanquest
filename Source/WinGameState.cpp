#include "WinGameState.h"
#include "CSGD_DirectInput.h"
#include "CSGD_TextureManager.h"
#include "CSGD_WaveManager.h"
#include "CGame.h"
#include "MainMenuState.h"

CWinGameState::CWinGameState(void)
{
}

CWinGameState::~CWinGameState(void)
{
}

void CWinGameState::Enter()
{

	m_pTM = CSGD_TextureManager::GetInstance();
	m_pDI = CSGD_DirectInput::GetInstance();
	m_pWM = CSGD_WaveManager::GetInstance();

	m_nImageID = m_pTM->LoadTexture("Resource/KQ_WonGame.png");
	m_nSongID = m_pWM->LoadWave("Resource/KQ_WonGame.wav");
	
	m_BF.InitBitmapFont(m_pTM->LoadTexture("Resource/KQ_FontLucidiaWhite.png"),' ',16,128,128);

	m_bAlpha = false;
	m_bEsc = false;
	m_fTimer = 0;
	m_nAlpha = 0;
	m_pWM->SetVolume(m_nSongID,CGame::GetInstance()->GetMusicVolume());
	m_pWM->Play(m_nSongID);

}
void CWinGameState::Exit()
{
	m_pWM->Stop(m_nSongID);
	m_pWM->UnloadWave(m_nSongID);
	CMainMenuState::GetInstance()->SetPause(false);

}

bool CWinGameState::Input(float fElapsedTime)
{
	m_fEscTimer += fElapsedTime;

	if(m_bEsc)
		StartEsc();

	if(m_pDI->GetBufferedKey(DIK_RETURN))
	{	
		m_bEsc = true;
	}
	
	return true;
}
void CWinGameState::Render(float fElapsedTime)
{
	CSGD_Direct3D::GetInstance()->Clear(160,80,0);
	RECT toDraw = {0,0,400,668};
	m_pTM->Draw(m_nImageID,0,0,1,1, &toDraw,0,0,0,D3DCOLOR_ARGB(m_nAlpha,255,255,255));

	m_BF.DrawTextA("   Congratulations!/You have Khanquered/      All That The/       Eye Can See",425,100,.30f,.30f,D3DCOLOR_ARGB(m_nAlpha,200,0,0));
} 
void CWinGameState::Update(float fElapsedTime)
{
	if(m_bEsc)
		return;
	m_fTimer += fElapsedTime;
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
				m_fTimer;
				if(m_nAlpha == 0)
					CGame::GetInstance()->PopCurrentState();
			}
	}
}
void CWinGameState::StartEsc()
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