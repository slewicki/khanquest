#include "UpgradeMenuState.h"
#include "CSGD_TextureManager.h"
#include "CSGD_WaveManager.h"
#include "CSGD_DirectInput.h"
#include "CGame.h"
#include "CGamePlayState.h"
#include "CUnit.h"
#include "CUnitCreationState.h"

CUpgradeMenuState::CUpgradeMenuState(void)
{
	m_nPlusButtonID = -1;
	m_nMinusButtonID = -1;
	m_nTotalUnits = 0;
	m_nNumUnits[0] = 0;
	m_nBackgroundID = -1;
	m_nFoodTotal = 0;
	m_nScrollButtonID = -1;
}

CUpgradeMenuState::~CUpgradeMenuState(void)
{
}
void CUpgradeMenuState::Enter()
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

	m_nLucidiaWhiteID = m_pTM->LoadTexture("Resource/KQ_FontLucidiaWhite.png");
	m_nPlusButtonID = m_pTM->LoadTexture("Resource/KQ_PlusButton.png");
	m_nMinusButtonID = m_pTM->LoadTexture("Resource/KQ_MinusButton.png");
	m_nBackgroundID = m_pTM->LoadTexture("Resource/KQ_PageBkg3.png");
	m_nScrollButtonID = m_pTM->LoadTexture("Resource/KQ_ScrollButton.png");
	m_nCheckBoxID = m_pTM->LoadTexture("Resource/KQ_CheckBox.png");
	m_nCheckMarkID = m_pTM->LoadTexture("Resource/KQ_CheckMark.png");
	m_nClick = m_pWM->LoadWave("Resource/KQ_Click.wav");
	m_nSongID = m_pWM->LoadWave("Resource/KQ_CitySelect.wav");
	m_pWM->SetVolume(m_nSongID,CGame::GetInstance()->GetMusicVolume());
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
	m_pWM->Play(m_nSongID);	
}

void CUpgradeMenuState::Exit()
{	
	if(m_pWM->IsWavePlaying(m_nSongID))
		m_pWM->Stop(m_nSongID);
	if(m_pWM->IsWavePlaying(m_nClick))
		m_pWM->Stop(m_nClick);
	m_pWM->UnloadWave(m_nSongID);
	m_pWM->UnloadWave(m_nClick);

	m_pTM->ReleaseTexture(m_nPlusButtonID);
	m_pTM->ReleaseTexture(m_nMinusButtonID);
	m_pTM->ReleaseTexture(m_nLucidiaWhiteID);
	m_pTM->ReleaseTexture(m_nBackgroundID);
	m_pTM->ReleaseTexture(m_nScrollButtonID);
//ToDo: Upgrade Units

}

bool CUpgradeMenuState::Input(float fElapsedTime)
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
	if(CGame::GetInstance()->IsMouseInRect(m_rAttackButton))
	{
		CGame::GetInstance()->SetCursorClick();
		if(m_pDI->GetBufferedMouseButton(M_BUTTON_LEFT))
		{
			// Ask if they are sure...?
			
			// Figure out map and enemy units (call factory enemycreates)
			// Change to on GamePlayState
			m_pWM->Play(m_nClick);
			CGame::GetInstance()->ChangeState(CGamePlayState::GetInstance());
		}
	}
	if(CGame::GetInstance()->IsMouseInRect(m_rBackButton))
	{
		CGame::GetInstance()->SetCursorClick();
		if(m_pDI->GetBufferedMouseButton(M_BUTTON_LEFT))
		{
			// Go back to the map
			m_pWM->Play(m_nClick);
			CGame::GetInstance()->ChangeState(CUnitCreationState::GetInstance());
		}
	}

	return true;
}

