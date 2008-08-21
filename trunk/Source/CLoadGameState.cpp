//////////////////////////////////////////////////////////
//	File:	"CLoadGameState.cpp"
//
//	Author:	Sean Hamstra (SH)
//
//	Purpose: To contain functionality of the unit creation state
//////////////////////////////////////////////////////////

#include "CLoadGameState.h"
#include "CWorldMapState.h"
#include "UpgradeMenuState.h"
#include "MainMenuState.h"
#include "CGame.h"
#include "CFactory.h"


CLoadGameState::CLoadGameState(void)
{
	m_nButtonID = -1;
	m_nChosenSlot = 0;

	m_nBackgroundID = -1;

	m_bIsNewGame = false;
}


CLoadGameState::~CLoadGameState(void)
{
	
}


void CLoadGameState::Enter(void)
{
	m_pTM = CSGD_TextureManager::GetInstance();
	m_pWM = CSGD_WaveManager::GetInstance();
	m_pDI = CSGD_DirectInput::GetInstance();
	m_nLucidiaWhiteID = m_pTM->LoadTexture("Resource/KQ_FontLucidiaWhite.png");
	m_nBackgroundID = m_pTM->LoadTexture("Resource/KQ_PageBkg3.png");
	m_nButtonID = m_pTM->LoadTexture("Resource/KQ_Slot.png");
	m_nScrollButtonID = m_pTM->LoadTexture("Resource/KQ_ScrollButton.png");

	m_nClickID =  m_pWM->LoadWave("Resource/KQ_Click.wav");
	m_nTickID =  m_pWM->LoadWave("Resource/KQ_ButtonTick.wav");
	m_pWM->SetVolume(m_nClickID,CGame::GetInstance()->GetSFXVolume());

	m_cFont.InitBitmapFont(m_nLucidiaWhiteID, ' ', 16, 128, 128);




	for (int i = 0; i < 3; i++)
	{
		m_rClickRect[i].left = 291;
		m_rClickRect[i].top = 80 + (i*150);
		m_rClickRect[i].right = m_rClickRect[i].left + 219;
		m_rClickRect[i].bottom = m_rClickRect[i].top + 30;
	}
	
	m_rAccept.left = 415;
	m_rAccept.top = 515;
	m_rAccept.right = 545;
	m_rAccept.bottom = 575;	
	
	m_rBack.left = 615;
	m_rBack.top = 515;
	m_rBack.right = 745;
	m_rBack.bottom = 575;


	CGame::GetInstance()->SetSongPlay(CITYSELECT);

	
}

void CLoadGameState::Exit(void)
{
	if(m_pWM->IsWavePlaying(m_nClickID))
		m_pWM->Stop(m_nClickID);
	if(m_pWM->IsWavePlaying(m_nTickID))
		m_pWM->Stop(m_nTickID);
	m_pWM->UnloadWave(m_nClickID);
	m_pWM->UnloadWave(m_nTickID);

	m_pTM->ReleaseTexture(m_nButtonID);
	m_pTM->ReleaseTexture(m_nLucidiaWhiteID);
	m_pTM->ReleaseTexture(m_nBackgroundID);
	




}

