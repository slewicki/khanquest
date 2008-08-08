//////////////////////////////////////////////////////////
//	File:	"CGamePlayState.cpp"
//
//	Author:	Sean Hamstra (SH)
//
//	Purpose: To contain functionality of the gameplay state
//////////////////////////////////////////////////////////

#include "CGamePlayState.h"

#include "CGame.h"

#include "ObjectManager.h"

#include "CWorldMapState.h"
#include "CPausedState.h"
#include "HUDState.h"

CGamePlayState::CGamePlayState(void)
{
	m_pCamera = NULL;
	m_nButtonID = -1;
	m_nHUD_ID = -1;

	m_bButtonDown = false;
	m_nTerrorLevel = 0;
}


CGamePlayState::~CGamePlayState(void)
{

}


void CGamePlayState::Enter(void)
{
	// Get Our Managers Ready
	m_bIsPaused = false;
	m_pTM = CSGD_TextureManager::GetInstance();
	m_pWM = CSGD_WaveManager::GetInstance();
	m_pDI = CSGD_DirectInput::GetInstance();
	m_pES = CEventSystem::GetInstance();
	m_pD3D = CSGD_Direct3D::GetInstance();
	m_pHUD = CHUDState::GetInstance();
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

	m_nHUD_ID = m_pTM->LoadTexture("Resource/KQ_HUD.png");
	m_nButtonID = m_pTM->LoadTexture("Resource/KQ_ScrollButton.png");

	m_nLucidiaWhiteID = m_pTM->LoadTexture("Resource/KQ_FontLucidiaWhite.png");

	m_cFont.InitBitmapFont(m_nLucidiaWhiteID, ' ', 16, 128, 128);
	
	m_pHUD->Enter();
}

void CGamePlayState::Exit(void)
{
	m_pTM->ReleaseTexture(m_nButtonID);
	m_pTM->ReleaseTexture(m_nLucidiaWhiteID);

	//Remove all objects from manager?	
	ObjectManager::GetInstance()->RemoveAllObjects();
	m_pHUD->Enter();
}

bool CGamePlayState::Input(float fElapsedTime)
{
	if(m_pDI->GetBufferedKey(DIK_ESCAPE))
	{
		m_bIsPaused = !m_bIsPaused;
		CGame::GetInstance()->PushState(CPausedState::GetInstance());
	}

	if(!m_bIsPaused)
	{
		POINT ptMousePos = CGame::GetInstance()->GetMousePos(); 
		if(ptMousePos.y <= 450)
		{
			if(m_pDI->GetBufferedMouseButton(M_BUTTON_LEFT))
			{
				m_ptBoxLocation = ptMousePos;
				m_ptCurrentLocation.x = m_ptBoxLocation.x + 1;
				m_ptCurrentLocation.y = m_ptBoxLocation.y +1;
				m_rSelectionBox = GetSelectionRect();
				ObjectManager::GetInstance()->SetSelectedUnit(m_rSelectionBox);

			}
			else if(m_pDI->GetMouseButton(M_BUTTON_LEFT))
			{
				if(!m_bButtonDown)
				{
					m_bButtonDown = true;
					m_ptBoxLocation = ptMousePos;
				}
				else
				{	
					m_ptCurrentLocation = CGame::GetInstance()->GetMousePos();
					m_rSelectionBox = GetSelectionRect();
				}
			}
			if(m_pDI->OnMouseButtonRelease(M_BUTTON_LEFT))
			{
				ObjectManager::GetInstance()->SetSelectedUnit(m_rSelectionBox);
				m_bButtonDown = false;
				m_ptBoxLocation.x = m_ptBoxLocation.y = 0;
				m_ptCurrentLocation.x = m_ptCurrentLocation.y = 0;;
				m_rSelectionBox = GetSelectionRect();
			}

		
#pragma region Camera
			if(!m_bButtonDown)
			{
				m_pCamera->SetVelX(0);
				m_pCamera->SetVelY(0);

				// Mouse Camera Movement

				// Move camera Left
				if((ptMousePos.x <= 800 && ptMousePos.x >= 750))
					m_pCamera->SetVelX(100);
				// Move camera Right
				if((ptMousePos.x >= 0 && ptMousePos.x <= 50))
					m_pCamera->SetVelX(-100);
				// Move camera Down
				if(( ptMousePos.y <= 450 && ptMousePos.y >= 400 ))
					m_pCamera->SetVelY(100);
				// Move camera Up
				if((ptMousePos.y >= 0 && ptMousePos.y <= 50))
					m_pCamera->SetVelY(-100); 

				// Keyboard Camera Movement
				// Move camera Left
				if(m_pDI->GetKey(DIK_D))
					m_pCamera->SetVelX(100);
				// Move camera Right
				if(m_pDI->GetKey(DIK_A))
					m_pCamera->SetVelX(-100);
				// Move camera Down
				if(m_pDI->GetKey(DIK_S))
					m_pCamera->SetVelY(100);
				// Move camera Up
				if(m_pDI->GetKey(DIK_W))
					m_pCamera->SetVelY(-100); 
			}
#pragma endregion

		}
		else
		{
			if(!m_bButtonDown)
			{
				m_pCamera->SetVelX(0);
				m_pCamera->SetVelY(0);
				// Move camera Left
				if(m_pDI->GetKey(DIK_D))
					m_pCamera->SetVelX(100);
				// Move camera Right
				if(m_pDI->GetKey(DIK_A))
					m_pCamera->SetVelX(-100);
				// Move camera Down
				if(m_pDI->GetKey(DIK_S))
					m_pCamera->SetVelY(100);
				// Move camera Up
				if(m_pDI->GetKey(DIK_W))
					m_pCamera->SetVelY(-100); 
			}
#pragma region TempButtons
			if(CGame::GetInstance()->IsMouseInRect(m_rVictoryButton))
			{
				//		CGame::GetInstance()->SetCursorClick();
				if(m_pDI->GetBufferedMouseButton(M_BUTTON_LEFT))
				{
					// Go back to the map
					// Call this function if the user wins the battle
					if(m_nTerrorLevel < 100)
					{
						m_nTerrorLevel += 25;
					}
					CGame::GetInstance()->SetCityConquered(CGame::GetInstance()->GetSelectedCity());
					CGame::GetInstance()->ChangeState(CWorldMapState::GetInstance());
				}
			}

#pragma endregion
		}
	}
	return true;
}

