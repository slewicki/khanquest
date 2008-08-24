//////////////////////////////////////////////////////////
//	File:	"CUnitCreationState.cpp"
//
//	Author:	Sean Hamstra (SH)
//
//	Purpose: To contain functionality of the unit creation state
//////////////////////////////////////////////////////////

#include "CUnitCreationState.h"
#include "CWorldMapState.h"
#include "UpgradeMenuState.h"
#include "CGamePlayState.h"
#include "CGame.h"
#include "CFactory.h"


CUnitCreationState::CUnitCreationState(void)
{
	PROFILE("CUnitCreationState::CUnitCreationState()");
	m_nPlusButtonID = -1;
	m_nMinusButtonID = -1;
	m_nTotalUnits = 0;
	m_nNumUnits[0] = 0;
	m_nBackgroundID = -1;
	m_nFoodTotal = 0;
	m_nScrollButtonID = -1;
	STOP("CUnitCreationState::CUnitCreationState()");
}


CUnitCreationState::~CUnitCreationState(void)
{
	
}


void CUnitCreationState::Enter(void)
{
	PROFILE("CUnitCreationState::Enter()");
	// Get Our Managers Ready
	m_fTimer = 0.f;
	

	// Clear out our unit counts
	m_nTotalUnits = 0;
	for (int i = 0; i < 6; i++)
	{
		m_nNumUnits[i] = 0;
	}

	m_pTM = CSGD_TextureManager::GetInstance();
	m_pWM = CSGD_WaveManager::GetInstance();
	m_pDI = CSGD_DirectInput::GetInstance();
	m_pSelectedCity = CGame::GetInstance()->GetSelectedCity();

	m_nFoodTotal = CGame::GetInstance()->GetTotalFoodTribute();

	// Replace with animations
	//m_nUnitID[INFANTRY] = m_pTM->LoadTexture("Resource/KQ_Infantry.png");
	//m_nUnitID[CAVALRY] = m_pTM->LoadTexture("Resource/KQ_Cavalry.png");
	//m_nUnitID[CAVALRY_ARCHER] = m_pTM->LoadTexture("Resource/KQ_CavalryArcher.png");
	//m_nUnitID[AXMEN] = m_pTM->LoadTexture("Resource/KQ_Axmen.png");
	//m_nUnitID[ARCHER] = m_pTM->LoadTexture("Resource/KQ_Archer.png");
	//m_nUnitID[WAR_ELEPHANT] = m_pTM->LoadTexture("Resource/KQ_WarElephant.png");
	m_nIconID = m_pTM->LoadTexture("Resource/KQ_UnitIcons.png");
	
	// Register any Events with the CUnitCreationState
	m_nLucidiaWhiteID = m_pTM->LoadTexture("Resource/KQ_FontLucidiaWhite.png");
	m_nPlusButtonID = m_pTM->LoadTexture("Resource/KQ_PlusButton.png");
	m_nMinusButtonID = m_pTM->LoadTexture("Resource/KQ_MinusButton.png");
	m_nBackgroundID = m_pTM->LoadTexture("Resource/KQ_PageBkg3.png");
	m_nScrollButtonID = m_pTM->LoadTexture("Resource/KQ_ScrollButton.png");

	m_nClick =  m_pWM->LoadWave("Resource/KQ_Click.wav");
	m_pWM->SetVolume(m_nClick,CGame::GetInstance()->GetSFXVolume());

	m_cFont.InitBitmapFont(m_nLucidiaWhiteID, ' ', 16, 128, 128);
	m_pPlayerUnitInfo = CGame::GetInstance()->GetPlayerUnitInfo();

	m_nUnitCosts[UNIT_INFANTRY] = 65;
	m_nUnitCosts[UNIT_CAVALRY] = 150;
	m_nUnitCosts[UNIT_CAVALRY_ARCHER] = 150;
	m_nUnitCosts[UNIT_AXMEN] = 65;
	m_nUnitCosts[UNIT_ARCHER] = 60;
	m_nUnitCosts[UNIT_WAR_ELEPHANT] = 300;

	for (int i = 0; i < 3; i++)
	{
		this->m_rMinusButtons[i].left = 10;
		this->m_rMinusButtons[i].top = 60 + (i*170);
		this->m_rMinusButtons[i].right = m_rMinusButtons[i].left + 35;
		this->m_rMinusButtons[i].bottom = m_rMinusButtons[i].top + 35;

		this->m_rPlusButtons[i].left = 150;
		this->m_rPlusButtons[i].top = 60 + (i*170);
		this->m_rPlusButtons[i].right = m_rPlusButtons[i].left + 35;
		this->m_rPlusButtons[i].bottom = m_rPlusButtons[i].top + 35;
	}
	if(CGame::GetInstance()->IsAxmenUnlocked())
	{
		this->m_rMinusButtons[3].left = 400;
		this->m_rMinusButtons[3].top = 60;
		this->m_rMinusButtons[3].right = m_rMinusButtons[3].left + 35;
		this->m_rMinusButtons[3].bottom = m_rMinusButtons[3].top + 35;

		this->m_rPlusButtons[3].left = 540;
		this->m_rPlusButtons[3].top = 60 ;
		this->m_rPlusButtons[3].right = m_rPlusButtons[3].left + 35;
		this->m_rPlusButtons[3].bottom = m_rPlusButtons[3].top + 35;
	}
	if(CGame::GetInstance()->IsArcherUnlocked())
	{
		this->m_rMinusButtons[4].left = 400;
		this->m_rMinusButtons[4].top = 60 + 170;
		this->m_rMinusButtons[4].right = m_rMinusButtons[4].left + 35;
		this->m_rMinusButtons[4].bottom = m_rMinusButtons[4].top + 35;

		this->m_rPlusButtons[4].left = 540;
		this->m_rPlusButtons[4].top = 60 + 170;
		this->m_rPlusButtons[4].right = m_rPlusButtons[4].left + 35;
		this->m_rPlusButtons[4].bottom = m_rPlusButtons[4].top + 35;
	}
	if(CGame::GetInstance()->IsWarElephantUnlocked())
	{
		this->m_rMinusButtons[5].left = 400;
		this->m_rMinusButtons[5].top = 60 + (2*170);
		this->m_rMinusButtons[5].right = m_rMinusButtons[5].left + 35;
		this->m_rMinusButtons[5].bottom = m_rMinusButtons[5].top + 35;

		this->m_rPlusButtons[5].left = 540;
		this->m_rPlusButtons[5].top = 60 + (2*170);
		this->m_rPlusButtons[5].right = m_rPlusButtons[5].left + 35;
		this->m_rPlusButtons[5].bottom = m_rPlusButtons[5].top + 35;
	}

	m_rUpgradeButton.left	= 215;
	m_rUpgradeButton.right	= 345;
	m_rUpgradeButton.top	= 515;
	m_rUpgradeButton.bottom	= 575;

	m_rAttackButton.left = 415;
	m_rAttackButton.top = 515;
	m_rAttackButton.right = 545;
	m_rAttackButton.bottom = 575;	
	
	m_rBackButton.left = 615;
	m_rBackButton.top = 515;
	m_rBackButton.right = 745;
	m_rBackButton.bottom = 575;

	CGame::GetInstance()->SetSongPlay(CITYSELECT);

	m_bPaused = false;
	m_fJoyTimer = 0;
	if(CGame::GetInstance()->GetTutorialMode())
	{
		m_bTutorial = true;
		m_rTutorial.top = 400;
		m_rTutorial.left = 350;
		m_rTutorial.bottom = m_rTutorial.top + 64;
		m_rTutorial.right = m_rTutorial.left + 128;
	}
	else
		m_bTutorial = false;
		STOP("CUnitCreationState::Enter()");
}

