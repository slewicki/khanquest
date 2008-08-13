#include "OptionsMenuState.h"
#include "MainMenuState.h"
#include "CGame.h"
#include "CSGD_DirectInput.h"
#include "CSGD_TextureManager.h"
#include "CSGD_WaveManager.h"
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
	m_nClick = m_pWM->LoadWave("Resource/KQ_Chainsaw.wav");

	m_pWM->SetVolume(m_nSongID,CGame::GetInstance()->GetMusicVolume());
	m_pWM->SetVolume(m_nClick,CGame::GetInstance()->GetSFXVolume());

	char image[128];
	strncpy(image,m_szImageFile.c_str(),m_szImageFile.length());
	image[m_szImageFile.length()]= 0;
	m_nImageID = m_pTM->LoadTexture(image);

	strncpy(image,m_szCursorName.c_str(),m_szCursorName.length());
	image[m_szCursorName.length()] = 0;
	m_nCursorID = m_pTM->LoadTexture(image);

	m_ptCursorPosition.x = Buttons[1].ptPosition.x - 50;
	m_ptCursorPosition.y = Buttons[1].ptPosition.y;
	m_nCurrentButton = 1;
}
void COptionsMenuState::Exit()
{
	if(m_pWM->IsWavePlaying(m_nClick))
		m_pWM->Stop(m_nClick);
	if(m_pWM->IsWavePlaying(m_nSongID))
		m_pWM->Stop(m_nSongID);	
	m_pWM->UnloadWave(m_nClick);
	m_pWM->UnloadWave(m_nSongID);

}

bool COptionsMenuState::Input(float fElapsedTime)
{
	if(m_pDI->GetBufferedKey(DIK_UP))
	{
		m_pWM->Stop(m_nSongID);
		m_nCurrentButton--;
		m_ptCursorPosition.y = Buttons[m_nCurrentButton].ptPosition.y;
		if(m_ptCursorPosition.y < Buttons[1].ptPosition.y)
		{
			m_ptCursorPosition.y = Buttons[m_nNumButtons-1].ptPosition.y;
			m_nCurrentButton = m_nNumButtons-1;
		}
		if(Buttons[m_nCurrentButton].Action == MUSIC)
			m_pWM->Play(m_nSongID);
		
	}
	if(m_pDI->GetBufferedKey(DIK_DOWN))
	{
		m_pWM->Stop(m_nSongID);
		m_nCurrentButton++;
		m_ptCursorPosition.y = Buttons[m_nCurrentButton].ptPosition.y;
		if(m_nCurrentButton > m_nNumButtons-1)
		{
			m_ptCursorPosition.y = Buttons[1].ptPosition.y;
			m_nCurrentButton = 1;
		}
		if(Buttons[m_nCurrentButton].Action == MUSIC)
			m_pWM->Play(m_nSongID);
		
	}
	if(m_pDI->GetBufferedKey(DIK_LEFT))
	{
		if(Buttons[m_nCurrentButton].Action == MUSIC)
		{
			CGame::GetInstance()->SetMusicVolume(CGame::GetInstance()->GetMusicVolume() - 1);
			if(CGame::GetInstance()->GetMusicVolume() < 0)
				CGame::GetInstance()->SetMusicVolume(0);
		}
		if(Buttons[m_nCurrentButton].Action == SFX)
		{
			CGame::GetInstance()->SetSFXVolume(CGame::GetInstance()->GetSFXVolume() - 1);
			if(CGame::GetInstance()->GetSFXVolume() < 0)
				CGame::GetInstance()->SetSFXVolume(0);
	
			m_pWM->Play(m_nClick);
		}
	}
	if(m_pDI->GetBufferedKey(DIK_RIGHT))
	{
		if(Buttons[m_nCurrentButton].Action == MUSIC)
		{
			CGame::GetInstance()->SetMusicVolume(CGame::GetInstance()->GetMusicVolume() + 1);
			if(CGame::GetInstance()->GetMusicVolume() > 100)
				CGame::GetInstance()->SetMusicVolume(100);
		}
		if(Buttons[m_nCurrentButton].Action == SFX)
		{
			CGame::GetInstance()->SetSFXVolume(CGame::GetInstance()->GetSFXVolume() + 1);
			if(CGame::GetInstance()->GetSFXVolume() > 100)
				CGame::GetInstance()->SetSFXVolume(100);
			m_pWM->Play(m_nClick);
		}
	}
	if(m_pDI->GetBufferedKey(DIK_RETURN))
	{
		if(Buttons[m_nCurrentButton].Action == KEYBIND)
		{
			m_pWM->Play(m_nClick);
			//Switch States;
		}
		if(Buttons[m_nCurrentButton].Action == BACK)
		{
			m_pWM->Play(m_nClick);
			CGame::GetInstance()->PopCurrentState();
			CMainMenuState::GetInstance()->SetPause(false);
		}		
	}
	return true;
}

void COptionsMenuState::Render(float fElapsedTime)
{
	CSGD_Direct3D::GetInstance()->Clear(128,60,0);
	//m_pTM->Draw(m_nImageID,m_ptImageLoc.x,m_ptImageLoc.y);

	for(int i = 0; i < m_nNumButtons; i++)
	{
		if(Buttons[i].Action == MUSIC)
		{
			char MusicVolume[64];
			sprintf(MusicVolume,"%s %d",Buttons[i].Text.c_str(),CGame::GetInstance()->GetMusicVolume());
			m_BF.DrawTextA(MusicVolume,Buttons[i].ptPosition.x,Buttons[i].ptPosition.y,Buttons[i].fscalex, Buttons[i].fscaley,
						   D3DCOLOR_ARGB(Buttons[i].alpha, Buttons[i].red, Buttons[i].green, Buttons[i].blue));
		}
		else if(Buttons[i].Action == SFX)
		{
			char SFXVolume[128];
			sprintf(SFXVolume,"%s %d",Buttons[i].Text.c_str(),CGame::GetInstance()->GetSFXVolume());
			m_BF.DrawTextA(SFXVolume,Buttons[i].ptPosition.x,Buttons[i].ptPosition.y,Buttons[i].fscalex, Buttons[i].fscaley,
						   D3DCOLOR_ARGB(Buttons[i].alpha, Buttons[i].red, Buttons[i].green, Buttons[i].blue));
		}
		else
		{
			m_BF.DrawTextA(Buttons[i].Text,Buttons[i].ptPosition.x, Buttons[i].ptPosition.y, Buttons[i].fscalex, Buttons[i].fscaley,
						   D3DCOLOR_ARGB(Buttons[i].alpha, Buttons[i].red, Buttons[i].green, Buttons[i].blue));
		}	
	}
	m_pTM->Draw(m_nCursorID,m_ptCursorPosition.x,m_ptCursorPosition.y,m_fCurScaleX,m_fCurScaleY);
}
void COptionsMenuState::Update(float fElapsedTime)
{
	m_pWM->SetVolume(m_nClick,CGame::GetInstance()->GetSFXVolume());
	m_pWM->SetVolume(m_nSongID,CGame::GetInstance()->GetMusicVolume());
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