void CUpgradeMenuState::Render(float fElapsedTime)
{
	m_pTM->Draw(m_nBackgroundID, -20, -10);
	m_cFont.DrawTextA("Upgrades",225,0,.5,.5,D3DCOLOR_ARGB(255,255,0,0));
	for(int i = 0; i < 6; ++i)
	{
		switch(i)
		{	
			case 0:
				{
					m_cFont.DrawTextA("Infantry:",		10, 100 + (i*50),.2f,.2f,D3DCOLOR_ARGB(255,0,0,0));
					m_cFont.DrawTextA("HP:",250,100 + (i*50),.2f,.2f,D3DCOLOR_ARGB(255,0,0,0));
					m_pTM->Draw(m_nCheckBoxID,305,100+i*50);
					m_cFont.DrawTextA("Attack Speed:",350,100+i*50,.2f,.2f,D3DCOLOR_ARGB(255,0,0,0));
					m_pTM->Draw(m_nCheckBoxID,525,100+i*50);
					m_cFont.DrawTextA("Range:",575,100+i*50,.2f,.2f,D3DCOLOR_ARGB(255,0,0,0));
					m_pTM->Draw(m_nCheckBoxID,675,100+i*50);
				}
				break;
			case 1:
				{
					m_cFont.DrawTextA("Cavalry:",		10,100 + (i*50),.2f,.2f,D3DCOLOR_ARGB(255,0,0,0));
					m_cFont.DrawTextA("HP:",250,100 + (i*50),.2f,.2f,D3DCOLOR_ARGB(255,0,0,0));
					m_pTM->Draw(m_nCheckBoxID,305,100+i*50);
					m_cFont.DrawTextA("Attack Speed:",350,100+i*50,.2f,.2f,D3DCOLOR_ARGB(255,0,0,0));
					m_pTM->Draw(m_nCheckBoxID,525,100+i*50);
					m_cFont.DrawTextA("Range:",575,100+i*50,.2f,.2f,D3DCOLOR_ARGB(255,0,0,0));
					m_pTM->Draw(m_nCheckBoxID,675,100+i*50);
				}
				break;
			case 2:
				{
					m_cFont.DrawTextA("Cavalry Archer:",		10,100 + (i*50),.2f,.2f,D3DCOLOR_ARGB(255,0,0,0));
					m_cFont.DrawTextA("HP:",250,100 + (i*50),.2f,.2f,D3DCOLOR_ARGB(255,0,0,0));
					m_pTM->Draw(m_nCheckBoxID,305,100+i*50);
					m_cFont.DrawTextA("Attack Speed:",350,100+i*50,.2f,.2f,D3DCOLOR_ARGB(255,0,0,0));
					m_pTM->Draw(m_nCheckBoxID,525,100+i*50);
					m_cFont.DrawTextA("Range:",575,100+i*50,.2f,.2f,D3DCOLOR_ARGB(255,0,0,0));
					m_pTM->Draw(m_nCheckBoxID,675,100+i*50);
				}
				break;
			case 3:
				{
					m_cFont.DrawTextA("Axmen:",		10,100 + (i*50),.2f,.2f,D3DCOLOR_ARGB(255,0,0,0));
					m_cFont.DrawTextA("HP:",250,100 + (i*50),.2f,.2f,D3DCOLOR_ARGB(255,0,0,0));
					m_pTM->Draw(m_nCheckBoxID,305,100+i*50);
					m_cFont.DrawTextA("Attack Speed:",350,100+i*50,.2f,.2f,D3DCOLOR_ARGB(255,0,0,0));
					m_pTM->Draw(m_nCheckBoxID,525,100+i*50);
					m_cFont.DrawTextA("Range:",575,100+i*50,.2f,.2f,D3DCOLOR_ARGB(255,0,0,0));
					m_pTM->Draw(m_nCheckBoxID,675,100+i*50);
				}
				break;
			case 4:
				{
					m_cFont.DrawTextA("Archer:",		10,100 + (i*50),.2f,.2f,D3DCOLOR_ARGB(255,0,0,0));
					m_cFont.DrawTextA("HP:",250,100 + (i*50),.2f,.2f,D3DCOLOR_ARGB(255,0,0,0));
					m_pTM->Draw(m_nCheckBoxID,305,100+i*50);
					m_cFont.DrawTextA("Attack Speed:",350,100+i*50,.2f,.2f,D3DCOLOR_ARGB(255,0,0,0));
					m_pTM->Draw(m_nCheckBoxID,525,100+i*50);
					m_cFont.DrawTextA("Range:",575,100+i*50,.2f,.2f,D3DCOLOR_ARGB(255,0,0,0));
					m_pTM->Draw(m_nCheckBoxID,675,100+i*50);
				}
				break;
			case 5:
				{
					m_cFont.DrawTextA("War Elephant:",		10,100 + (i*50),.2f,.2f,D3DCOLOR_ARGB(255,0,0,0));
					m_cFont.DrawTextA("HP:",250,100 + (i*50),.2f,.2f,D3DCOLOR_ARGB(255,0,0,0));
					m_pTM->Draw(m_nCheckBoxID,305,100+i*50);
					m_cFont.DrawTextA("Attack Speed:",350,100+i*50,.2f,.2f,D3DCOLOR_ARGB(255,0,0,0));
					m_pTM->Draw(m_nCheckBoxID,525,100+i*50);
					m_cFont.DrawTextA("Range:",575,100+i*50,.2f,.2f,D3DCOLOR_ARGB(255,0,0,0));
					m_pTM->Draw(m_nCheckBoxID,675,100+i*50);
				}
				break;
		}

	}

	m_cFont.DrawTextA("Gold:        " + IntToString(CGame::GetInstance()->GetTotalGold()), 50, 500, .25f, .25f, D3DCOLOR_ARGB(255, 255, 255, 0));
	m_cFont.DrawTextA("Army Size: " + IntToString(m_nTotalUnits), 50, 530, .25f, .25f, D3DCOLOR_ARGB(255, 0, 0, 255));
	m_cFont.DrawTextA("Max Size:   " + IntToString(MAX_UNITS), 50, 560, .25f, .25f, D3DCOLOR_ARGB(255, 0, 0, 0));


	m_pTM->Draw(m_nScrollButtonID, m_rAttackButton.left, m_rAttackButton.top, .4f, .3f);
	m_cFont.DrawTextA("Attack!", m_rAttackButton.left+30, m_rAttackButton.top+24, .2f, .2f, D3DCOLOR_ARGB(255, 255, 0, 0));

	m_pTM->Draw(m_nScrollButtonID, m_rBackButton.left, m_rBackButton.top, .4f, .3f);
	m_cFont.DrawTextA("Back", m_rBackButton.left+40, m_rBackButton.top+24, .2f, .2f, D3DCOLOR_ARGB(255, 255, 0, 0));

}

string CUpgradeMenuState::IntToString(int nNum)
{
	char szNumVal[10];
	itoa(nNum, szNumVal, 10);
	string szNum = szNumVal;
	return szNum;
}

string CUpgradeMenuState::FloatToString(float fNum)
{
	char szNumVal[10];
	sprintf_s(szNumVal, 10, "%.2f", fNum);
	string szNum = szNumVal;
	return szNum;
}

void CUpgradeMenuState::Update(float fElapsedTime)
{

}