void CUnitCreationState::Exit(void)
{
	PROFILE("CUnitCreationState::Exit()");
	if(m_pWM->IsWavePlaying(m_nClick))
		m_pWM->Stop(m_nClick);
	m_pWM->UnloadWave(m_nClick);

	m_pTM->ReleaseTexture(m_nPlusButtonID);
	m_pTM->ReleaseTexture(m_nMinusButtonID);
	m_pTM->ReleaseTexture(m_nLucidiaWhiteID);
	m_pTM->ReleaseTexture(m_nBackgroundID);
	m_pTM->ReleaseTexture(m_nScrollButtonID);
	m_pTM->ReleaseTexture(m_nIconID);

	for (int i = 0; i < m_nNumUnits[UNIT_INFANTRY] ; i++)
		CFactory::CreatePlayerUnit(UNIT_INFANTRY);
	for (int i = 0; i < m_nNumUnits[UNIT_CAVALRY] ; i++)
		CFactory::CreatePlayerUnit(UNIT_CAVALRY);
	for (int i = 0; i < m_nNumUnits[UNIT_CAVALRY_ARCHER] ; i++)
		CFactory::CreatePlayerUnit(UNIT_CAVALRY_ARCHER);
	for (int i = 0; i < m_nNumUnits[UNIT_AXMEN] ; i++)
		CFactory::CreatePlayerUnit(UNIT_AXMEN);
	for (int i = 0; i < m_nNumUnits[UNIT_ARCHER] ; i++)
		CFactory::CreatePlayerUnit(UNIT_ARCHER);
	for (int i = 0; i < m_nNumUnits[UNIT_WAR_ELEPHANT] ; i++)
		CFactory::CreatePlayerUnit(UNIT_WAR_ELEPHANT);
	STOP("CUnitCreationState::Exit()");

}

