//////////////////////////////////////////////////////////////////////////
// File CAttractMode.h
//
//	Author:	Dennis Wallace (DW)
//
// Purpose: Contains class definition for CAttractMode Class,
//////////////////////////////////////////////////////////////////////////
#include "AttractMode.h"
#include "CSGD_Direct3D.h"
#include "CSGD_DirectInput.h"
#include "MainMenuState.h"
#include "CGame.h"
CAttractMode::CAttractMode(void)
{

}

CAttractMode::~CAttractMode(void)
{

}

void CAttractMode::Enter()
{
	m_pDS = CDirectShow::GetInstance();
	m_pDS->Init();
	m_pDS->Play(L"Resource/Test.wmv");
}

bool CAttractMode::Input(float fElapsedTime)
{

	return true;
}

void CAttractMode::Exit()
{

	CMainMenuState::GetInstance()->SetPause(false);
}

void CAttractMode::Render(float fElapsedTime)
{
}

void CAttractMode::Update(float fElapsedTime)
{
	if(!m_pDS->GetIsPlaying())
	{

		CGame::GetInstance()->PopCurrentState();
	}
	
	//Sleep(10);
	//m_pDS->Update(fElapsedTime);
}