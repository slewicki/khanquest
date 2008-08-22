//////////////////////////////////////////////////////////
//	File:	"CPausedState.h"
//
//	Author:	Dennis Wallace (DW)
//
//	Purpose: To contain functionality of the CPausedState state
//////////////////////////////////////////////////////////
#include "CPausedState.h"
#include "CGame.h"
#include "CGamePlayState.h"
#include "CWorldMapState.h"
#include "MainMenuState.h"
#include "LoseBattleState.h"
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
	
	CGame::GetInstance()->SetSongPlay(BATTLESTATE);
	m_fJoyTimer = 0;
}

void CPausedState::Exit(void)
{
	m_pTM->ReleaseTexture(m_nButtonID);
	m_pTM->ReleaseTexture(m_nLucidiaWhiteID);

}

bool CPausedState::Input(float fElapsedTime)
{
	m_fJoyTimer = fElapsedTime;
#pragma region Controller to Mouse
	
	if(m_pDI->GetJoystickDir(JOYSTICK_UP) && m_pDI->GetJoystickDir(JOYSTICK_LEFT))
	{
		if(m_fJoyTimer > .002f)
		{
			GetCursorPos(&m_ptMousePos);
			SetCursorPos(m_ptMousePos.x-3,m_ptMousePos.y-3);
			m_fJoyTimer = 0;
		}
	}
	else if(m_pDI->GetJoystickDir(JOYSTICK_UP) && m_pDI->GetJoystickDir(JOYSTICK_RIGHT))
	{
		if(m_fJoyTimer > .002f)
		{
			GetCursorPos(&m_ptMousePos);
			SetCursorPos(m_ptMousePos.x+3,m_ptMousePos.y-3);
			m_fJoyTimer = 0;
		}	
	}
	else if(m_pDI->GetJoystickDir(JOYSTICK_DOWN) && m_pDI->GetJoystickDir(JOYSTICK_LEFT))
	{
		if(m_fJoyTimer > .002f)
		{
			GetCursorPos(&m_ptMousePos);
			SetCursorPos(m_ptMousePos.x-3,m_ptMousePos.y+3);
			m_fJoyTimer = 0;
		}	
	}
	else if(m_pDI->GetJoystickDir(JOYSTICK_DOWN) && m_pDI->GetJoystickDir(JOYSTICK_RIGHT))
	{
		if(m_fJoyTimer > .002f)
		{
			GetCursorPos(&m_ptMousePos);
			SetCursorPos(m_ptMousePos.x+3,m_ptMousePos.y+3);
			m_fJoyTimer = 0;
		}	
	}
	else if(m_pDI->GetJoystickDir(JOYSTICK_UP))
	{
		if(m_fJoyTimer > .002f)
		{
			GetCursorPos(&m_ptMousePos);
			SetCursorPos(m_ptMousePos.x,m_ptMousePos.y-3);
			m_fJoyTimer = 0;
		}
	}
	else if(m_pDI->GetJoystickDir(JOYSTICK_DOWN))
	{
		if(m_fJoyTimer > .002f)
		{
			GetCursorPos(&m_ptMousePos);
			SetCursorPos(m_ptMousePos.x,m_ptMousePos.y+3);
			m_fJoyTimer = 0;
		}
	}
	else if(m_pDI->GetJoystickDir(JOYSTICK_LEFT))
	{
		if(m_fJoyTimer > .002f)
		{
			GetCursorPos(&m_ptMousePos);
			SetCursorPos(m_ptMousePos.x-3,m_ptMousePos.y);
			m_fJoyTimer = 0;
		}
	}
	else if(m_pDI->GetJoystickDir(JOYSTICK_RIGHT))
	{
		if(m_fJoyTimer > .002f)
		{
			GetCursorPos(&m_ptMousePos);
			SetCursorPos(m_ptMousePos.x+3,m_ptMousePos.y);
			m_fJoyTimer = 0;
		}
	}


#pragma endregion

	if(CGame::GetInstance()->IsMouseInRect(m_rResumeButton))
	{
		CGame::GetInstance()->SetCursorClick();
		if(m_pDI->GetBufferedMouseButton(M_BUTTON_LEFT) || m_pDI->GetBufferedJoyButton(JOYSTICK_X))
		{
			CGamePlayState::GetInstance()->SetPaused(false);
			CGame::GetInstance()->PopCurrentState();
		}	
	}
	else if(CGame::GetInstance()->IsMouseInRect(m_rRetreatButton)|| m_pDI->GetBufferedJoyButton(JOYSTICK_X))
	{
		CGame::GetInstance()->SetCursorClick();
		if(m_pDI->GetBufferedMouseButton(M_BUTTON_LEFT))
		{
			CGame::GetInstance()->ChangeState(CLoseBattleState::GetInstance());
			CGame::GetInstance()->AddLoses();

				
		}
	}
	else if(CGame::GetInstance()->IsMouseInRect(m_rQuitButton) || m_pDI->GetBufferedJoyButton(JOYSTICK_X))
	{
		CGame::GetInstance()->SetCursorClick();
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