void CGamePlayState::Update(float fElapsedTime)
{
	// Rendered Paused
	if(!m_bIsPaused)
	{
		if(m_bIsPaused)
			return;
		m_pCamera->Update(fElapsedTime);
		// Update units
		ObjectManager::GetInstance()->UpdateObjects(fElapsedTime);

		m_pHUD->Update(fElapsedTime);

		m_pES->ProcessEvents();
	}

}

void CGamePlayState::Render(float fElapsedTime)
{

	// Render units
	// Temp for map changes
	//-----------------------------------------------
	if( m_pDI->GetBufferedKey(DIK_1))
		Map.LoadFile("Resource/Levels/KQ_Wawa.level");
	else if( m_pDI->GetBufferedKey(DIK_2))
		Map.LoadFile("Resource/Levels/KQ_Wee.level");
	else if( m_pDI->GetBufferedKey(DIK_3))
		Map.LoadFile("Resource/Levels/KQ_Tech_Demo1.level");

	Map.Render();
	ObjectManager::GetInstance()->RenderObjects(fElapsedTime);

	// Temp for demo
	m_pTM->Draw(m_nButtonID, m_rVictoryButton.left, m_rVictoryButton.top, .4f, .3f);

	m_cFont.DrawTextA("Victory", m_rVictoryButton.left+30, m_rVictoryButton.top+24, .2f, .2f, D3DCOLOR_ARGB(255, 255, 0, 0));
	//----------------------------------------------
	RECT rHud;
	rHud.top = rHud.left = 0;
	rHud.bottom = 600;
	rHud.right = 800;
	m_pHUD->Render(fElapsedTime);
//	m_pTM->Draw(m_nHUD_ID,0,0,1,1,&rHud);

	// Render selection Box
	m_pD3D->SpriteEnd();
	m_pD3D->DeviceEnd();

	//m_pD3D->DrawLine(0,450,800,450,255,0,0);
	m_pD3D->DrawPrimitiveRect(m_rSelectionBox, D3DCOLOR_ARGB(255,255,255,255));

	m_pD3D->DeviceBegin();
	m_pD3D->SpriteBegin();
	//---------------------------------------------

}

string CGamePlayState::IntToString(int nNum)
{
	char szNumVal[10];
	itoa(nNum, szNumVal, 10);
	string szNum = szNumVal;
	return szNum;
}

RECT CGamePlayState::GetSelectionRect()
{
	RECT toDraw;
	toDraw.top = m_ptBoxLocation.y;
	toDraw.left = m_ptBoxLocation.x;
	toDraw.bottom = m_ptCurrentLocation.y;
	toDraw.right = m_ptCurrentLocation.x;

	int nSwap;
	if(toDraw.top > toDraw.bottom)
	{
		nSwap = toDraw.top;
		toDraw.top = toDraw.bottom;
		toDraw.bottom = nSwap;
	}
	if(toDraw.left > toDraw.right)
	{
		nSwap = toDraw.left;
		toDraw.left = toDraw.right;
		toDraw.right = nSwap;
	}
	return toDraw;
}
