#include "OptionsMenuState.h"
#include "MainMenuState.h"
#include "CGame.h"
#include "CSGD_DirectInput.h"
#include "CSGD_Direct3D.h"
#include "CSGD_TextureManager.h"
#include "CSGD_WaveManager.h"
#include "CreditState.h"
#include "KeyBindState.h"
#include "irrXML.h"
#include <fstream>
#include <iostream>
using namespace std;
using namespace irr;
using namespace io;

COptionsMenuState::COptionsMenuState(void)
{
}

COptionsMenuState::~COptionsMenuState(void)
{
}

void COptionsMenuState::Enter()
{
	m_pDI = CSGD_DirectInput::GetInstance();
	m_pTM = CSGD_TextureManager::GetInstance();
	m_pWM = CSGD_WaveManager::GetInstance();
	int nFontID = m_pTM->LoadTexture("Resource/KQ_FontLucidiaWhite.png");
	m_BF.InitBitmapFont(nFontID,' ',16,128,128);
	Parse("Resource/KQ_OptionsMenu.xml");
	m_nClick = m_pWM->LoadWave("Resource/KQ_Click.wav");
	m_nCheckBoxID = m_pTM->LoadTexture("Resource/KQ_CheckBox.PNG");
	m_nCheckMarkID = m_pTM->LoadTexture("Resource/KQ_CheckMark.PNG");
	m_pWM->SetVolume(m_nClick,CGame::GetInstance()->GetSFXVolume());

	char image[128];
	strncpy(image,m_szImageFile.c_str(),m_szImageFile.length());
	image[m_szImageFile.length()]= 0;
	m_nImageID = m_pTM->LoadTexture(image);

	strncpy(image,m_szCursorName.c_str(),m_szCursorName.length());
	image[m_szCursorName.length()] = 0;
	m_nCursorID = m_pTM->LoadTexture(image);
	CGame::GetInstance()->SetSongPlay(CITYSELECT);

	m_ptCursorPosition.x = Buttons[1].ptPosition.x;
	m_ptCursorPosition.y = Buttons[1].ptPosition.y;
	m_nCurrentButton = 1;
	m_pToSwitchTo = NULL;
	m_JoyTimer = 0;
}
void COptionsMenuState::Exit()
{
	SaveOptions();
	if(m_pWM->IsWavePlaying(m_nClick))
		m_pWM->Stop(m_nClick);
	m_pTM->ReleaseTexture(m_nCheckBoxID);
	m_pTM->ReleaseTexture(m_nCheckMarkID);
	m_pTM->ReleaseTexture(m_nImageID);
	m_pTM->ReleaseTexture(m_nCursorID);
	m_pWM->UnloadWave(m_nClick);
}

