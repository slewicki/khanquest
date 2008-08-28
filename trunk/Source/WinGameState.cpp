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
	
	m_BF.InitBitmapFont(m_pTM->LoadTexture("Resource/KQ_FontLucidiaWhite.png"),' ',16,128,128);

	m_bAlpha = false;
	m_bEsc = false;
	m_fTimer = 0;
	m_nAlpha = 0;
	CGame::GetInstance()->SetSongPlay(WONGAME);

}
void CWinGameState::Exit()
{
	CMainMenuState::GetInstance()->SetPause(false);
	CGame::GetInstance()->SetSongPlay(CITYSELECT);
	m_pTM->ReleaseTexture(m_nImageID);
}

bool CWinGameState::Input(float fElapsedTime)
{
	m_fEscTimer += fElapsedTime;

	if(m_bEsc)
		StartEsc();

	if(m_pDI->GetBufferedKey(DIK_RETURN) || m_pDI->GetBufferedKey(DIK_NUMPADENTER) || m_pDI->GetBufferedJoyButton(JOYSTICK_R2) || m_pDI->GetBufferedJoyButton(JOYSTICK_X))
	{	
		m_bEsc = true;
	}
	if(m_pDI->GetBufferedMouseButton(M_BUTTON_LEFT))
	{
		m_bEsc = true;
	}
	return true;
}
void CWinGameState::Render(float fElapsedTime)
{
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
	if(m_bAlpha && m_bEsc)
	{
		if(m_fTimer > 2.f)
			if(m_fTimer > 6.f)
			{
				m_nAlpha--;
				m_fTimer;

				if(m_nAlpha == 0)
					CGame::GetInstance()->ChangeState(CMainMenuState::GetInstance());
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
					CGame::GetInstance()->ChangeState(CMainMenuState::GetInstance());
		}
	}
}