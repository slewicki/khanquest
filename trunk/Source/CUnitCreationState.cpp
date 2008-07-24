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
	
	// Replace with animations
	//m_nUnitID[INFANTRY] = m_pTM->LoadTexture("Resource/KQ_Infantry.png");
	//m_nUnitID[CAVALRY] = m_pTM->LoadTexture("Resource/KQ_Cavalry.png");
	//m_nUnitID[CAVALRY_ARCHER] = m_pTM->LoadTexture("Resource/KQ_CavalryArcher.png");
	//m_nUnitID[AXMEN] = m_pTM->LoadTexture("Resource/KQ_Axmen.png");
	//m_nUnitID[ARCHER] = m_pTM->LoadTexture("Resource/KQ_Archer.png");
	//m_nUnitID[WAR_ELEPHANT] = m_pTM->LoadTexture("Resource/KQ_WarElephant.png");
	// Register any Events with the CUnitCreationState
	
}

void CUnitCreationState::Exit(void)
{
	
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
		

}

