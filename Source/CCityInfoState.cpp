//////////////////////////////////////////////////////////
//	File:	"CCityInfoState.cpp"
//
//	Author:	Sean Hamstra (SH)
//
//	Purpose: To contain functionality of the city info state
//////////////////////////////////////////////////////////

#include "CCityInfoState.h"
#include "CWorldMapState.h"
#include "CGame.h"


CCityInfoState::CCityInfoState(void)
{
	m_pSelectedCity = NULL;
	m_bRetract = false;
}


CCityInfoState::~CCityInfoState(void)
{
	
}


void CCityInfoState::Enter(void)
{
	// Get Our Managers Ready
	m_bRetract = false;
	m_pTM = CSGD_TextureManager::GetInstance();
	m_pWM = CSGD_WaveManager::GetInstance();
	m_pDI = CSGD_DirectInput::GetInstance();
	
	m_pSelectedCity = CGame::GetInstance()->GetSelectedCity();

	m_fPositionX = 800.f;
	// Register any Events with the CCityInfoState
	switch(m_pSelectedCity->GetOwner())
	{
	case K_CITY:
		m_nDisplayID = m_pTM->LoadTexture("Resource/KQ_KInfoBkg.png");
		break;
	case XIA_CITY:
		m_nDisplayID = m_pTM->LoadTexture("Resource/KQ_XiaInfoBkg.png");
		break;
	case JIN_CITY:
		m_nDisplayID = m_pTM->LoadTexture("Resource/KQ_JinInfoBkg.png");
		break;
	default:
		// We got a problem here
		CGame::GetInstance()->PopCurrentState();
	}
	
}

void CCityInfoState::Exit(void)
{
	m_pTM->ReleaseTexture(m_nDisplayID);
}

bool CCityInfoState::Input(float fElapsedTime)
{
	if(m_pDI->GetBufferedKey(VK_RETURN))
	{
		// This function is used only if the city is conquered after battle
		//--------------------
		CWorldMapState::GetInstance()->SetCityConquered(m_pSelectedCity);
		m_bRetract = true;
		//--------------------
		
		// Push on or change state (transition) to CGamePlayState here
	}
	if(m_pDI->GetBufferedMouseButton(M_BUTTON_RIGHT))
		m_bRetract = true;
	return true;
}

void CCityInfoState::Update(float fElapsedTime)
{
	if(m_bRetract)
	{
		m_fPositionX  += 500*fElapsedTime;
		if(m_fPositionX > 805)
			CGame::GetInstance()->ChangeState(CWorldMapState::GetInstance());
	}
	else if(m_fPositionX >= 270)
		m_fPositionX  -= 500*fElapsedTime;
	
}

void CCityInfoState::Render(float fElapsedTime)
{
	m_pTM->Draw(m_nDisplayID, (int)m_fPositionX, 20);

}

