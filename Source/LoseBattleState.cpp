#include "LoseBattleState.h"
#include "CSGD_DirectInput.h"
#include "CSGD_TextureManager.h"
#include "CSGD_WaveManager.h"
#include "CGame.h"
#include "MainMenuState.h"

CLoseBattleState::CLoseBattleState(void)
{
}
CLoseBattleState::~CLoseBattleState(void)
{
}

void CLoseBattleState::Enter()
{
	m_pTM = CSGD_TextureManager::GetInstance();
	m_pDI = CSGD_DirectInput::GetInstance();
	m_pWM = CSGD_WaveManager::GetInstance();
	m_nSongID = m_pWM->LoadWave("Resource/KQ_LoseBattle.wav");
	m_nImageID = m_pTM->LoadTexture("Resource/KQ_LoseBattle.png");
	CMainMenuState::GetInstance()->SetPause(true);
	m_BF.InitBitmapFont(m_pTM->LoadTexture("Resource/KQ_FontLucidiaWhite.png"),' ',16,128,128);
	
	m_bAlpha = false;
	m_bEsc = false;
	m_fEscTimer = 0;
	m_fTimer = 0;
	m_nAlpha = 0;
	m_pWM->SetVolume(m_nSongID,CGame::GetInstance()->GetMusicVolume());
	m_pWM->Play(m_nSongID);
}

void CLoseBattleState::Exit()
{
	m_pWM->Stop(m_nSongID);
	m_pWM->UnloadWave(m_nSongID);
	CMainMenuState::GetInstance()->SetPause(false);
}

bool CLoseBattleState::Input(float fElapsedTime)
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

void CLoseBattleState::Render(float fElapsedTime)
{	
	CSGD_Direct3D::GetInstance()->Clear(51,75,0);
	m_pTM->Draw(m_nImageID,0,100,.78f,.72f,0,0,0,0,D3DCOLOR_ARGB(m_nAlpha,255,255,255));
	m_BF.DrawTextA("Many Were Lost, Retreat!/   Live to Fight Again.",50,50,.5f,.5f,D3DCOLOR_ARGB(m_nAlpha,255,0,0));	
	
}

void CLoseBattleState::Update(float fElapsedTime)
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
void CLoseBattleState::StartEsc()
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