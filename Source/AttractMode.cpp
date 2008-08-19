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
	m_nDemoID = -1;
}

CAttractMode::~CAttractMode(void)
{

}

void CAttractMode::Enter()
{
	m_pDS = CDirectShow::GetInstance();
	m_pDS->Init();
	m_nDemoID = m_pDS->LoadVideo(L"Resource/KQ_Test.WMV", CGame::GetInstance()->GetWindowHandle(), CGame::GetInstance()->GetIsWindowed());
	m_pDS->Play(m_nDemoID);
}

bool CAttractMode::Input(float fElapsedTime)
{
	CSGD_DirectInput* pDI = CSGD_DirectInput::GetInstance();
	if(!m_pDS->IsPlaying() || pDI->GetBufferedKey(DIK_ESCAPE) || pDI->GetBufferedKey(DIK_RETURN) || pDI->GetBufferedKey(DIK_SPACE))
	{
		m_pDS->Stop(m_nDemoID);
		CGame::GetInstance()->ChangeState(CMainMenuState::GetInstance());
	}
	return true;
}

void CAttractMode::Exit()
{
	m_pDS->ShutDown();
	ShowWindow(CGame::GetInstance()->GetWindowHandle(), SW_RESTORE);
	SetFocus(CGame::GetInstance()->GetWindowHandle());
}

void CAttractMode::Render(float fElapsedTime)
{
}

void CAttractMode::Update(float fElapsedTime)
{

}