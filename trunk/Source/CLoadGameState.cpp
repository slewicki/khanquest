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
	PROFILE("CLoadGameState::CLoadGameState()");
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
	PROFILE("CLoadGameState::Enter()");
	m_pCG = CGame::GetInstance();
	m_pTM = CSGD_TextureManager::GetInstance();
	m_pWM = CSGD_WaveManager::GetInstance();
	m_pDI = CSGD_DirectInput::GetInstance();

	m_pPE = CParticleEngine::GetInstance();

	m_nLucidiaWhiteID = m_pTM->LoadTexture("Resource/KQ_FontLucidiaWhite.png");
	m_nBackgroundID = m_pTM->LoadTexture("Resource/KQ_PageBkg3.png");
	m_nButtonID = m_pTM->LoadTexture("Resource/KQ_Slot.png");
	m_nScrollButtonID = m_pTM->LoadTexture("Resource/KQ_ScrollButton.png");

	// torch 
	m_nTorchPicID = m_pTM->LoadTexture("Resource/KQ_Torch1.png");
	m_nTorchID = m_pPE->LoadBineryEmitter("Resource/Emitters/KQ_Torch2.dat", 100, 300);
	m_nSmokeID1 = m_pPE->LoadBineryEmitter("Resource/Emitters/KQ_Smoke.dat", 100, 300);

	m_nTorchID2 = m_pPE->LoadBineryEmitter("Resource/Emitters/KQ_Torch2.dat", 670, 300);
	m_nSmokeID2 = m_pPE->LoadBineryEmitter("Resource/Emitters/KQ_Smoke.dat", 670, 300);
	
	// fire sound
	m_nTorchSound = m_pWM->LoadWave("Resource/KQ_FireBurn.wav");


	m_nClickID =  m_pWM->LoadWave("Resource/KQ_Click.wav");
	m_nTickID =  m_pWM->LoadWave("Resource/KQ_ButtonTick.wav");
	m_pWM->SetVolume(m_nClickID, m_pCG->GetSFXVolume());

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

	m_fJoyTimer += 0;
	m_pCG->SetSongPlay(CITYSELECT);

	//m_pPE->SetPostion(100, 100, m_nTorchID);
	m_pPE->SetIsRunning(m_nTorchID, true);
	m_pPE->SetIsRunning(m_nSmokeID1, true);
	m_pPE->SetIsRunning(m_nSmokeID2, true);
	m_pPE->SetIsRunning(m_nTorchID2, true);

	if(m_pCG->GetTutorialMode())
	{
		m_bTutorial = true;
		m_rTutorial.top = 400;
		m_rTutorial.left = 350;
		m_rTutorial.bottom = m_rTutorial.top + 64;
		m_rTutorial.right = m_rTutorial.left + 128;
	}
	else
		m_bTutorial = false;

	m_pWM->Play(m_nTorchSound, DSBPLAY_LOOPING);
	m_pWM->SetVolume(m_nTorchSound, m_pCG->GetSFXVolume());
}

void CLoadGameState::Exit(void)
{
	PROFILE("CLoadGameState::Exit()");
	if(m_pWM->IsWavePlaying(m_nClickID))
		m_pWM->Stop(m_nClickID);
	if(m_pWM->IsWavePlaying(m_nTickID))
		m_pWM->Stop(m_nTickID);
	// fire sound
	if(m_pWM->IsWavePlaying(m_nTorchSound))
		m_pWM->Stop(m_nTorchSound);

	m_pWM->UnloadWave(m_nTorchSound);
	m_pWM->UnloadWave(m_nClickID);
	m_pWM->UnloadWave(m_nTickID);

	m_pTM->ReleaseTexture(m_nButtonID);
	m_pTM->ReleaseTexture(m_nLucidiaWhiteID);
	m_pTM->ReleaseTexture(m_nBackgroundID);
	m_pTM->ReleaseTexture(m_nTorchPicID);

	m_pPE->SetIsRunning(m_nTorchID, false);
	m_pPE->SetIsRunning(m_nSmokeID1, false);

	m_pPE->SetIsRunning(m_nTorchID2, false);
	m_pPE->SetIsRunning(m_nSmokeID2, false);

	m_pPE->UnLoadEmitter(m_nTorchID);
	m_pPE->UnLoadEmitter(m_nTorchID2);


}