bool COptionsMenuState::Input(float fElapsedTime)
{
	m_JoyTimer += fElapsedTime;

	if(m_pToSwitchTo != NULL)
		FadeOut(fElapsedTime);
	if(m_bPaused)
		return true;
#pragma region UP
	if(m_pDI->GetBufferedKey(DIK_UP))
	{
		m_nCurrentButton--;
		m_ptCursorPosition.y = Buttons[m_nCurrentButton].ptPosition.y;
		if(m_ptCursorPosition.y < Buttons[1].ptPosition.y)
		{
			m_ptCursorPosition.y = Buttons[m_nNumButtons-1].ptPosition.y;
			m_nCurrentButton = m_nNumButtons-1;
		}
	}
	if(m_pDI->GetJoystickDir(JOYSTICK_UP))
	{
		if(m_JoyTimer > .2)
		{
			m_nCurrentButton--;
			m_ptCursorPosition.y = Buttons[m_nCurrentButton].ptPosition.y;
			if(m_ptCursorPosition.y < Buttons[1].ptPosition.y)
			{
				m_ptCursorPosition.y = Buttons[m_nNumButtons-1].ptPosition.y;
				m_nCurrentButton = m_nNumButtons-1;
			}
			/*if(Buttons[m_nCurrentButton].Action == MUSIC)*/
			m_JoyTimer = 0;
		}
	}
#pragma endregion
#pragma region DOWN
	if(m_pDI->GetBufferedKey(DIK_DOWN))
	{	
		m_nCurrentButton++;
		m_ptCursorPosition.y = Buttons[m_nCurrentButton].ptPosition.y;
		if(m_nCurrentButton > m_nNumButtons-1)
		{
			m_ptCursorPosition.y = Buttons[1].ptPosition.y;
			m_nCurrentButton = 1;
		}
	}
	if(m_pDI->GetJoystickDir(JOYSTICK_DOWN))
	{
		if(m_JoyTimer > .2)
		{
			m_nCurrentButton++;
			m_ptCursorPosition.y = Buttons[m_nCurrentButton].ptPosition.y;
			if(m_nCurrentButton > m_nNumButtons-1)
			{
				m_ptCursorPosition.y = Buttons[1].ptPosition.y;
				m_nCurrentButton = 1;
			}
			//if(Buttons[m_nCurrentButton].Action == MUSIC)
			m_JoyTimer = 0;
		}
	}
#pragma endregion
#pragma region Left
	if(m_pDI->GetBufferedKey(DIK_LEFT))
	{
		if(Buttons[m_nCurrentButton].Action == MUSIC)
		{
			CGame::GetInstance()->SetMusicVolume(CGame::GetInstance()->GetMusicVolume() - 3);
			if(CGame::GetInstance()->GetMusicVolume() < 0)
				CGame::GetInstance()->SetMusicVolume(0);
		}
		if(Buttons[m_nCurrentButton].Action == SFX)
		{
			CGame::GetInstance()->SetSFXVolume(CGame::GetInstance()->GetSFXVolume() - 3);
			if(CGame::GetInstance()->GetSFXVolume() < 0)
				CGame::GetInstance()->SetSFXVolume(0);

			m_pWM->Play(m_nClick);
		}
	}
	if(m_pDI->GetJoystickDir(JOYSTICK_LEFT))
	{	
		if(m_JoyTimer > .2)
		{
			if(Buttons[m_nCurrentButton].Action == MUSIC)
			{
				CGame::GetInstance()->SetMusicVolume(CGame::GetInstance()->GetMusicVolume() - 3);
				if(CGame::GetInstance()->GetMusicVolume() < 0)
					CGame::GetInstance()->SetMusicVolume(0);
			}
			if(Buttons[m_nCurrentButton].Action == SFX)
			{
				CGame::GetInstance()->SetSFXVolume(CGame::GetInstance()->GetSFXVolume() - 3);
				if(CGame::GetInstance()->GetSFXVolume() < 0)
					CGame::GetInstance()->SetSFXVolume(0);

				m_pWM->Play(m_nClick);
			}
			m_JoyTimer = 0;
		}
	}
#pragma endregion 
#pragma region RIGHT
	if(m_pDI->GetBufferedKey(DIK_RIGHT))
	{
		if(Buttons[m_nCurrentButton].Action == MUSIC)
		{
			CGame::GetInstance()->SetMusicVolume(CGame::GetInstance()->GetMusicVolume() + 3);
			if(CGame::GetInstance()->GetMusicVolume() > 100)
				CGame::GetInstance()->SetMusicVolume(100);
		}
		if(Buttons[m_nCurrentButton].Action == SFX)
		{
			CGame::GetInstance()->SetSFXVolume(CGame::GetInstance()->GetSFXVolume() + 3);
			if(CGame::GetInstance()->GetSFXVolume() > 100)
				CGame::GetInstance()->SetSFXVolume(100);
			m_pWM->Play(m_nClick);
		}
	}
	if(m_pDI->GetJoystickDir(JOYSTICK_RIGHT))
	{	
		if(m_JoyTimer > .2)
		{
			if(Buttons[m_nCurrentButton].Action == MUSIC)
			{
				CGame::GetInstance()->SetMusicVolume(CGame::GetInstance()->GetMusicVolume() + 3);
				if(CGame::GetInstance()->GetMusicVolume() > 100)
					CGame::GetInstance()->SetMusicVolume(100);
			}
			if(Buttons[m_nCurrentButton].Action == SFX)
			{
				CGame::GetInstance()->SetSFXVolume(CGame::GetInstance()->GetSFXVolume() + 3);
				if(CGame::GetInstance()->GetSFXVolume() > 100)
					CGame::GetInstance()->SetSFXVolume(100);
				m_pWM->Play(m_nClick);
			}
			m_JoyTimer = 0;
		}
	}
#pragma endregion
#pragma region Selection
	if(m_pDI->GetBufferedKey(DIK_RETURN)|| m_pDI->GetBufferedKey(DIK_NUMPADENTER) || m_pDI->GetBufferedJoyButton(JOYSTICK_X) || m_pDI->GetBufferedJoyButton(JOYSTICK_R2))
	{
		if(Buttons[m_nCurrentButton].Action == KEYBIND)
		{
			m_pWM->Play(m_nClick);
			//Switch States;
			m_pToSwitchTo = CKeyBindState::GetInstance();
			SetPause(true);
		}
		if(Buttons[m_nCurrentButton].Action == CREDITS)
		{
			m_pToSwitchTo = CCreditState::GetInstance();
			SetPause(true);
		}
		if(Buttons[m_nCurrentButton].Action == FPS)
		{
			CGame::GetInstance()->SetFPSDisplay(!CGame::GetInstance()->GetFPSDisplay());
		}
		if(Buttons[m_nCurrentButton].Action == FULLSCREEN)
		{
			//CGame::GetInstance()->SetIsWindowed(!CGame::GetInstance()->GetIsWindowed());
			CGame::GetInstance()->SwitchFullScreen(/*CGame::GetInstance()->GetIsWindowed()*/);
			//CGame::GetInstance()->Initialize(CGame::GetInstance()->GetWindowHandle(), CGame::GetInstance()->GetHInstance(), 800, 600, !CGame::GetInstance()->GetIsWindowed());
			//CSGD_Direct3D::GetInstance()->ChangeDisplayParam(800, 600, CGame::GetInstance()->GetIsWindowed());
			//ShowCursor(true);
			return true;
		}
		if(Buttons[m_nCurrentButton].Action == BACK)
		{
			m_pWM->Play(m_nClick);
			CGame::GetInstance()->PopCurrentState();
			CMainMenuState::GetInstance()->SetPause(false);
		}	
		if(Buttons[m_nCurrentButton].Action == STATEBOXES)
		{
			CGame::GetInstance()->SetBoxes(!CGame::GetInstance()->GetBoxes());
		}
	}
#pragma endregion

	return true;
}

