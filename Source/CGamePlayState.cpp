//////////////////////////////////////////////////////////
//	File:	"CGamePlayState.cpp"
//
//	Author:	Sean Hamstra (SH)
//
//	Purpose: To contain functionality of the gameplay state
//////////////////////////////////////////////////////////

#include "CGamePlayState.h"
#include "ObjectManager.h"
#include "CGame.h"
#include "CWorldMapState.h"


CGamePlayState::CGamePlayState(void)
{
	m_pCamera = NULL;
	m_nButtonID = -1;
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
	m_pES = CEventSystem::GetInstance();

	m_pES->RegisterClient("Play", ObjectManager::GetInstance());
	m_pCamera = CCamera::GetInstance();
	m_pCamera->InitCamera(0.f,0.f);

	// Register any Events with the GamePlayState
	Map.LoadFile("Resource/Levels/KQ_Wawa.level");
	//---------------------------------
	m_rVictoryButton.left = 100;
	m_rVictoryButton.top = 500;
	m_rVictoryButton.right = 230;
	m_rVictoryButton.bottom = 560;

	m_rRetreatButton.left = 600;
	m_rRetreatButton.top = 500;
	m_rRetreatButton.right = 730;
	m_rRetreatButton.bottom = 560;
	m_nButtonID = m_pTM->LoadTexture("Resource/KQ_ScrollButton.png");

	m_nLucidiaWhiteID = m_pTM->LoadTexture("Resource/KQ_FontLucidiaWhite.png");

	m_cFont.InitBitmapFont(m_nLucidiaWhiteID, ' ', 16, 128, 128);

}

void CGamePlayState::Exit(void)
{
	m_pTM->ReleaseTexture(m_nButtonID);
	m_pTM->ReleaseTexture(m_nLucidiaWhiteID);
	
	//Remove all objects from manager?	
	ObjectManager::GetInstance()->RemoveAllObjects();

}

bool CGamePlayState::Input(float fElapsedTime)
{


#pragma region TempButtons
	if(CGame::GetInstance()->IsMouseInRect(m_rVictoryButton))
	{
		CGame::GetInstance()->SetCursorClick();
		if(m_pDI->GetBufferedMouseButton(M_BUTTON_LEFT))
		{
			// Go back to the map
			CGame::GetInstance()->SetCityConquered(CGame::GetInstance()->GetSelectedCity());
			CGame::GetInstance()->ChangeState(CWorldMapState::GetInstance());
		}
	}
	if(CGame::GetInstance()->IsMouseInRect(m_rRetreatButton))
	{
		CGame::GetInstance()->SetCursorClick();
		if(m_pDI->GetBufferedMouseButton(M_BUTTON_LEFT))
		{
			// Go back to the map
			CGame::GetInstance()->LoseLastCity();
			CGame::GetInstance()->ChangeState(CWorldMapState::GetInstance());
		}
	} 
#pragma endregion
	m_pCamera->SetVelX(0);
	m_pCamera->SetVelY(0);
	POINT ptMousePos = CGame::GetInstance()->GetMousePos();
	if(ptMousePos.x >= 799)
		m_pCamera->SetVelX(100);
	if(ptMousePos.x <= 0)
		m_pCamera->SetVelX(-100);
	if(ptMousePos.y >= 599)
		m_pCamera->SetVelY(100);
	if(ptMousePos.y <= 0)
		m_pCamera->SetVelY(-100);

	return true;
}

void CGamePlayState::Update(float fElapsedTime)
{
	if(m_bIsPaused)
		return;
	m_pCamera->Update(fElapsedTime);
	// Update units
	ObjectManager::GetInstance()->UpdateObjects(fElapsedTime);
	m_pES->ProcessEvents();

}

void CGamePlayState::Render(float fElapsedTime)
{
	// Render units
	

	

	if( m_pDI->GetBufferedKey(DIK_1))
		Map.LoadFile("Resource/Levels/KQ_Wawa.level");
	else if( m_pDI->GetBufferedKey(DIK_2))
		Map.LoadFile("Resource/Levels/KQ_Wee.level");
	else if( m_pDI->GetBufferedKey(DIK_3))
		Map.LoadFile("Resource/Levels/KQ_Tech_Demo1.level");
	Map.Render();
	
	m_pTM->Draw(m_nButtonID, m_rVictoryButton.left, m_rVictoryButton.top, .4f, .3f);
	m_pTM->Draw(m_nButtonID, m_rRetreatButton.left, m_rRetreatButton.top, .4f, .3f);

	m_cFont.DrawTextA("Victory", m_rVictoryButton.left+30, m_rVictoryButton.top+24, .2f, .2f, D3DCOLOR_ARGB(255, 255, 0, 0));
	m_cFont.DrawTextA("Retreat", m_rRetreatButton.left+30, m_rRetreatButton.top+24, .2f, .2f, D3DCOLOR_ARGB(255, 255, 0, 0));
	ObjectManager::GetInstance()->RenderObjects(fElapsedTime);

}

