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
#include "CGame.h"
#include "CFactory.h"


CUnitCreationState::CUnitCreationState(void)
{
	m_nPlusButtonID = -1;
	m_nMinusButtonID = -1;
	m_nTotalUnits = 0;
	m_nNumUnits[0] = 0;
	m_nBackgroundID = -1;
	m_nFoodTotal = 0;
	m_nScrollButtonID = -1;
}


CUnitCreationState::~CUnitCreationState(void)
{
	
}


void CUnitCreationState::Enter(void)
{
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

	m_rAttackButton.left = 415;
	m_rAttackButton.top = 515;
	m_rAttackButton.right = 545;
	m_rAttackButton.bottom = 575;	
	
	m_rBackButton.left = 615;
	m_rBackButton.top = 515;
	m_rBackButton.right = 745;
	m_rBackButton.bottom = 575;

	CGame::GetInstance()->SetSongPlay(CITYSELECT);

	
}

void CUnitCreationState::Exit(void)
{
	if(m_pWM->IsWavePlaying(m_nClick))
		m_pWM->Stop(m_nClick);
	m_pWM->UnloadWave(m_nClick);

	m_pTM->ReleaseTexture(m_nPlusButtonID);
	m_pTM->ReleaseTexture(m_nMinusButtonID);
	m_pTM->ReleaseTexture(m_nLucidiaWhiteID);
	m_pTM->ReleaseTexture(m_nBackgroundID);
	m_pTM->ReleaseTexture(m_nScrollButtonID);

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

}

bool CUnitCreationState::Input(float fElapsedTime)
{
	for (int i = 0; i < 6; i++)
	{
		if(CGame::GetInstance()->IsMouseInRect(m_rMinusButtons[i]))
		{
			// Change cursor to click icon
			CGame::GetInstance()->SetCursorClick();
			if(m_pDI->GetBufferedMouseButton(M_BUTTON_LEFT))
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
			if(m_nFoodTotal > 0 && m_nTotalUnits < MAX_UNITS && m_pDI->GetBufferedMouseButton(M_BUTTON_LEFT))
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
		if(m_pDI->GetBufferedMouseButton(M_BUTTON_LEFT))
		{
			
			if(m_nTotalUnits > 0)
			{
				m_pWM->Play(m_nClick);

				// Ask if they are sure...?
				
				// Figure out map and enemy units (call factory enemycreates)
				// Change to on GamePlayState
				CGame::GetInstance()->ChangeState(CUpgradeMenuState::GetInstance());
				
			}
		}
	}
	if(CGame::GetInstance()->IsMouseInRect(m_rBackButton))
	{
		CGame::GetInstance()->SetCursorClick();
		if(m_pDI->GetBufferedMouseButton(M_BUTTON_LEFT))
		{
			m_pWM->Play(m_nClick);

			// Go back to the map
			CGame::GetInstance()->ChangeState(CWorldMapState::GetInstance());
		}
	}

	return true;
}

void CUnitCreationState::Update(float fElapsedTime)
{
	
}

void CUnitCreationState::Render(float fElapsedTime)
{
	m_pTM->Draw(m_nBackgroundID, -20, -10);
	
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
	
	m_cFont.DrawTextA("Food:        " + IntToString(m_nFoodTotal), 50, 500, .25f, .25f, D3DCOLOR_ARGB(255, 255, 0, 0));
	m_cFont.DrawTextA("Army Size: " + IntToString(m_nTotalUnits), 50, 530, .25f, .25f, D3DCOLOR_ARGB(255, 0, 0, 255));
	m_cFont.DrawTextA("Max Size:   " + IntToString(MAX_UNITS), 50, 560, .25f, .25f, D3DCOLOR_ARGB(255, 0, 0, 0));

	
	m_pTM->Draw(m_nScrollButtonID, m_rAttackButton.left, m_rAttackButton.top, .4f, .3f);
	m_cFont.DrawTextA("Attack!", m_rAttackButton.left+30, m_rAttackButton.top+24, .2f, .2f, D3DCOLOR_ARGB(255, 255, 0, 0));

	if(!m_nTotalUnits)
		m_pTM->Draw(m_nScrollButtonID, m_rAttackButton.left, m_rAttackButton.top, .4f, .3f, 0, 0, 0, 0, D3DCOLOR_ARGB(150, 50, 50, 50));

	m_pTM->Draw(m_nScrollButtonID, m_rBackButton.left, m_rBackButton.top, .4f, .3f);
	m_cFont.DrawTextA("Back", m_rBackButton.left+40, m_rBackButton.top+24, .2f, .2f, D3DCOLOR_ARGB(255, 255, 0, 0));

}

string CUnitCreationState::IntToString(int nNum)
{
	char szNumVal[10];
	itoa(nNum, szNumVal, 10);
	string szNum = szNumVal;
	return szNum;
}

string CUnitCreationState::FloatToString(float fNum)
{
	char szNumVal[10];
	sprintf_s(szNumVal, 10, "%.2f", fNum);
	string szNum = szNumVal;
	return szNum;
}