bool CLoadGameState::Input(float fElapsedTime)
{
	PROFILE("CLoadGameState::Input(float)");
	if(!m_bTutorial)
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

		for (int i = 0; i < 3; i++)
		{
			if(m_pCG->GetSaveName(i, true) == "EMPTY")
				m_bIsEmpty[i] = true;
			else
				m_bIsEmpty[i] = false;
			if(m_nChosenSlot != i &&m_pCG->IsMouseInRect(m_rClickRect[i]))
			{
				// Change cursor to click icon
				m_pCG->SetCursorClick();
				if(m_pDI->GetBufferedMouseButton(M_BUTTON_LEFT) || m_pDI->GetBufferedJoyButton(JOYSTICK_X))
				{
					m_pWM->Play(m_nTickID);

					m_nChosenSlot = i;

				}
			}
		}
		if(m_pCG->IsMouseInRect(m_rAccept))
		{
			if(!m_bIsNewGame && m_bIsEmpty[m_nChosenSlot])
				return true;
			m_pCG->SetCursorClick();
			if(m_pDI->GetBufferedMouseButton(M_BUTTON_LEFT) || m_pDI->GetBufferedJoyButton(JOYSTICK_X))
			{
				m_pWM->Play(m_nClickID);

				if(m_bIsNewGame)
				{
					if(m_bIsEmpty[m_nChosenSlot])
					{
						m_pCG->NewGame(m_nChosenSlot);
						m_pCG->Save(false);
						m_pCG->ChangeState(CWorldMapState::GetInstance());
					}
					else
					{
						m_pCG->NewGame(m_nChosenSlot);
						m_pCG->Save(true);
						m_pCG->PopCurrentState();
						m_pCG->PushState(this);
					}
				}
				else
					if(m_pCG->LoadSlot(m_nChosenSlot))
						m_pCG->ChangeState(CWorldMapState::GetInstance());			
			}
		}
		if(m_pCG->IsMouseInRect(m_rBack) )
		{
			m_pCG->SetCursorClick();
			if(m_pDI->GetBufferedMouseButton(M_BUTTON_LEFT)|| m_pDI->GetBufferedJoyButton(JOYSTICK_X))
			{
				m_pWM->Play(m_nClickID);
				// Go back to the map
				
				CMainMenuState::GetInstance()->SetPause(false);
				m_pCG->PopCurrentState();
			}
		}
	}
	else
	{
		if(m_pCG->IsMouseInRect(m_rTutorial))
		{
			m_pCG->SetCursorClick();
			if(m_pDI->GetBufferedMouseButton(M_BUTTON_LEFT)|| m_pDI->GetBufferedJoyButton(JOYSTICK_X))
			{
				m_pWM->Play(m_nClickID);
				m_bTutorial = false;
			}
		}
	}
	return true;
}

void CLoadGameState::Update(float fElapsedTime)
{
	m_pPE->Update(fElapsedTime);
}

