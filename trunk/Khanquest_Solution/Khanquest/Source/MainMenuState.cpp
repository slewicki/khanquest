#include "MainMenuState.h"
#include "CGame.h"
#include "CWorldMapState.h"
#include "OptionsMenuState.h"
#include "AttractMode.h"
#include "CSGD_WaveManager.h"
#include "OutroState.h"

#include "irrXML.h"
#include <fstream>
#include <iostream>
using namespace std;
using namespace irr;
using namespace io;

CMainMenuState::CMainMenuState(void)
{
}

CMainMenuState::~CMainMenuState(void)
{
}

void CMainMenuState::Enter(void)
{
	m_pDI = CSGD_DirectInput::GetInstance();
	m_pTM = CSGD_TextureManager::GetInstance();
	m_pWM = CSGD_WaveManager::GetInstance();
	int nFontID = m_pTM->LoadTexture("Resource/KQ_FontLucidiaWhite.png");
	m_BF.InitBitmapFont(nFontID,' ',16,128,128);
	Parse("Resource/KQ_MainMenu.xml");
	char image[128];
	strncpy(image,m_szImageFile.c_str(),m_szImageFile.length());
	image[m_szImageFile.length()]= 0;
	m_nImageID = m_pTM->LoadTexture(image);

	strncpy(image,m_szCursorName.c_str(),m_szCursorName.length());
	image[m_szCursorName.length()] = 0;
	m_nCursorID = m_pTM->LoadTexture(image);

	m_ptCursorPosition.x = Buttons[1].ptPosition.x;
	m_ptCursorPosition.y = Buttons[1].ptPosition.y;
	m_nCurrentButton = 1;
	m_bPaused = false;

	m_bAlpha = false;

	m_fEscTimer = 0;
	m_fTimer = 0;
	m_nAlpha = 0;
	
	m_nVolume = 0;
	m_nMaxVolume = 0;

	m_pWM->SetVolume(m_nSongID,m_nVolume);
	m_pWM->Play(m_nSongID);
	m_pToSwitchTo = NULL;
}

void CMainMenuState::Exit(void)
{
	m_pWM->UnloadWave(m_nSongID);
	delete [] Buttons;
}

bool CMainMenuState::Input(float fElapsedTime)
{
	if(m_pToSwitchTo != NULL)
		FadeOut(fElapsedTime);

	if(m_bPaused)
		return true;

	if(m_pDI->GetBufferedKey(DIK_1))
	{
		m_bPaused = true;
		CGame::GetInstance()->PushState(CAttractMode::GetInstance());
	}
	
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

	if(m_pDI->GetBufferedKey(DIK_RETURN))
	{
		switch(Buttons[m_nCurrentButton].Action)
		{
		case WorldMapState:
			{
				m_bPaused = true;
				m_pToSwitchTo = CWorldMapState::GetInstance();
			}
			break;
		case Load:
			{
				//TODO: Make Loading State
			}
			break;
		case Options:
			{
				m_bPaused = true;
				m_pToSwitchTo = COptionsMenuState::GetInstance();
			}
			break;
		case ExitGame:
			{
				//TODO: ExitGameState
				m_bPaused = true;
				m_pToSwitchTo = COutroState::GetInstance();
			}
			break;
		default:
			{

			}
			break;
		};
	}

	return true;
}

void CMainMenuState::Update(float fElapsedTime)
{
	if(m_bPaused)
		return;
	
	FadeIn(fElapsedTime);
}

void CMainMenuState::Render(float fElapsedTime)
{
	CSGD_Direct3D::GetInstance()->Clear(128,60,0);
	//m_pTM->Draw(m_nImageID,m_ptImageLoc.x,m_ptImageLoc.y);

	for(int i = 0; i < m_nNumButtons; i++)
	{
		m_BF.DrawTextA(Buttons[i].Text,Buttons[i].ptPosition.x, Buttons[i].ptPosition.y, Buttons[i].fscalex, Buttons[i].fscaley,
			D3DCOLOR_ARGB(m_nAlpha/*Buttons[i].alpha*/, Buttons[i].red, Buttons[i].green, Buttons[i].blue));
	}
	m_pTM->Draw(m_nCursorID,m_ptCursorPosition.x-50,m_ptCursorPosition.y,m_fCurScaleX,m_fCurScaleY,0,0,0,0,D3DCOLOR_ARGB(m_nAlpha,255,255,255));
}

void CMainMenuState::FadeIn(float fElapsedTime)
{
	m_nMaxVolume = CGame::GetInstance()->GetMusicVolume();

	m_fTimer += fElapsedTime;
	
	if(!m_pWM->IsWavePlaying(m_nSongID))
		m_pWM->Play(m_nSongID);


	if(!m_bAlpha)
		if(m_fTimer > .002f && m_nAlpha < 255)
		{
			m_fTimer = 0;
			m_nAlpha++;

			if(m_nVolume < m_nMaxVolume)
				m_pWM->SetVolume(m_nSongID,m_nVolume++);
			else
				m_pWM->SetVolume(m_nSongID,m_nMaxVolume);

			if(m_nAlpha == 255)
				m_bAlpha = true;
		}
}
void CMainMenuState::FadeOut(float fElapsedTime)
{
	m_fEscTimer += fElapsedTime;
	if(m_fEscTimer > .001)
	{
		m_nAlpha--;
		m_fEscTimer = 0;

		if(m_nVolume >= 0)
			m_pWM->SetVolume(m_nSongID,m_nVolume--);
		else 
			m_pWM->SetVolume(m_nSongID,0);

		if(m_nAlpha == 0)
		{	
			m_pWM->Stop(m_nSongID);
			m_nAlpha = 0;
			m_ptCursorPosition  = Buttons[1].ptPosition;
			m_nCurrentButton = 1;
			m_nVolume = 0;
			CGame::GetInstance()->PushState(m_pToSwitchTo);
			m_pToSwitchTo = NULL;
			m_bAlpha = false;
		}
	}
}

bool CMainMenuState::Parse(char* szFileName)
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
				else if(!strcmp("Song",szName.c_str()))
				{
					char buffer[64];
					string idk = xml->getNodeName();
					strncpy(buffer,idk.c_str(),idk.length());
					buffer[idk.length()] = 0;
					m_nSongID = m_pWM->LoadWave(buffer);

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