bool CUnitCreationState::Input(float fElapsedTime)
{
	PROFILE("CUnitCreationState::Input(float)");
	if(m_bPaused)
	{
		STOP("CUnitCreationState::Input(float)");
		return true;
	}
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

	for (int i = 0; i < 6; i++)
	{
		if(CGame::GetInstance()->IsMouseInRect(m_rMinusButtons[i]))
		{
			// Change cursor to click icon
			CGame::GetInstance()->SetCursorClick();
			if(m_pDI->GetBufferedMouseButton(M_BUTTON_LEFT) || m_pDI->GetBufferedJoyButton(JOYSTICK_X))
			{
				if(m_nNumUnits[i]>0)
				{
					m_pWM->Play(m_nClick);

					m_nNumUnits[i]--;
					m_nTotalUnits--;
					m_nFoodTotal += m_nUnitCosts[i];
				}
			}
		}
		if(CGame::GetInstance()->IsMouseInRect(m_rPlusButtons[i]))
		{
			CGame::GetInstance()->SetCursorClick();
			if((m_nFoodTotal > 0 && m_nTotalUnits < MAX_UNITS && m_pDI->GetBufferedMouseButton(M_BUTTON_LEFT)) || (m_nFoodTotal > 0 && m_nTotalUnits < MAX_UNITS && m_pDI->GetBufferedJoyButton(JOYSTICK_X)))
			{
				
				if(m_nFoodTotal >= m_nUnitCosts[i])
				{
					m_pWM->Play(m_nClick);

					m_nFoodTotal -= m_nUnitCosts[i];
					m_nNumUnits[i]++;
					m_nTotalUnits++;
				}

			}
		}
	}
	if(m_nTotalUnits && CGame::GetInstance()->IsMouseInRect(m_rAttackButton))
	{
		CGame::GetInstance()->SetCursorClick();
		if(m_pDI->GetBufferedMouseButton(M_BUTTON_LEFT) || m_pDI->GetBufferedJoyButton(JOYSTICK_X))
		{
			
			if(m_nTotalUnits > 0)
			{
				m_pWM->Play(m_nClick);

				// Ask if they are sure...?
				
				// Figure out map and enemy units (call factory enemycreates)
				// Change to on GamePlayState
				CGame::GetInstance()->ChangeState(CGamePlayState::GetInstance());
				
			}
		}
	}
	if(CGame::GetInstance()->IsMouseInRect(m_rUpgradeButton))
	{
		CGame::GetInstance()->SetCursorClick();
		if(m_pDI->GetBufferedMouseButton(M_BUTTON_LEFT) || m_pDI->GetBufferedJoyButton(JOYSTICK_X))
		{
			
			
				m_pWM->Play(m_nClick);

				
				m_bPaused = true;
				CGame::GetInstance()->PushState(CUpgradeMenuState::GetInstance());
				
			
		}
	}
	if(CGame::GetInstance()->IsMouseInRect(m_rBackButton))
	{
		CGame::GetInstance()->SetCursorClick();
		if(m_pDI->GetBufferedMouseButton(M_BUTTON_LEFT) || m_pDI->GetBufferedJoyButton(JOYSTICK_X))
		{
			m_pWM->Play(m_nClick);
			// Go back to the map
			for (int i = 0; i < 6; ++i)
			{
			m_nNumUnits[i] = 0;
			}
			m_nTotalUnits =  0;
			CGame::GetInstance()->ChangeState(CWorldMapState::GetInstance());
		}
	}
}
else 
	{
		if(CGame::GetInstance()->IsMouseInRect(m_rTutorial))
		{
			CGame::GetInstance()->SetCursorClick();
			if(m_pDI->GetBufferedMouseButton(M_BUTTON_LEFT)|| m_pDI->GetBufferedJoyButton(JOYSTICK_X))
			{
				m_pWM->Play(m_nClick);
				m_bTutorial = false;
			}
		}
	}
		STOP("CUnitCreationState::Input(float)");
	return true;
}

