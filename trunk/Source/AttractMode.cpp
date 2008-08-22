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

CAttractMode::CAttractMode(void)
{
	PROFILE("CAttractMode::CAttractMode()");
	m_nDemoID = -1;
	
}

CAttractMode::~CAttractMode(void)
{

}

void CAttractMode::Enter()
{
	PROFILE("CAttractMode::Enter()");
	m_pDS = CDirectShow::GetInstance();
	m_pDS->Init();
	m_nDemoID = m_pDS->LoadVideo(L"Resource/KQ_Test.WMV", m_pCG->GetWindowHandle(), m_pCG->GetIsWindowed());
	m_pDS->Play(m_nDemoID);
	m_pCG = CGame::GetInstance();
}

bool CAttractMode::Input(float fElapsedTime)
{
	PROFILE("CAttractMode::Input(float)");
	CSGD_DirectInput* pDI = CSGD_DirectInput::GetInstance();
	if(!m_pDS->IsPlaying() || pDI->GetBufferedKey(DIK_ESCAPE) || pDI->GetBufferedKey(DIK_RETURN) || pDI->GetBufferedKey(DIK_NUMPADENTER) || pDI->GetBufferedKey(DIK_SPACE))
	{
		m_pDS->Stop(m_nDemoID);
		m_pCG->ChangeState(CMainMenuState::GetInstance());
	}
	return true;
}

void CAttractMode::Exit()
{
	PROFILE("CAttractMode::Exit()");
	m_pDS->ShutDown();
	ShowWindow(m_pCG->GetWindowHandle(), SW_RESTORE);
	SetFocus(m_pCG->GetWindowHandle());
}

void CAttractMode::Render(float fElapsedTime)
{
}

void CAttractMode::Update(float fElapsedTime)
{

}