void CLoadGameState::Render(float fElapsedTime)
{
	m_pTM->Draw(m_nBackgroundID, -20, -10, 1.f, 1.f, 0, 0.f, 0.f, 0.f, D3DCOLOR_ARGB(255, 0, 0, 0));
	// draw torch and flame
		m_pTM->Draw(m_nTorchPicID, 80, 300, 0.4f, 0.5f, 0);
		m_pTM->Draw(m_nTorchPicID, 650, 300, 0.4f, 0.5f, 0);
		m_pPE->Render(fElapsedTime);
	if(!m_bTutorial)
	{
		for (int i = 0; i < 3; i++)
		{
			m_pTM->Draw(m_nButtonID, m_rClickRect[i].left, m_rClickRect[i].top, 1.f, 1.f);


			if(i == m_nChosenSlot)
			{
				string szInfo =m_pCG->GetSaveName(i, false);
				m_cFont.DrawTextA(m_pCG->GetSaveName(i, true), m_rClickRect[i].left+50, m_rClickRect[i].top, .25f, .25f, D3DCOLOR_ARGB(255, 255, 255, 0));
				if(!m_bIsEmpty[i])
					m_cFont.DrawTextA(szInfo, (int)(((800-(15*szInfo.length()))*.5f)), m_rClickRect[i].top+20, .25f, .25f, D3DCOLOR_ARGB(255, 255, 255, 0));
			}

			else	
				m_cFont.DrawTextA(m_pCG->GetSaveName(i, true), m_rClickRect[i].left+50, m_rClickRect[i].top, .25f, .25f, D3DCOLOR_ARGB(255, 0, 0, 0));

		}
		if(m_bIsNewGame)
		{
			m_cFont.DrawTextA("New Game", 320, 10, .3f, .3f, D3DCOLOR_ARGB(255, 255, 0, 0));
		}
		else
			m_cFont.DrawTextA("Load Game", 310, 10, .3f, .3f, D3DCOLOR_ARGB(255, 255, 0, 0));


		m_pTM->Draw(m_nScrollButtonID, m_rAccept.left, m_rAccept.top, .4f, .3f);
		if(m_bIsNewGame)
		{
			if(m_bIsEmpty[m_nChosenSlot])
				m_cFont.DrawTextA("Accept", m_rAccept.left+24, m_rAccept.top+24, .2f, .2f, D3DCOLOR_ARGB(255, 0, 155, 0));
			else
				m_cFont.DrawTextA("Erase", m_rAccept.left+30, m_rAccept.top+24, .2f, .2f, D3DCOLOR_ARGB(255, 255, 0, 0));
		}
		else
		{

			m_cFont.DrawTextA("Accept", m_rAccept.left+24, m_rAccept.top+24, .2f, .2f, D3DCOLOR_ARGB(255, 255, 0, 0));
			if(m_bIsEmpty[m_nChosenSlot])
				m_pTM->Draw(m_nScrollButtonID, m_rAccept.left, m_rAccept.top, .4f, .3f, 0, 0.f, 0.f, 0.f, D3DCOLOR_ARGB(150, 50, 50, 50));
		}

		m_pTM->Draw(m_nScrollButtonID, m_rBack.left, m_rBack.top, .4f, .3f);
		m_cFont.DrawTextA("Back", m_rBack.left+40, m_rBack.top+24, .2f, .2f, D3DCOLOR_ARGB(255, 255, 0, 0));

		
	}
	else if(m_pCG->GetTutorialMode() && m_bTutorial)
	{
		RECT toDraw; toDraw.top = 0; toDraw.left = 0; toDraw.right = 578; toDraw.bottom = 495;
		int nImage = m_pTM->LoadTexture("Resource/KQ_TutorialBox.png");
		m_pTM->Draw(nImage,0,2,1.4f,1.2f,&toDraw);
		m_pTM->Draw(m_nScrollButtonID,325,400,.4f,.3f);
		m_cFont.DrawTextA("Accept",350,425,.2f,.2f,D3DCOLOR_ARGB(255,255,0,0));
		m_cFont.DrawTextA("Tutorial",315,15,.4f,.4f,D3DCOLOR_ARGB(255,255,0,0));
		m_cFont.DrawTextA("This next screen is the new game screen./The current file will be selected and is /highlighted in yellow. Feel free to pick /a different file if you like.//Click accept to continue.",100,100,.25f,.25f,D3DCOLOR_ARGB(255,0,0,0));
	}		
}



