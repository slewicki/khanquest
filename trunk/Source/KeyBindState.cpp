#include "KeyBindState.h"
#include "CGame.h"
#include "CGamePlayState.h"
#include "OptionsMenuState.h"

CKeyBindState::CKeyBindState(void)
{
	m_pDI = CSGD_DirectInput::GetInstance();
	m_nSwitchSoundID = -1;
	// Init player keys
	m_dwBoundKeys[CAMERA_LEFT] =		DIK_LEFT;
	m_dwBoundKeys[CAMERA_RIGHT] =		DIK_RIGHT;
	m_dwBoundKeys[CAMERA_UP] =			DIK_UP;
	m_dwBoundKeys[CAMERA_DOWN] =		DIK_DOWN;

	m_szBoundKeys[CAMERA_UP] =				m_pDI->DIKtoString(DIK_UP);
	m_szBoundKeys[CAMERA_DOWN] =				m_pDI->DIKtoString(DIK_DOWN);
	m_szBoundKeys[CAMERA_LEFT] =				m_pDI->DIKtoString(DIK_LEFT);
	m_szBoundKeys[CAMERA_RIGHT] =			m_pDI->DIKtoString(DIK_RIGHT);
	m_bLeaving = false;
}

CKeyBindState::~CKeyBindState(void) {}

void CKeyBindState::Enter(void)
{
	m_pDI = CSGD_DirectInput::GetInstance();
	m_pTM = CSGD_TextureManager::GetInstance();
	m_pWM = CSGD_WaveManager::GetInstance();
	m_nCurrentButton = 1;
	m_bIsBinding = false;
	Buttons = NULL;
	Parse("Resource/KQ_KeyBinding.xml");

	//	Load resources
	m_nFontID = m_pTM->LoadTexture("Resource/KQ_FontLucidiaWhite.png");
	m_cFont.InitBitmapFont(m_nFontID,' ',16,128,128);

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

	m_bAlpha = false;
	m_bLeaving = false;
	m_fTimer = 0;
	m_nAlpha = 0;
	CGame::GetInstance()->SetSongPlay(CITYSELECT);

	m_JoyTimer = 0;
}

void CKeyBindState::Exit(void)
{
	m_pTM->ReleaseTexture(m_nFontID);
	m_pTM->ReleaseTexture(m_nCursorID);
	m_pTM->ReleaseTexture(m_nImageID);
	if(Buttons != NULL)
	delete [] Buttons;
}

bool CKeyBindState::Input(float fElapsedTime)
{	
	for(int i = 0; i < m_nNumButtons;i++)
	{
		if(CGame::GetInstance()->IsMouseInRect(Buttons[i].rToClick))
		{
			CGame::GetInstance()->SetCursorClick();
			if(m_pDI->GetBufferedMouseButton(M_BUTTON_LEFT))
			{
				switch(Buttons[i].Action)
				{
				case KeyBind:
					{
						m_bIsBinding = true;
						m_nCurrentButton++;
						m_ptCursorPosition.x = Buttons[m_nCurrentButton].ptPosition.x;
						m_ptCursorPosition.y = Buttons[m_nCurrentButton].ptPosition.y;
					}break;
				case Back:
					{
						m_bLeaving = true;
					}break;
				}
			}
		}
	}

	//	Change state if user presses ENTER
	if ( (m_pDI->GetBufferedKey(DIK_RETURN) || m_pDI->GetBufferedKey(DIK_NUMPADENTER) ) && !m_pDI->GetKey(DIK_LMENU) && !m_pDI->GetKey(DIK_RMENU) )
	{
			if(Buttons[m_nCurrentButton].Action == KeyBind)
			{
				m_bIsBinding = true;
				m_nCurrentButton++;
				m_ptCursorPosition.x = Buttons[m_nCurrentButton].ptPosition.x;
				m_ptCursorPosition.y = Buttons[m_nCurrentButton].ptPosition.y;
			}
			else if(Buttons[m_nCurrentButton].Action == Back)
			{
				m_bLeaving = true;
			}			
	}
	if(m_bIsBinding)
	{
		if(BindKey(m_pDI->CheckKeysInDIK(),m_nCurrentButton-2))
		{
			m_nCurrentButton++;
			m_ptCursorPosition.x = Buttons[m_nCurrentButton].ptPosition.x;
			m_ptCursorPosition.y = Buttons[m_nCurrentButton].ptPosition.y;
		}
		if(m_nCurrentButton >= m_nNumButtons-1)
			m_bIsBinding = false;
	}

	if (!m_bIsBinding && m_pDI->GetBufferedKey(DIK_UP))
	{
		m_nCurrentButton--;
		if(m_nCurrentButton < 1)
			m_nCurrentButton = m_nNumButtons-1;		
		
		m_ptCursorPosition.x = Buttons[m_nCurrentButton].ptPosition.x;
		m_ptCursorPosition.y = Buttons[m_nCurrentButton].ptPosition.y;
	}
	if (!m_bIsBinding && m_pDI->GetBufferedKey(DIK_DOWN))
	{
		m_nCurrentButton++;
		if(m_nCurrentButton > m_nNumButtons-1)
			m_nCurrentButton = 1;
		m_ptCursorPosition.x = Buttons[m_nCurrentButton].ptPosition.x;
		m_ptCursorPosition.y = Buttons[m_nCurrentButton].ptPosition.y;
	}
	return true;
}

