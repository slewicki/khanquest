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
	if(CSGD_DirectInput::GetInstance()->GetBufferedKey(DIK_RETURN))
	{
		CGame::GetInstance()->PopCurrentState();
	}
	return true;
}

void CAttractMode::Exit()
{
	m_pDS->ShutDown();
	CMainMenuState::GetInstance()->SetPause(false);
}

void CAttractMode::Render(float fElapsedTime)
{
}

void CAttractMode::Update(float fElapsedTime)
{
	Sleep(10);
	m_pDS->Update();
}