void CUnitCreationState::Update(float fElapsedTime)
{
	
}

void CUnitCreationState::Render(float fElapsedTime)
{
	PROFILE("CUnitCreationState::Render(float)");
	m_pTM->Draw(m_nBackgroundID, -20, -10);


	RECT InfIcon;
	InfIcon.top = 0; InfIcon.left = 200; InfIcon.right = 250; InfIcon.bottom = 50;
	m_pTM->Draw(m_nIconID,75,50,1.f,1.f,&InfIcon);


	RECT CavIcon;
	CavIcon.top = 100; CavIcon.left = 0; CavIcon.right = 50; CavIcon.bottom = 150;
	m_pTM->Draw(m_nIconID,75,220,1.f,1.f,&CavIcon);
	

	RECT CAIcon;
	CAIcon.top = 100; CAIcon.left = 200; CAIcon.right = 250; CAIcon.bottom = 150;
	m_pTM->Draw(m_nIconID,75,390,1.f,1.f,&CAIcon);
	

	RECT AXIcon;
	AXIcon.top = 0; AXIcon.left = 0; AXIcon.right = 50; AXIcon.bottom = 50;
	m_pTM->Draw(m_nIconID,465,50,1.f,1.f,&AXIcon);


	RECT ArcIcon;
	ArcIcon.top = 0; ArcIcon.left = 100; ArcIcon.right = 150; ArcIcon.bottom = 50;
	m_pTM->Draw(m_nIconID,465,220,1.f,1.f,&ArcIcon);


	RECT EleIcon;
	EleIcon.top = 100; EleIcon.left = 100; EleIcon.right = 150; EleIcon.bottom = 150;
	m_pTM->Draw(m_nIconID,465,390,1.f,1.f,&EleIcon);
	if(!m_bTutorial)
	{
	int nPosY = 40;
	int nPosX = 200;
	for (int i = 0; i < 6; i++)
	{
		string szType;
		switch(i)
		{
		case 0:
			szType = "Infantry";
			break;
		case 1:
			szType = "Cavalry";
			break;
		case 2:
			szType = "Cavalry Archer";
			break;
		case 3:
			szType = "Axmen";
			nPosX = 590;
			nPosY = 40;
			break;
		case 4:
			nPosX = 590;
			szType = "Archer";
			break;
		case 5:
			nPosX = 590;
			szType = "War Elephant";
			break;
		}
		
		m_cFont.DrawTextA(szType + "/HP: " + IntToString(m_pPlayerUnitInfo[i].GetMaxHP()) +
		"    Attack: " + IntToString(m_pPlayerUnitInfo[i].GetAttackPower()) +
		"/Attack Speed: " + FloatToString(m_pPlayerUnitInfo[i].GetAttackSpeed()) +
		"/Range: " + IntToString(m_pPlayerUnitInfo[i].GetRange()) +
		"/Movement: " + FloatToString(m_pPlayerUnitInfo[i].GetSpeed()), nPosX, nPosY, .15f, .15f, D3DCOLOR_ARGB(255, 0, 0, 0));
		m_cFont.DrawTextA("Food: " + IntToString(m_nUnitCosts[i]), nPosX, nPosY - 25, .15f, .15f, D3DCOLOR_ARGB(255, 255, 0, 0));
		
		nPosY += 170;
	}

		
	for (int i = 0; i < 3; i++)
	{
		m_pTM->Draw(m_nMinusButtonID, m_rMinusButtons[i].left, m_rMinusButtons[i].top, .5f, .5f);
		
		// Draw unit picture or animation

		m_cFont.DrawTextA(IntToString(m_nNumUnits[i]), m_rMinusButtons[i].right + 40, m_rMinusButtons[i].bottom, .25f, .25f, D3DCOLOR_ARGB(255, 0, 0, 0));
		
		m_pTM->Draw(m_nPlusButtonID, m_rPlusButtons[i].left, m_rPlusButtons[i].top, .5f, .5f);
	}
	if(CGame::GetInstance()->IsAxmenUnlocked())
	{
		m_pTM->Draw(m_nMinusButtonID, m_rMinusButtons[3].left, m_rMinusButtons[3].top, .5f, .5f);
		// Draw unit picture or animation
		m_cFont.DrawTextA(IntToString(m_nNumUnits[3]), m_rMinusButtons[3].right + 40, m_rMinusButtons[3].bottom, .25f, .25f, D3DCOLOR_ARGB(255, 0, 0, 0));
		m_pTM->Draw(m_nPlusButtonID, m_rPlusButtons[3].left, m_rPlusButtons[3].top, .5f, .5f);
	}
	else
		m_cFont.DrawTextA("Locked", 400+40, 70, .25f, .25f, D3DCOLOR_ARGB(255, 150, 0,0));
	if(CGame::GetInstance()->IsArcherUnlocked())
	{
		m_pTM->Draw(m_nMinusButtonID, m_rMinusButtons[4].left, m_rMinusButtons[4].top, .5f, .5f);
		// Draw unit picture or animation
		m_cFont.DrawTextA(IntToString(m_nNumUnits[4]), m_rMinusButtons[4].right + 40, m_rMinusButtons[4].bottom, .25f, .25f, D3DCOLOR_ARGB(255, 0, 0, 0));
		m_pTM->Draw(m_nPlusButtonID, m_rPlusButtons[4].left, m_rPlusButtons[4].top, .5f, .5f);
	}
	else
		m_cFont.DrawTextA("Locked", 400+40, 240, .25f, .25f, D3DCOLOR_ARGB(255, 150, 0,0));

	if(CGame::GetInstance()->IsWarElephantUnlocked())
	{
		m_pTM->Draw(m_nMinusButtonID, m_rMinusButtons[5].left, m_rMinusButtons[5].top, .5f, .5f);
		// Draw unit picture or animation
		m_cFont.DrawTextA(IntToString(m_nNumUnits[5]), m_rMinusButtons[5].right + 40, m_rMinusButtons[5].bottom, .25f, .25f, D3DCOLOR_ARGB(255, 0, 0, 0));
		m_pTM->Draw(m_nPlusButtonID, m_rPlusButtons[5].left, m_rPlusButtons[5].top, .5f, .5f);
	}
	else
		m_cFont.DrawTextA("Locked", 400+40, 410, .25f, .25f, D3DCOLOR_ARGB(255, 150, 0,0));
	
	m_cFont.DrawTextA("Food:        " + IntToString(m_nFoodTotal), 10, 500, .2f, .2f, D3DCOLOR_ARGB(255, 255, 0, 0));
	m_cFont.DrawTextA("Army Size: " + IntToString(m_nTotalUnits), 10, 530, .2f, .2f, D3DCOLOR_ARGB(255, 0, 0, 255));
	m_cFont.DrawTextA("Max Size:   " + IntToString(MAX_UNITS), 10, 560, .2f, .2f, D3DCOLOR_ARGB(255, 0, 0, 0));

	m_pTM->Draw(m_nScrollButtonID, m_rUpgradeButton.left, m_rUpgradeButton.top, .4f, .3f);
	m_cFont.DrawTextA("Upgrades", m_rUpgradeButton.left+25, m_rUpgradeButton.top+24, .15f, .15f, D3DCOLOR_ARGB(255, 255, 0, 0));
	
	m_pTM->Draw(m_nScrollButtonID, m_rAttackButton.left, m_rAttackButton.top, .4f, .3f);
	m_cFont.DrawTextA("Attack!", m_rAttackButton.left+30, m_rAttackButton.top+24, .2f, .2f, D3DCOLOR_ARGB(255, 255, 0, 0));

	if(!m_nTotalUnits)
	{
		m_pTM->Draw(m_nScrollButtonID, m_rAttackButton.left, m_rAttackButton.top, .4f, .3f, 0, 0, 0, 0, D3DCOLOR_ARGB(150, 50, 50, 50));
	}
	m_pTM->Draw(m_nScrollButtonID, m_rBackButton.left, m_rBackButton.top, .4f, .3f);
	m_cFont.DrawTextA("Back", m_rBackButton.left+40, m_rBackButton.top+24, .2f, .2f, D3DCOLOR_ARGB(255, 255, 0, 0));
	}
	else if(CGame::GetInstance()->GetTutorialMode())
	{
		RECT toDraw; toDraw.top = 0; toDraw.left = 0; toDraw.right = 578; toDraw.bottom = 495;
		int nImage = m_pTM->LoadTexture("Resource/KQ_TutorialBox.png");
		m_pTM->Draw(nImage,0,2,1.4f,1.2f,&toDraw);
		m_pTM->Draw(m_nScrollButtonID,325,400,.4f,.3f);
		m_cFont.DrawTextA("Accept",350,425,.2f,.2f,D3DCOLOR_ARGB(255,255,0,0));
		m_cFont.DrawTextA("Tutorial",315,15,.4f,.4f,D3DCOLOR_ARGB(255,255,0,0));
		m_cFont.DrawTextA("This next screen will let you purchase units./Food is used to purchase units. Your current /food total is listed in the bottom left corner /of your screen. The cost of each unit /and their stats are all displayed.//Weigh your options and construct an army!",80,100,.25f,.25f,D3DCOLOR_ARGB(255,0,0,0));
	}	
	
	STOP("CUnitCreationState::Render(float)");	
}

string CUnitCreationState::IntToString(int nNum)
{
	PROFILE("CUnitCreationState::IntToString(int)");
	char szNumVal[10];
	itoa(nNum, szNumVal, 10);
	string szNum = szNumVal;
	STOP("CUnitCreationState::IntToString(int)");
	return szNum;
}

string CUnitCreationState::FloatToString(float fNum)
{
	PROFILE("CUnitCreationState::FloatToString(float)");
	char szNumVal[10];
	sprintf_s(szNumVal, 10, "%.2f", fNum);
	string szNum = szNumVal;
	STOP("CUnitCreationState::FloatToString(float)");
	return szNum;
}

