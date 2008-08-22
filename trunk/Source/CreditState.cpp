#include "CreditState.h"
#include "CGame.h"
#include "OptionsMenuState.h"
#include "CSGD_DirectInput.h"
#include "CSGD_TextureManager.h"
#include "CSGD_WaveManager.h"
#include "irrXML.h"
#include <fstream>
#include <iostream>
using namespace std;
using namespace irr;
using namespace io;

CCreditState::CCreditState(void)
{

}
CCreditState::~CCreditState(void)
{

}
void CCreditState::Enter()
{
	m_pDI = CSGD_DirectInput::GetInstance();
	m_pTM = CSGD_TextureManager::GetInstance();
	m_pWM = CSGD_WaveManager::GetInstance();
	int nFontID = m_pTM->LoadTexture("Resource/KQ_FontLucidiaWhite.png");
	m_cFont.InitBitmapFont(nFontID,' ',16,128,128);
	Parse("Resource/KQ_Credits.xml");
	m_nYpos = 600;
	CGame::GetInstance()->SetSongPlay(CREDITS);

}
void CCreditState::Exit()
{
	CGame::GetInstance()->SetSongPlay(CITYSELECT);
	
}

bool CCreditState::Input(float fElapsedTime)
{
	if(m_pDI->GetBufferedKey(DIK_RETURN) || m_pDI->GetBufferedKey(DIK_NUMPADENTER) || m_pDI->GetBufferedJoyButton(JOYSTICK_R2) || m_pDI->GetBufferedJoyButton(JOYSTICK_X)))
	{
		COptionsMenuState::GetInstance()->SetPause(false);
		CGame::GetInstance()->PopCurrentState();
	}
	return true;
}

void CCreditState::Update(float fElapsedTime)
{
	m_fTimer += fElapsedTime;
	if(m_fTimer >= .002)
	{
		m_nYpos -= 1;
		m_fTimer = 0;
	}
}
void CCreditState::Render(float fElapsedTime)
{
	for( int i = 0; i < m_nNumTitle;++i)
	{
		if(m_szTitles[i] == "Programmers")
			m_cFont.DrawTextA(m_szTitles[i],50,m_nYpos,.5f,.5f);
		if(m_szTitles[i] == "Resources")
			m_cFont.DrawTextA(m_szTitles[i],50,m_nYpos + 50 + (m_nNumCoders*60),.5f,.5f);
		if(m_szTitles[i] == "Instructors")
			m_cFont.DrawTextA(m_szTitles[i],50,m_nYpos + 100 + (m_nNumCoders*60) + (m_nNumResources*60),.5f,.5f);

		if(m_szTitles[i] == "Programmers")
			for(int j= 0; j < m_nNumCoders;++j)	
				m_cFont.DrawTextA(m_szCoders[j],100,m_nYpos + 50 +(i+(j*60)),.3f,.3f);
		if(m_szTitles[i] == "Resources")
			for(int j=0; j < m_nNumResources;++j)
				m_cFont.DrawTextA(m_szResources[j],100,m_nYpos + 100 + (m_nNumCoders*60) + (i+(j*60)),.3f,.3f);
		if(m_szTitles[i] == "Instructors")
			for(int j = 0; j < m_nNumInstructors;++j)
				m_cFont.DrawTextA(m_szInstructors[j],100,m_nYpos + 150 + (m_nNumResources*60) + (m_nNumCoders*60) + (i+(j*60)),.3f,.3f);		
	}
}

bool CCreditState::Parse(char* szFileName)
{
	//Function variables - do not change
	string szName, szTempBool;
	IrrXMLReader* xml = createIrrXMLReader(szFileName);
	int nCoderCounter = 0;
	int nInstructorCounter = 0;
	int nResourceCounter = 0;
	int nTitleCounter = 0;
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
				if (!strcmp("Code", szName.c_str()))
				{
					m_szCoders[nCoderCounter] = xml->getNodeName();
					nCoderCounter++;
				}
				if(!strcmp("NumCoders",szName.c_str()))
				{
					m_nNumCoders = atoi(xml->getNodeName());
				}

				if(!strcmp("NumInstructors",szName.c_str()))
				{
					m_nNumInstructors = atoi(xml->getNodeName());
				}
				if (!strcmp("Instructor",szName.c_str()))
				{
					m_szInstructors[nInstructorCounter] = xml->getNodeName();
					nInstructorCounter++;
				}
				if(!strcmp("NumResources",szName.c_str()))
				{
					m_nNumResources = atoi(xml->getNodeName());
				}
				if(!strcmp("Resource",szName.c_str()))
				{
					m_szResources[nResourceCounter] = xml->getNodeName();
					nResourceCounter++;
				}
				if (!strcmp("Title",szName.c_str()))
				{
					m_szTitles[nTitleCounter] = xml->getNodeName();
					nTitleCounter++;
				}
				if(!strcmp("NumTitles",szName.c_str()))
				{
					m_nNumTitle = atoi(xml->getNodeName());
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
