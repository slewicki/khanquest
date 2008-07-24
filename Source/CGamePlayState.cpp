//////////////////////////////////////////////////////////
//	File:	"CGamePlayState.cpp"
//
//	Author:	Sean Hamstra (SH)
//
//	Purpose: To contain functionality of the gameplay state
//////////////////////////////////////////////////////////

#include "CGamePlayState.h"


CGamePlayState::CGamePlayState(void)
{
	
}


CGamePlayState::~CGamePlayState(void)
{
	
}


void CGamePlayState::Enter(void)
{
	// Get Our Managers Ready
	m_pTM = CSGD_TextureManager::GetInstance();
	m_pWM = CSGD_WaveManager::GetInstance();
	m_pDI = CSGD_DirectInput::GetInstance();
	
	// Register any Events with the GamePlayState
	
}

void CGamePlayState::Exit(void)
{
	//Remove all objects from manager?	

}

bool CGamePlayState::Input(float fElapsedTime)
{
	
	return true;
}

void CGamePlayState::Update(float fElapsedTime)
{
	if(m_bIsPaused)
		return;
}

void CGamePlayState::Render(float fElapsedTime)
{
		

}

