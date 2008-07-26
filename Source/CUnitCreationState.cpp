//////////////////////////////////////////////////////////
//	File:	"CUnitCreationState.cpp"
//
//	Author:	Sean Hamstra (SH)
//
//	Purpose: To contain functionality of the unit creation state
//////////////////////////////////////////////////////////

#include "CUnitCreationState.h"
#include "CWorldMapState.h"
#include "CGame.h"


CUnitCreationState::CUnitCreationState(void)
{
}


CUnitCreationState::~CUnitCreationState(void)
{
	
}


void CUnitCreationState::Enter(void)
{
	// Get Our Managers Ready
	m_fTimer = 0.f;

	m_pTM = CSGD_TextureManager::GetInstance();
	m_pWM = CSGD_WaveManager::GetInstance();
	m_pDI = CSGD_DirectInput::GetInstance();
	m_pSelectedCity = CGame::GetInstance()->GetSelectedCity();

	// Replace with animations
	//m_nUnitID[INFANTRY] = m_pTM->LoadTexture("Resource/KQ_Infantry.png");
	//m_nUnitID[CAVALRY] = m_pTM->LoadTexture("Resource/KQ_Cavalry.png");
	//m_nUnitID[CAVALRY_ARCHER] = m_pTM->LoadTexture("Resource/KQ_CavalryArcher.png");
	//m_nUnitID[AXMEN] = m_pTM->LoadTexture("Resource/KQ_Axmen.png");
	//m_nUnitID[ARCHER] = m_pTM->LoadTexture("Resource/KQ_Archer.png");
	//m_nUnitID[WAR_ELEPHANT] = m_pTM->LoadTexture("Resource/KQ_WarElephant.png");
	// Register any Events with the CUnitCreationState
	m_nLucidiaWhiteID = m_pTM->LoadTexture("Resource/KQ_FontLucidiaWhite.png");
	
	m_cFont.InitBitmapFont(m_nLucidiaWhiteID, ' ', 16, 128, 128);
	m_pPlayerUnitInfo = CGame::GetInstance()->GetPlayerUnitInfo();

	m_nUnitCosts[UNIT_INFANTRY] = 65;
	m_nUnitCosts[UNIT_CAVALRY] = 150;
	m_nUnitCosts[UNIT_CAVALRY_ARCHER] = 150;
	m_nUnitCosts[UNIT_AXMEN] = 65;
	m_nUnitCosts[UNIT_ARCHER] = 60;
	m_nUnitCosts[UNIT_WAR_ELEPHANT] = 300;
}

void CUnitCreationState::Exit(void)
{
	m_pTM->ReleaseTexture(m_nLucidiaWhiteID);
}

bool CUnitCreationState::Input(float fElapsedTime)
{
	if(m_pDI->GetBufferedMouseButton(M_BUTTON_RIGHT))
		CGame::GetInstance()->ChangeState(CWorldMapState::GetInstance());
	return true;
}

void CUnitCreationState::Update(float fElapsedTime)
{

}

void CUnitCreationState::Render(float fElapsedTime)
{
	int nPosY = 80;
	int nPosX = 40;
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
			nPosX = 420;
			nPosY = 80;
			break;
		case 4:
			nPosX = 420;
			szType = "Archer";
			break;
		case 5:
			nPosX = 420;
			szType = "War Elephant";
			break;
		}
		
		m_cFont.DrawTextA(szType + "/HP: " + IntToString(m_pPlayerUnitInfo[i].GetHP()) +
		"    Attack: " +IntToString(m_pPlayerUnitInfo[i].GetAttackPower()) +
		"/Attack Speed: " + FloatToString(m_pPlayerUnitInfo[i].GetAttackSpeed()) +
		"    Range: " +IntToString(m_pPlayerUnitInfo[i].GetRange()) +
		"/Movement: " +FloatToString(m_pPlayerUnitInfo[i].GetSpeed()), nPosX, nPosY, .2f, .2f, D3DCOLOR_ARGB(255, 255, 255, 0));
		nPosY += 150;
	}
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

