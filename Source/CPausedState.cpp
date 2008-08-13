#include "CPausedState.h"
#include "CGame.h"
#include "CGamePlayState.h"
#include "CWorldMapState.h"
#include "MainMenuState.h"
CPausedState::CPausedState(void)
{
	m_nButtonID = -1;

}
CPausedState::~CPausedState(void)
{
}
void CPausedState::Enter(void)
{
	m_pDI = CSGD_DirectInput::GetInstance();
	m_pTM = CSGD_TextureManager::GetInstance();

	m_rResumeButton.left   = 300;
	m_rResumeButton.top    = 100;
	m_rResumeButton.right  = 430;
	m_rResumeButton.bottom = 160;

	m_rRetreatButton.left   = 300;
	m_rRetreatButton.top    = 200;
	m_rRetreatButton.right  = 430;
	m_rRetreatButton.bottom = 260;

	m_rQuitButton.left   = 300;
	m_rQuitButton.top    = 300;
	m_rQuitButton.right  = 430;
	m_rQuitButton.bottom = 360;

	m_nButtonID = m_pTM->LoadTexture("Resource/KQ_ScrollButton.png");

	m_nLucidiaWhiteID = m_pTM->LoadTexture("Resource/KQ_FontLucidiaWhite.png");

	m_cFont.InitBitmapFont(m_nLucidiaWhiteID, ' ', 16, 128, 128);

}

void CPausedState::Exit(void)
{
	m_pTM->ReleaseTexture(m_nButtonID);
	m_pTM->ReleaseTexture(m_nLucidiaWhiteID);

}

bool CPausedState::Input(float fElapsedTime)
{
	if(CGame::GetInstance()->IsMouseInRect(m_rResumeButton))
	{
		//		CGame::GetInstance()->SetCursorClick();
		if(m_pDI->GetBufferedMouseButton(M_BUTTON_LEFT))
		{
			CGamePlayState::GetInstance()->SetPaused(false);
			CGame::GetInstance()->PopCurrentState();
		}	
	}
	if(CGame::GetInstance()->IsMouseInRect(m_rRetreatButton))
	{
		if(m_pDI->GetBufferedMouseButton(M_BUTTON_LEFT))
		{
			CGamePlayState::GetInstance()->SetTerrorLevel(CGamePlayState::GetInstance()->GetTerrorLevel() - 25);
			CGame::GetInstance()->LoseLastCity();
			CGame::GetInstance()->ChangeState(CWorldMapState::GetInstance());
		}
	}
	if(CGame::GetInstance()->IsMouseInRect(m_rQuitButton))
	{
		if(m_pDI->GetBufferedMouseButton(M_BUTTON_LEFT))
		{
			CGame::GetInstance()->ChangeState(CMainMenuState::GetInstance());
		}
	}
	return true;
}

void CPausedState::Update(float fElapsedTime)
{
}

void CPausedState::Render(float fElapsedTime)
{
	m_pTM->Draw(m_nButtonID, m_rResumeButton.left, m_rResumeButton.top, .4f, .3f);
	m_pTM->Draw(m_nButtonID, m_rRetreatButton.left, m_rRetreatButton.top, .4f, .3f);
	m_pTM->Draw(m_nButtonID, m_rQuitButton.left, m_rQuitButton.top, .4f, .3f);


	m_cFont.DrawTextA("Paused", 175,0,1.0f,1.0f);
	m_cFont.DrawTextA("Resume", m_rResumeButton.left+30, m_rResumeButton.top+24, .2f, .2f, D3DCOLOR_ARGB(255, 255, 0, 0));
	m_cFont.DrawTextA("Retreat", m_rRetreatButton.left+30, m_rRetreatButton.top+24, .2f, .2f, D3DCOLOR_ARGB(255, 255, 0, 0));
	m_cFont.DrawTextA("Quit", m_rQuitButton.left+30, m_rQuitButton.top+24, .2f, .2f, D3DCOLOR_ARGB(255, 255, 0, 0));


}