bool CLoadGameState::Input(float fElapsedTime)
{
	for (int i = 0; i < 3; i++)
	{
		if(CGame::GetInstance()->GetSaveName(i, true) == "EMPTY")
			m_bIsEmpty[i] = true;
		else
			m_bIsEmpty[i] = false;

		if(CGame::GetInstance()->IsMouseInRect(m_rClickRect[i]))
		{
			// Change cursor to click icon
			CGame::GetInstance()->SetCursorClick();
			if(m_pDI->GetBufferedMouseButton(M_BUTTON_LEFT))
			{
					m_pWM->Play(m_nTickID);

					m_nChosenSlot = i;
				
			}
		}
		
	}
	if(CGame::GetInstance()->IsMouseInRect(m_rAccept))
	{
		CGame::GetInstance()->SetCursorClick();
		if(m_pDI->GetBufferedMouseButton(M_BUTTON_LEFT))
		{
			
			
				m_pWM->Play(m_nClickID);
				
				if(m_bIsNewGame)
				{
					if(m_bIsEmpty[m_nChosenSlot])
					{
						CGame::GetInstance()->NewGame(m_nChosenSlot);
						CGame::GetInstance()->Save(false);
						CGame::GetInstance()->PushState(CWorldMapState::GetInstance());
					}
					else
					{
						CGame::GetInstance()->NewGame(m_nChosenSlot);
						CGame::GetInstance()->Save(true);
						CGame::GetInstance()->PopCurrentState();
						CGame::GetInstance()->PushState(this);
					}

					
				}
				else
					if(CGame::GetInstance()->LoadSlot(m_nChosenSlot))
						CGame::GetInstance()->PushState(CWorldMapState::GetInstance());


				
			
		}
	}
	if(CGame::GetInstance()->IsMouseInRect(m_rBack))
	{
		CGame::GetInstance()->SetCursorClick();
		if(m_pDI->GetBufferedMouseButton(M_BUTTON_LEFT))
		{
			m_pWM->Play(m_nClickID);

			// Go back to the map
			CMainMenuState::GetInstance()->SetPause(false);
			CGame::GetInstance()->PopCurrentState();
		}
	}

	return true;
}

void CLoadGameState::Update(float fElapsedTime)
{
	
}

void CLoadGameState::Render(float fElapsedTime)
{
	m_pTM->Draw(m_nBackgroundID, -20, -10, 1.f, 1.f, 0, 0.f, 0.f, 0.f, D3DCOLOR_ARGB(255, 0, 0, 0));
	
		
	for (int i = 0; i < 3; i++)
	{
		m_pTM->Draw(m_nButtonID, m_rClickRect[i].left, m_rClickRect[i].top, 1.f, 1.f);
		
		
		if(i == m_nChosenSlot)
		{
			string szInfo = CGame::GetInstance()->GetSaveName(i, false);
			m_cFont.DrawTextA(CGame::GetInstance()->GetSaveName(i, true), m_rClickRect[i].left+50, m_rClickRect[i].top, .25f, .25f, D3DCOLOR_ARGB(255, 255, 255, 0));
			m_cFont.DrawTextA(szInfo, (int)(((800-(15*szInfo.length()))*.5f)), m_rClickRect[i].top+20, .25f, .25f, D3DCOLOR_ARGB(255, 255, 255, 0));
		}
		
		else	
			m_cFont.DrawTextA(CGame::GetInstance()->GetSaveName(i, true), m_rClickRect[i].left+50, m_rClickRect[i].top, .25f, .25f, D3DCOLOR_ARGB(255, 0, 0, 0));

	}
	if(m_bIsNewGame)
	{
		m_cFont.DrawTextA("New Game", 320, 10, .3f, .3f, D3DCOLOR_ARGB(255, 255, 0, 0));
	}
	else
		m_cFont.DrawTextA("Load Game", 310, 10, .3f, .3f, D3DCOLOR_ARGB(255, 255, 0, 0));
	
	
	
	m_pTM->Draw(m_nScrollButtonID, m_rAccept.left, m_rAccept.top, .4f, .3f);
	if(m_bIsEmpty[m_nChosenSlot])
		m_cFont.DrawTextA("Accept", m_rAccept.left+24, m_rAccept.top+24, .2f, .2f, D3DCOLOR_ARGB(255, 255, 0, 0));
	else
		m_cFont.DrawTextA("Erase", m_rAccept.left+30, m_rAccept.top+24, .2f, .2f, D3DCOLOR_ARGB(255, 255, 0, 0));


	m_pTM->Draw(m_nScrollButtonID, m_rBack.left, m_rBack.top, .4f, .3f);
	m_cFont.DrawTextA("Back", m_rBack.left+40, m_rBack.top+24, .2f, .2f, D3DCOLOR_ARGB(255, 255, 0, 0));

}



