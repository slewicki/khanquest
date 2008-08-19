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
#include "CPausedState.h"
#include "HUDState.h"
#include "CFactory.h"
//#include "KeyBindState.h"

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
	m_pOM = ObjectManager::GetInstance();
	Map = CTileEngine::GetInstance();
	m_pD3D = CSGD_Direct3D::GetInstance();
	m_pHUD = CHUDState::GetInstance();
	m_pES->RegisterClient("Play", ObjectManager::GetInstance());
	m_pES->RegisterClient("Remove", ObjectManager::GetInstance());
	m_pCamera = CCamera::GetInstance();
	m_pCamera->InitCamera(0.f,0.f);

	m_pPE = CParticleEngine::GetInstance();
	m_nTestEmitter = m_pPE->LoadBineryEmitter("Resource/Emitters/KQ_DustCload.dat", 128, 128);

	// Register any Events with the GamePlayState
	Map->LoadFile("Resource/Levels/KQ_Level3.level");

	m_pOM->UpdatePlayerUnitStartTile();


	//---------------------------------
	m_rVictoryButton.left = 100;
	m_rVictoryButton.top = 500;
	m_rVictoryButton.right = 230;
	m_rVictoryButton.bottom = 560;

	m_nHUD_ID = m_pTM->LoadTexture("Resource/KQ_HUD.png");
	m_nButtonID = m_pTM->LoadTexture("Resource/KQ_ScrollButton.png");

	m_nLucidiaWhiteID = m_pTM->LoadTexture("Resource/KQ_FontLucidiaWhite.png");
	m_nSelectionID = CSGD_TextureManager::GetInstance()->LoadTexture("Resource/KQ_SelectionCircle1.png");


	m_cFont.InitBitmapFont(m_nLucidiaWhiteID, ' ', 16, 128, 128);
	
	m_pHUD->Enter();
}

void CGamePlayState::Exit(void)
{
	m_pTM->ReleaseTexture(m_nButtonID);
	m_pTM->ReleaseTexture(m_nLucidiaWhiteID);
	m_pTM->ReleaseTexture(m_nSelectionID);

	//Remove all objects from manager?	
	ObjectManager::GetInstance()->RemoveAllObjects();
	m_pHUD->Exit();
	//m_pHUD->Enter();
}