void COptionsMenuState::Render(float fElapsedTime)
{
	CSGD_Direct3D::GetInstance()->Clear(0,0,0);

	m_pTM->Draw(m_nImageID,m_ptImageLoc.x,m_ptImageLoc.y);
	if(m_bPaused)
		return;
	for(int i = 0; i < m_nNumButtons; i++)
	{
		if(Buttons[i].Action == MUSIC)
		{
			char MusicVolume[64];
			sprintf(MusicVolume,"%s %d",Buttons[i].Text.c_str(),CGame::GetInstance()->GetMusicVolume());
			m_BF.DrawTextA(MusicVolume,Buttons[i].ptPosition.x,Buttons[i].ptPosition.y,Buttons[i].fscalex, Buttons[i].fscaley,
				D3DCOLOR_ARGB(m_nAlpha/*Buttons[i].alpha*/, Buttons[i].red, Buttons[i].green, Buttons[i].blue));
		}
		else if(Buttons[i].Action == SFX)
		{
			char SFXVolume[128];
			sprintf(SFXVolume,"%s %d",Buttons[i].Text.c_str(),CGame::GetInstance()->GetSFXVolume());
			m_BF.DrawTextA(SFXVolume,Buttons[i].ptPosition.x,Buttons[i].ptPosition.y,Buttons[i].fscalex, Buttons[i].fscaley,
				D3DCOLOR_ARGB(m_nAlpha/*Buttons[i].alpha*/, Buttons[i].red, Buttons[i].green, Buttons[i].blue));
		}
		else if(Buttons[i].Action == FULLSCREEN)
		{
			m_BF.DrawTextA(Buttons[i].Text,Buttons[i].ptPosition.x, Buttons[i].ptPosition.y, Buttons[i].fscalex, Buttons[i].fscaley,
				D3DCOLOR_ARGB(m_nAlpha/*Buttons[i].alpha*/, Buttons[i].red, Buttons[i].green, Buttons[i].blue));

			m_pTM->Draw(m_nCheckBoxID,Buttons[i].ptPosition.x + 200,Buttons[i].ptPosition.y);
			if(!CGame::GetInstance()->GetIsWindowed())
				m_pTM->Draw(m_nCheckMarkID,Buttons[i].ptPosition.x + 200,Buttons[i].ptPosition.y);

		}
		else if(Buttons[i].Action == FPS)
		{
			m_BF.DrawTextA(Buttons[i].Text,Buttons[i].ptPosition.x, Buttons[i].ptPosition.y, Buttons[i].fscalex, Buttons[i].fscaley,
				D3DCOLOR_ARGB(m_nAlpha/*Buttons[i].alpha*/, Buttons[i].red, Buttons[i].green, Buttons[i].blue));

			m_pTM->Draw(m_nCheckBoxID,Buttons[i].ptPosition.x + 200,Buttons[i].ptPosition.y);
			if(CGame::GetInstance()->GetFPSDisplay())
				m_pTM->Draw(m_nCheckMarkID,Buttons[i].ptPosition.x + 200, Buttons[i].ptPosition.y);
		}
		else if(Buttons[i].Action == STATEBOXES)
		{
			m_BF.DrawTextA(Buttons[i].Text,Buttons[i].ptPosition.x, Buttons[i].ptPosition.y, Buttons[i].fscalex, Buttons[i].fscaley,
				D3DCOLOR_ARGB(m_nAlpha/*Buttons[i].alpha*/, Buttons[i].red, Buttons[i].green, Buttons[i].blue));

			m_pTM->Draw(m_nCheckBoxID,Buttons[i].ptPosition.x + 200,Buttons[i].ptPosition.y);
			if(CGame::GetInstance()->GetBoxes())
				m_pTM->Draw(m_nCheckMarkID,Buttons[i].ptPosition.x + 200, Buttons[i].ptPosition.y);
		}
		else
		{
			m_BF.DrawTextA(Buttons[i].Text,Buttons[i].ptPosition.x, Buttons[i].ptPosition.y, Buttons[i].fscalex, Buttons[i].fscaley,
				D3DCOLOR_ARGB(m_nAlpha/*Buttons[i].alpha*/, Buttons[i].red, Buttons[i].green, Buttons[i].blue));
		}	
	}
	m_pTM->Draw(m_nCursorID,m_ptCursorPosition.x-50,m_ptCursorPosition.y,m_fCurScaleX,m_fCurScaleY,
		0,0,0,0,D3DCOLOR_ARGB(m_nAlpha,255,255,255));

}
void COptionsMenuState::Update(float fElapsedTime)
{
	FadeIn(fElapsedTime);

	m_pWM->SetVolume(m_nClick,CGame::GetInstance()->GetSFXVolume());
}
void COptionsMenuState::FadeIn(float fElapsedTime)
{
	m_fTimer += fElapsedTime;

	if(!m_bAlpha)
		if(m_fTimer > .00002f && m_nAlpha < 255)
		{
			m_fTimer = 0;
			m_nAlpha+=5;

			if(m_nAlpha == 255)
				m_bAlpha = true;
		}
}
void COptionsMenuState::FadeOut(float fElapsedTime)
{
	m_fEscTimer += fElapsedTime;
	if(m_fEscTimer > .0001)
	{
		m_nAlpha-=5;
		m_fEscTimer = 0;

		if(m_nAlpha <= 0)
		{	
			m_nAlpha = 0;
			m_ptCursorPosition  = Buttons[1].ptPosition;
			m_nCurrentButton = 1;
			CGame::GetInstance()->PushState(m_pToSwitchTo);
			m_pToSwitchTo = NULL;
			m_bAlpha = false;
		}
	}
}
bool COptionsMenuState::Parse(char* szFileName)
{
	//Function variables - do not change
	string szName, szTempBool;
	IrrXMLReader* xml = createIrrXMLReader(szFileName);
	int nCounter = 0;
	while(xml && xml->read())
	{
		switch(xml->getNodeType())
		{
		case EXN_ELEMENT:
			{
				//Grabs the attribute name
				szName = xml->getNodeName();
			}
			break;
		case EXN_TEXT:
			{
				//Checks the Attribute name and sets the approiate value
				if (!strcmp("ImageLocationX", szName.c_str()))
				{
					m_ptImageLoc.x = atoi(xml->getNodeName());
				}				
				else if (!strcmp("ImageLocationY", szName.c_str()))
				{
					m_ptImageLoc.y = atoi(xml->getNodeName());
				}
				else if (!strcmp("ImageName", szName.c_str()))
				{
					m_szImageFile = xml->getNodeName();
				}
				else if (!strcmp("SizeX", szName.c_str()))
				{
					m_ptImageSize.x = atoi(xml->getNodeName());
				}			
				else if (!strcmp("SizeY", szName.c_str()))
				{
					m_ptImageSize.y = atoi(xml->getNodeName());
				}
				else if(!strcmp("CursorName",szName.c_str()))
				{
					m_szCursorName = xml->getNodeName();
				}
				else if(!strcmp("CursorScaleX",szName.c_str()))
				{
					m_fCurScaleX = float(atof(xml->getNodeName()));
				}
				else if(!strcmp("CursorScaleY",szName.c_str()))
				{
					m_fCurScaleY = float(atof(xml->getNodeName()));
				}
				else if (!strcmp("NumButtons", szName.c_str()))
				{
					m_nNumButtons = atoi(xml->getNodeName());
					Buttons = new TextToHold[m_nNumButtons];
				}	
				else if(!strcmp("ButtonPositionX",szName.c_str()))
				{
					if(nCounter < m_nNumButtons)
						Buttons[nCounter].ptPosition.x = atoi(xml->getNodeName());
				}
				else if(!strcmp("ButtonPositionY",szName.c_str()))
				{
					if(nCounter < m_nNumButtons)
						Buttons[nCounter].ptPosition.y = atoi(xml->getNodeName());
				}
				else if(!strcmp("TextColorA",szName.c_str()))
				{
					if(nCounter < m_nNumButtons)
						Buttons[nCounter].alpha = atoi(xml->getNodeName());
				}
				else if(!strcmp("TextColorR",szName.c_str()))
				{
					if(nCounter < m_nNumButtons)
						Buttons[nCounter].red = atoi(xml->getNodeName());
				}
				else if(!strcmp("TextColorG",szName.c_str()))
				{
					if(nCounter < m_nNumButtons)
						Buttons[nCounter].green  = atoi(xml->getNodeName());
				}
				else if(!strcmp("TextColorB",szName.c_str()))
				{
					if(nCounter < m_nNumButtons)
						Buttons[nCounter].blue = atoi(xml->getNodeName());
				}
				else if(!strcmp("TextScaleX",szName.c_str()))
				{
					if(nCounter < m_nNumButtons)
						Buttons[nCounter].fscalex = float(atof(xml->getNodeName()));
				}
				else if(!strcmp("TextScaleY",szName.c_str()))
				{
					if(nCounter < m_nNumButtons)
						Buttons[nCounter].fscaley = float(atof(xml->getNodeName()));
				}
				else if(!strcmp("Action",szName.c_str()))
				{
					if(nCounter < m_nNumButtons)
						Buttons[nCounter].Action = atoi(xml->getNodeName()); 
				}
				else if(!strcmp("ButtonText",szName.c_str()))
				{
					if(nCounter < m_nNumButtons)
					{
						Buttons[nCounter].Text = xml->getNodeName();
						nCounter++;
					}
				}
				szName = "none";
			}
			break;
		}
	}
	//Delete the parser
	delete xml;
	return true;
}
void COptionsMenuState::SaveOptions()
{
	ofstream output("Resource/KQ_Options.dat",ios_base::binary);
	
	for( int i = 0; i < 4 ;++i)
	{
		DWORD temp = CKeyBindState::GetInstance()->GetBoundKey(i);
		output.write((char*)&temp,sizeof(temp));
	}

	bool bFPS = CGame::GetInstance()->GetFPSDisplay();
	output.write((char*)&bFPS,sizeof(bFPS));
	bool bBoxes = CGame::GetInstance()->GetBoxes();
	output.write((char*)&bBoxes,sizeof(bBoxes));
	int nMusicVol = CGame::GetInstance()->GetMusicVolume();
	output.write((char*)&nMusicVol,sizeof(nMusicVol));
	int nSFXVol = CGame::GetInstance()->GetSFXVolume();
	output.write((char*)&nSFXVol,sizeof(nSFXVol));
}