void CKeyBindState::Update(float fElapsedTime)
{
	if(!m_bLeaving)
		FadeIn(fElapsedTime);
	else
		FadeOut(fElapsedTime);
}

void CKeyBindState::Render(float fElapsedTime)
{
	CSGD_Direct3D::GetInstance()->Clear(0,0,0);
	m_pTM->Draw(m_nImageID,m_ptImageLoc.x,m_ptImageLoc.y);

	for(int i = 0; i < m_nNumButtons; i++)
	{
		m_cFont.DrawTextA(Buttons[i].Text,Buttons[i].ptPosition.x, Buttons[i].ptPosition.y, Buttons[i].fscalex, Buttons[i].fscaley,
			D3DCOLOR_ARGB(m_nAlpha/*Buttons[i].alpha*/, Buttons[i].red, Buttons[i].green, Buttons[i].blue));

		if(Buttons[i+2].Action == Up || Buttons[i+2].Action == Left || Buttons[i+2].Action == Right || Buttons[i+2].Action == Down)
			m_cFont.DrawTextA(m_pDI->DIKtoString(m_dwBoundKeys[i]),Buttons[i+2].ptPosition.x + 150, Buttons[i+2].ptPosition.y,Buttons[i+2].fscalex,Buttons[i+2].fscaley,
				D3DCOLOR_ARGB(m_nAlpha,Buttons[i+2].red,Buttons[i+2].green,Buttons[i+2].blue));
	}
	m_pTM->Draw(m_nCursorID,m_ptCursorPosition.x-50,m_ptCursorPosition.y,m_fCurScaleX,m_fCurScaleY,0,0,0,0,D3DCOLOR_ARGB(m_nAlpha,255,255,255));
}

bool CKeyBindState::BindKey(DWORD dwDIK, int nID)
{
	m_pDI = CSGD_DirectInput::GetInstance();
	
	for (int i = 0; i < nID; i++)
	{
		if(i != nID && dwDIK == m_dwBoundKeys[i])
			return false;
	}

	if(m_pDI->DIKtoString(dwDIK) == "Invalid")
		return false;
	m_szBoundKeys[nID] = m_pDI->DIKtoString(dwDIK);
	m_dwBoundKeys[nID] = dwDIK;
	return true;
}


void CKeyBindState::FadeIn(float fElapsedTime)
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
void CKeyBindState::FadeOut(float fElapsedTime)
{
	m_fEscTimer += fElapsedTime;
	if(m_fEscTimer > .0001)
	{
		m_nAlpha-=5;
		m_fEscTimer = 0;

		if(m_nAlpha == 0)
		{	
			m_nAlpha = 0;
			m_ptCursorPosition  = Buttons[1].ptPosition;
			m_nCurrentButton = 1;
			CGame::GetInstance()->PopCurrentState();
			COptionsMenuState::GetInstance()->SetPause(false);
			m_bAlpha = false;
		}
	}
}


bool CKeyBindState::Parse(char* szFileName)
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
					Buttons[nCounter].rToClick.left = Buttons[nCounter].ptPosition.x;
				}
				else if(!strcmp("ButtonPositionY",szName.c_str()))
				{
					if(nCounter < m_nNumButtons)
						Buttons[nCounter].ptPosition.y = atoi(xml->getNodeName());
					Buttons[nCounter].rToClick.top = Buttons[nCounter].ptPosition.y;
				}
				else if(!strcmp("ButtonRight",szName.c_str()))
				{
					if(nCounter < m_nNumButtons)
						Buttons[nCounter].rToClick.right = atoi(xml->getNodeName());
				}
				else if(!strcmp("ButtonBottom",szName.c_str()))
				{
					if(nCounter < m_nNumButtons)
						Buttons[nCounter].rToClick.bottom = atoi(xml->getNodeName());
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