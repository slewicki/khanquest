#include "WinBattleState.h"
#include "CSGD_DirectInput.h"
#include "CSGD_TextureManager.h"
#include "CSGD_WaveManager.h"
#include "MainMenuState.h"
#include "CWorldMapState.h"
#include "CGamePlayState.h"
#include "CGame.h"

CWinBattleState::CWinBattleState(void)
{
}

CWinBattleState::~CWinBattleState(void)
{
}

void CWinBattleState::Enter()
{
	m_pTM = CSGD_TextureManager::GetInstance();
	m_pDI = CSGD_DirectInput::GetInstance();
	m_pWM = CSGD_WaveManager::GetInstance();
	//TODO: Look for better wav
	m_nImageID = m_pTM->LoadTexture("Resource/KQ_WonBattle.png");

	m_BF.InitBitmapFont(m_pTM->LoadTexture("Resource/KQ_FontLucidiaWhite.png"),' ',16,128,128);

	// Go back to the map
	// Call this function if the user wins the battle
	if(CGamePlayState::GetInstance()->GetTerrorLevel() < 100)
	{
		CGamePlayState::GetInstance()->SetTerrorLevel(CGamePlayState::GetInstance()->GetTerrorLevel() + 25);
	}
	CGame::GetInstance()->SetCityConquered(CGame::GetInstance()->GetSelectedCity());
	CGame::GetInstance()->SetSongPlay(WONBATTLE);
	m_bAlpha = false;
	m_bEsc = false;
	m_fEscTimer = 0;
	m_fTimer = 0;
	m_nAlpha = 0;
}

void CWinBattleState::Exit()
{
	CMainMenuState::GetInstance()->SetPause(false);
}

bool CWinBattleState::Input(float fElapsedTime)
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

void CWinBattleState::Render(float fElapsedTime)
{
	m_pTM->Draw(m_nImageID,0,0,.78f,.72f,0,0,0,0,D3DCOLOR_ARGB(m_nAlpha,255,255,255));
	m_BF.DrawTextA("The Battle Is Yours",100,50,.50f,.50f,D3DCOLOR_ARGB(m_nAlpha,255,0,0));	
}

void CWinBattleState::Update(float fElapsedTime)
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
					{
						CGame::GetInstance()->PopCurrentState();
						CGame::GetInstance()->PushState(CWorldMapState::GetInstance());
					}

				}
		}
}

void CWinBattleState::StartEsc()
{
	if(m_fEscTimer > .001)
	{
		m_nAlpha--;
		m_fEscTimer = 0;

		if(m_nAlpha == 0)
		{
			CGame::GetInstance()->PopCurrentState();
			CGame::GetInstance()->PushState(CWorldMapState::GetInstance());
		}
	}
}