bool CGamePlayState::Input(float fElapsedTime)
{
	if(m_pDI->GetBufferedKey(DIK_ESCAPE))
	{
		m_bIsPaused = true;
		if(m_bIsPaused)
			CGame::GetInstance()->PushState(CPausedState::GetInstance());
	}

	if(!m_bIsPaused)
	{

		if(m_pDI->GetBufferedKey(DIK_F1))
		{
			//Tile  = &Map->GetTile(4,4);
			m_pOM->UpdatePlayerUnitDestTile(Map->GetTile(0,2,4));
		}
		if(m_pDI->GetBufferedKey(DIK_F2))
		{
			m_pPE->SetPostion(200, 200, m_nTestEmitter);
			m_pPE->SetIsRunning(m_nTestEmitter, true);
		}

		if(m_pDI->GetBufferedKey(DIK_F3))
		{
			m_pOM->SetSelectedUnitsRetreat();
		}

		POINT ptMousePos = CGame::GetInstance()->GetMousePos(); 
		if(ptMousePos.y <= 450 && ((ptMousePos.x > 0 && ptMousePos.x < 800) && (ptMousePos.y > 0 && ptMousePos.y < 600)))
		{
			if(m_pDI->GetBufferedMouseButton(M_BUTTON_LEFT))
			{
				m_ptBoxLocation = ptMousePos;
				m_ptCurrentLocation.x = m_ptBoxLocation.x + 1;
				m_ptCurrentLocation.y = m_ptBoxLocation.y +1;
				m_rSelectionBox = GetSelectionRect();
				m_pOM->SetSelectedUnit(m_rSelectionBox);
			}
			else if(m_pDI->GetBufferedMouseButton(M_BUTTON_RIGHT))
			{
				POINT globleMouse = m_pCamera->TransformToGlobal(ptMousePos.x, ptMousePos.y);
				globleMouse = Map->IsoMouse(globleMouse.x, globleMouse.y, 0);
				m_pOM->UpdatePlayerUnitDestTile(Map->GetTile(0,globleMouse.x, globleMouse.y));
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
				m_pOM->SetSelectedUnit(m_rSelectionBox);
				m_bButtonDown = false;
				m_ptBoxLocation.x = m_ptBoxLocation.y = 0;
				m_ptCurrentLocation.x = m_ptCurrentLocation.y = 0;;
				m_rSelectionBox = GetSelectionRect();
			}
		}
			
		
#pragma region Camera
			if(!m_bButtonDown)
			{
				m_pCamera->SetVelX(0);
				m_pCamera->SetVelY(0);

				// Mouse Camera Movement

				// Move camera Left
				//if(CGame::GetInstance()->GetCursorPosition().x <= 5)
				//	m_pCamera->SetVelX(-100);
				//// Move camera Right
				//if(CGame::GetInstance()->GetCursorPosition().x >= 795)
				//	m_pCamera->SetVelX(100);
				//// Move camera Down
				//if(CGame::GetInstance()->GetCursorPosition().y >= 595 )
				//	m_pCamera->SetVelY(100);
				//// Move camera Up
				//if(CGame::GetInstance()->GetCursorPosition().y <= 5)
				//	m_pCamera->SetVelY(-100); 

				// Keyboard Camera Movement
				// Move camera Left
				if(m_pDI->GetKey(DIK_D))//m_pDI->GetKey((UCHAR)(CKeyBindState::GetInstance()->GetBoundKey(CAMERA_RIGHT))))
					m_pCamera->SetVelX(100);
				// Move camera Right
				if( m_pDI->GetKey(DIK_A))//m_pDI->GetKey((UCHAR)(CKeyBindState::GetInstance()->GetBoundKey(CAMERA_LEFT))))
					m_pCamera->SetVelX(-100);
				// Move camera Down
				if(m_pDI->GetKey(DIK_S))//m_pDI->GetKey((UCHAR)(CKeyBindState::GetInstance()->GetBoundKey(CAMERA_DOWN))))
					m_pCamera->SetVelY(100);
				// Move camera Up
				if(m_pDI->GetKey(DIK_W))//m_pDI->GetKey((UCHAR)(CKeyBindState::GetInstance()->GetBoundKey(CAMERA_UP))))
					m_pCamera->SetVelY(-100); 
			}
#pragma endregion		
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
		m_pOM->UpdateObjects(fElapsedTime);

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
		Map->LoadFile("Resource/Levels/KQ_Wawa.level");
	else if( m_pDI->GetBufferedKey(DIK_2))
		Map->LoadFile("Resource/Levels/KQ_Wee.level");
	else if( m_pDI->GetBufferedKey(DIK_3))
		Map->LoadFile("Resource/Levels/KQ_Tech_Demo1.level");

	POINT MapLoc = m_pCamera->TransformToGlobal(CGame::GetInstance()->GetCursorPosition().x, CGame::GetInstance()->GetCursorPosition().y);

	Map->Render(m_pCamera->GetScreenArea());

	if(m_pDI->GetMouseButton(M_BUTTON_RIGHT))
	{
		char buffer[32];	//Tile Pos
		char buffer2[32];	//Tile Type
		char buffer3[32];	//Player Spawn
		char buffer4[32];	//Local Anchor
		char buffer5[32];	//Global Anchor
		POINT TileLoc = Map->IsoMouse(MapLoc.x, MapLoc.y, 0);

		sprintf_s(buffer, 32, "Tile: %i, %i", TileLoc.x, TileLoc.y);
		sprintf_s(buffer2, 32, "TileType: %i", Map->GetTile(0,TileLoc.x, TileLoc.y)->nType);

		if(Map->GetTile(0,TileLoc.x, TileLoc.y)->bIsPlayerSpawn == true)
			sprintf_s(buffer3, 32, "PlayerSpawn: True");
		else
			sprintf_s(buffer3, 32, "PlayerSpawn: False");

		sprintf_s(buffer4, 32, "Local Anchor: %i, %i", Map->GetTile(0,TileLoc.x, TileLoc.y)->ptLocalAnchor.x, Map->GetTile(0,TileLoc.x, TileLoc.y)->ptLocalAnchor.y);
		sprintf_s(buffer5, 32, "Global Anchor: %i, %i", MapLoc.x, MapLoc.y);

		m_cFont.DrawTextA(buffer, 500, 0, .2f, .2f);
		m_cFont.DrawTextA(buffer2, 500, 30, .2f, .2f);
		m_cFont.DrawTextA(buffer3, 500, 60, .2f, .2f);
		m_cFont.DrawTextA(buffer4, 500, 90, .2f, .2f);
		m_cFont.DrawTextA(buffer5, 500, 120, .2f, .2f);
	}

	m_pOM->RenderObjects(fElapsedTime);

	// Temp for demo
	m_pTM->Draw(m_nButtonID, m_rVictoryButton.left, m_rVictoryButton.top, .4f, .3f);
	Map->ParalaxScroll(true, m_pCamera->GetScreenArea());

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
	/*char buffer[128];
	sprintf_s(buffer, 128, "%i, %i", CGame::GetInstance()->GetCursorPosition().x, CGame::GetInstance()->GetCursorPosition().y);
	m_cFont.DrawTextA(buffer, 0, 0);*/

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
	if(toDraw.bottom == toDraw.top)
		toDraw.bottom++;
	if(toDraw.left == toDraw.right)
		toDraw.right++;
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
