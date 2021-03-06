//////////////////////////////////////////////////////////
//	File:	"CGamePlayState.cpp"
//
//	Author:	Sean Hamstra (SH)
//
//	Purpose: To contain functionality of the gameplay state
//////////////////////////////////////////////////////////

#include "CGamePlayState.h"
#include "ObjectManager.h"
#include "CWorldMapState.h"
#include "CPausedState.h"
#include "HUDState.h"
#include "CFactory.h"
#include "KeyBindState.h"
#include "CGame.h"

CGamePlayState::CGamePlayState(void)
{
	PROFILE("CGamePlayState::CGamePlayState()");
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
	PROFILE("CGamePlayState::Enter()");
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
	m_pPE = CParticleEngine::GetInstance();

	m_pES->RegisterClient("Play", ObjectManager::GetInstance());
	//m_pES->RegisterClient("Remove", ObjectManager::GetInstance());
	m_pCamera = CCamera::GetInstance();
	m_pCamera->InitCamera(0.f,0.f);
	m_pCG = CGame::GetInstance();

	m_pPE = CParticleEngine::GetInstance();
	//	m_nTestEmitter = m_pPE->LoadBineryEmitter("Resource/Emitters/KQ_DustCload.dat", 128, 128);

	// Register any Events with the GamePlayState
	
		switch(CGame::GetInstance()->GetSelectedCity()->GetID())
		{
		case KCITY1:
			Map->LoadFile("Resource/Levels/KQ_KWACity1.level");

			break;
		case KCITY2:
			Map->LoadFile("Resource/Levels/KQ_KWACity2.level");

			break;
		case KCITY3:
			Map->LoadFile("Resource/Levels/KQ_KWACity3.level");

			break;
		case XCITY1:
			Map->LoadFile("Resource/Levels/KQ_XIACity1.level");

			break;
		case XCITY2:
			Map->LoadFile("Resource/Levels/KQ_XIACity2.level");

			break;
		case XCITY3:
			Map->LoadFile("Resource/Levels/KQ_XIACity3.level");

			break;
		case JCITY1:
			Map->LoadFile("Resource/Levels/KQ_JinCity1.level");

			break;
		case JCITY2:
			Map->LoadFile("Resource/Levels/KQ_JinCity2.level");

			break;
		case JCITY3:
			Map->LoadFile("Resource/Levels/KQ_JinCity3.level");

			break;
		default:
			Map->LoadFile("Resource/Levels/KQ_Level4.level");
			// Bad news bears.

			break;
		}
		

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

		m_nSkyCloudID   = m_pPE->LoadBineryEmitter("Resource/Emitters/KQ_SkyClouds.dat", -10, 200);
		m_nSkyCloudID2  = m_pPE->LoadBineryEmitter("Resource/Emitters/KQ_SkyClouds2.dat", -10, 200);
		CEventSystem::GetInstance()->RegisterClient("Attack_Sound",m_pOM);
		CEventSystem::GetInstance()->RegisterClient("Dying_Sound",m_pOM);

		m_cFont.InitBitmapFont(m_nLucidiaWhiteID, ' ', 16, 128, 128);
		CGame::GetInstance()->SetSongPlay(BATTLESTATE);
		m_pHUD->Enter();

		if(CGame::GetInstance()->GetTutorialMode())
		{
			m_bTutorial = true;
			m_rTutorial.top = 400;
			m_rTutorial.left = 350;
			m_rTutorial.bottom = m_rTutorial.top + 64;
			m_rTutorial.right = m_rTutorial.left + 128;
		}
		else
			m_bTutorial = false;
		Map->LoadImages();
	
}
void CGamePlayState::Exit(void)
{
	PROFILE("CGamePlayState::Exit()");
	m_pTM->ReleaseTexture(m_nButtonID);
	m_pTM->ReleaseTexture(m_nLucidiaWhiteID);
	m_pTM->ReleaseTexture(m_nSelectionID);
	//	CEventSystem::GetInstance()->ClearEvents();
	//Remove all objects from manager?	
	ObjectManager::GetInstance()->RemoveAllObjects();

	for (unsigned int i = 0; i < m_pPE->vEmitterList.size(); ++i)
	{
		m_pPE->SetIsRunning(i, false);
	}
	m_pPE->ClearEmitter();

	m_pHUD->Exit();

	Map->ClearImages();
	//m_pHUD->Enter();
}

bool CGamePlayState::Input(float fElapsedTime)
{
	PROFILE("CGamePlayState::Input(float)");
	if(!m_bTutorial)
	{
		if(!m_bIsPaused)
		{
			m_fJoyTimer += fElapsedTime;
			if(m_pDI->GetBufferedKey(DIK_ESCAPE) || m_pDI->GetBufferedJoyButton(JOYSTICK_R2))
			{
				m_bIsPaused = true;
				if(m_bIsPaused)
					m_pCG->PushState(CPausedState::GetInstance());
			}
			m_pHUD->Input(fElapsedTime);

#pragma region Controller to Mouse

			if(m_pDI->GetJoystickDir(JOYSTICK_UP) && m_pDI->GetJoystickDir(JOYSTICK_LEFT))
			{
				if(m_fJoyTimer > .0002f)
				{
					GetCursorPos(&m_ptMousePos);
					SetCursorPos(m_ptMousePos.x-3,m_ptMousePos.y-3);
					m_fJoyTimer = 0;
				}
			}
			else if(m_pDI->GetJoystickDir(JOYSTICK_UP) && m_pDI->GetJoystickDir(JOYSTICK_RIGHT))
			{
				if(m_fJoyTimer > .0002f)
				{
					GetCursorPos(&m_ptMousePos);
					SetCursorPos(m_ptMousePos.x+3,m_ptMousePos.y-3);
					m_fJoyTimer = 0;
				}	
			}
			else if(m_pDI->GetJoystickDir(JOYSTICK_DOWN) && m_pDI->GetJoystickDir(JOYSTICK_LEFT))
			{
				if(m_fJoyTimer > .0002f)
				{
					GetCursorPos(&m_ptMousePos);
					SetCursorPos(m_ptMousePos.x-3,m_ptMousePos.y+3);
					m_fJoyTimer = 0;
				}	
			}
			else if(m_pDI->GetJoystickDir(JOYSTICK_DOWN) && m_pDI->GetJoystickDir(JOYSTICK_RIGHT))
			{
				if(m_fJoyTimer > .0002f)
				{
					GetCursorPos(&m_ptMousePos);
					SetCursorPos(m_ptMousePos.x+3,m_ptMousePos.y+3);
					m_fJoyTimer = 0;
				}	
			}
			else if(m_pDI->GetJoystickDir(JOYSTICK_UP))
			{
				if(m_fJoyTimer > .0002f)
				{
					GetCursorPos(&m_ptMousePos);
					SetCursorPos(m_ptMousePos.x,m_ptMousePos.y-3);
					m_fJoyTimer = 0;
				}
			}
			else if(m_pDI->GetJoystickDir(JOYSTICK_DOWN))
			{
				if(m_fJoyTimer > .0002f)
				{
					GetCursorPos(&m_ptMousePos);
					SetCursorPos(m_ptMousePos.x,m_ptMousePos.y+3);
					m_fJoyTimer = 0;
				}
			}
			else if(m_pDI->GetJoystickDir(JOYSTICK_LEFT))
			{
				if(m_fJoyTimer > .0002f)
				{
					GetCursorPos(&m_ptMousePos);
					SetCursorPos(m_ptMousePos.x-3,m_ptMousePos.y);
					m_fJoyTimer = 0;
				}
			}
			else if(m_pDI->GetJoystickDir(JOYSTICK_RIGHT))
			{
				if(m_fJoyTimer > .0002f)
				{
					GetCursorPos(&m_ptMousePos);
					SetCursorPos(m_ptMousePos.x+3,m_ptMousePos.y);
					m_fJoyTimer = 0;
				}
			}
#pragma endregion
			if(m_pDI->GetBufferedKey(DIK_F1))
			{
				//Tile  = &Map->GetTile(4,4);
				m_pOM->UpdatePlayerUnitDestTile(Map->GetTile(0,2,4));
			}
			if(m_pDI->GetBufferedKey(DIK_F2))
			{
				//m_pPE->SetPostion(200, 200, m_nTestEmitter);
				//m_pPE->SetIsRunning(m_nTestEmitter, true);
			}

			if(m_pDI->GetBufferedKey(DIK_F3))
			{
				m_pOM->SetSelectedUnitsRetreat();
			}
			/*if(m_pDI->GetBufferedKey(DIK_F8))
			{
			m_pCG->AddWins(false);
			m_pCG->ChangeState(CWorldMapState::GetInstance());
			}*/

			POINT ptMousePos = m_pCG->GetMousePos(); 
			if(ptMousePos.y <= 450 && ((ptMousePos.x > 0 && ptMousePos.x < 800) && (ptMousePos.y > 0 && ptMousePos.y < 600)))
			{
				if(m_pDI->GetBufferedMouseButton(M_BUTTON_LEFT) || m_pDI->GetBufferedJoyButton(JOYSTICK_X))
				{
					m_ptBoxLocation = ptMousePos;
					m_ptCurrentLocation.x = m_ptBoxLocation.x + 1;
					m_ptCurrentLocation.y = m_ptBoxLocation.y +1;
					m_rSelectionBox = GetSelectionRect();
					m_pOM->SetSelectedUnit(m_rSelectionBox);
					// Only update when selected
					CHUDState::GetInstance()->UpdateSelected();
				}
				else if(m_pDI->GetBufferedMouseButton(M_BUTTON_RIGHT) || m_pDI->GetBufferedJoyButton(JOYSTICK_A))
				{
					POINT globleMouse = m_pCamera->TransformToGlobal(ptMousePos.x, ptMousePos.y);
					globleMouse = Map->IsoMouse(globleMouse.x, globleMouse.y, 0);
					m_pOM->UpdatePlayerUnitDestTile(Map->GetTile(0,globleMouse.x, globleMouse.y));
				}
				else if(m_pDI->GetMouseButton(M_BUTTON_LEFT) || m_pDI->GetJoystickButton(JOYSTICK_X))
				{
					if(!m_bButtonDown)
					{
						m_bButtonDown = true;
						m_ptBoxLocation = ptMousePos;
					}
					else
					{	
						m_ptCurrentLocation = m_pCG->GetMousePos();
						m_rSelectionBox = GetSelectionRect();
					}
				}
				if(m_pDI->OnMouseButtonRelease(M_BUTTON_LEFT) || m_pDI->GetBufferedJoyButton(JOYSTICK_B))
				{
					m_pOM->SetSelectedUnit(m_rSelectionBox);
					m_bButtonDown = false;
					m_ptBoxLocation.x = m_ptBoxLocation.y = 0;
					m_ptCurrentLocation.x = m_ptCurrentLocation.y = 0;;
					m_rSelectionBox = GetSelectionRect();
					CHUDState::GetInstance()->UpdateSelected();
				}
			}

#pragma region Camera
			if(!m_bButtonDown)
			{
				m_pCamera->SetVelX(0);
				m_pCamera->SetVelY(0);

				// Mouse Camera Movement
				 //Move camera Left
				if(m_pCG->GetCursorPosition().x <= 5)
					m_pCamera->SetVelX(-200);
				// Move camera Right
				if(m_pCG->GetCursorPosition().x >= 795)
					m_pCamera->SetVelX(200);
				// Move camera Down
				if(m_pCG->GetCursorPosition().y >= 595 )
					m_pCamera->SetVelY(200);
				// Move camera Up
				if(m_pCG->GetCursorPosition().y <= 5)
					m_pCamera->SetVelY(-200); 

				// Keyboard Camera Movement
				// Move camera Left
				if(m_pDI->GetKey((UCHAR)(CKeyBindState::GetInstance()->GetBoundKey(CAMERA_RIGHT))))
					m_pCamera->SetVelX(200);
				// Move camera Right
				if( m_pDI->GetKey((UCHAR)(CKeyBindState::GetInstance()->GetBoundKey(CAMERA_LEFT))))
					m_pCamera->SetVelX(-200);
				// Move camera Down
				if(m_pDI->GetKey((UCHAR)(CKeyBindState::GetInstance()->GetBoundKey(CAMERA_DOWN))))
					m_pCamera->SetVelY(200);
				// Move camera Up
				if(m_pDI->GetKey((UCHAR)(CKeyBindState::GetInstance()->GetBoundKey(CAMERA_UP))))
					m_pCamera->SetVelY(-200); 

			}
#pragma endregion		
		}
	}
	else 
	{
		if(CGame::GetInstance()->IsMouseInRect(m_rTutorial))
		{
			CGame::GetInstance()->SetCursorClick();
			if(m_pDI->GetBufferedMouseButton(M_BUTTON_LEFT)|| m_pDI->GetBufferedJoyButton(JOYSTICK_X))
			{
				m_bTutorial = false;
			}
		}
	}
	return true;
}

void CGamePlayState::Update(float fElapsedTime)
{
	PROFILE("CGamePlayState::Update(float)");
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

	PROFILE("CGamePlayState::Render(float)");

	if(!m_bTutorial)
	{
		// Render units
		// Temp for map changes
		//-----------------------------------------------
		/*if( m_pDI->GetBufferedKey(DIK_1))
		Map->LoadFile("Resource/Levels/KQ_Wawa.level");
		else if( m_pDI->GetBufferedKey(DIK_2))
		Map->LoadFile("Resource/Levels/KQ_Wee.level");
		else if( m_pDI->GetBufferedKey(DIK_3))
		Map->LoadFile("Resource/Levels/KQ_Tech_Demo1.level");*/

		POINT MapLoc = m_pCamera->TransformToGlobal(m_pCG->GetCursorPosition().x, m_pCG->GetCursorPosition().y);

		Map->Render(m_pCamera->GetScreenArea());

#if _DEBUG
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

			POINT miniTileLoc = Map->IsoMiniMouse(m_pCG->GetCursorPosition().x, m_pCG->GetCursorPosition().y, 0);

			sprintf_s(buffer4, 32, "Local Anchor: %i, %i", Map->GetTile(0,TileLoc.x, TileLoc.y)->ptLocalAnchor.x, Map->GetTile(0,TileLoc.x, TileLoc.y)->ptLocalAnchor.y);
			sprintf_s(buffer5, 32, "Global Anchor: %i, %i", MapLoc.x, MapLoc.y);

			m_cFont.DrawTextA(buffer, 500, 0, .2f, .2f);
			m_cFont.DrawTextA(buffer2, 500, 30, .2f, .2f);
			m_cFont.DrawTextA(buffer3, 500, 60, .2f, .2f);
			m_cFont.DrawTextA(buffer4, 500, 90, .2f, .2f);
			m_cFont.DrawTextA(buffer5, 500, 120, .2f, .2f);
			sprintf_s(buffer5, 32, "Minimap Tile: %i, %i", miniTileLoc.x, miniTileLoc.y);
			m_cFont.DrawTextA(buffer5, 500, 150, .2f, .2f);
		}
#endif
		m_pOM->RenderObjects(fElapsedTime);

		// Temp for demo
		m_pTM->Draw(m_nButtonID, m_rVictoryButton.left, m_rVictoryButton.top, .4f, .3f);
		//Map->ParalaxScroll(true, m_pCamera->GetScreenArea());

		m_cFont.DrawTextA("Victory", m_rVictoryButton.left+30, m_rVictoryButton.top+24, .2f, .2f, D3DCOLOR_ARGB(255, 255, 0, 0));
		//----------------------------------------------
		RECT rHud;
		rHud.top = rHud.left = 0;
		rHud.bottom = 600;
		rHud.right = 800;

		POINT pSkyCloud = m_pCamera->TransformToScreen(0, -10);
		m_pPE->SetPostion((float)pSkyCloud.x, (float)pSkyCloud.y, m_nSkyCloudID);
		m_pPE->SetIsRunning(m_nSkyCloudID, true);


		POINT pSkyCloud2 = m_pCamera->TransformToScreen(0, -10);
		m_pPE->SetPostion((float)pSkyCloud2.x, (float)pSkyCloud2.y, m_nSkyCloudID2);
		m_pPE->SetIsRunning(m_nSkyCloudID2, true);


		m_pHUD->Render(fElapsedTime);
		//	m_pTM->Draw(m_nHUD_ID,0,0,1,1,&rHud);

		// Render selection Box
		m_pD3D->SpriteEnd();
		m_pD3D->LineEnd();
		m_pD3D->DeviceEnd();

		//m_pD3D->DrawLine(0,450,800,450,255,0,0);
		m_pD3D->DrawPrimitiveRect(m_rSelectionBox, D3DCOLOR_ARGB(255,255,255,255));

		m_pD3D->DeviceBegin();
		m_pD3D->LineBegin();
		m_pD3D->SpriteBegin();
		//---------------------------------------------
		/*char buffer[128];
		sprintf_s(buffer, 128, "%i, %i", m_pCG->GetCursorPosition().x,m_pCG->GetCursorPosition().y);
		m_cFont.DrawTextA(buffer, 0, 0);*/
	}
	else if(CGame::GetInstance()->GetTutorialMode())
	{
		RECT toDraw; toDraw.top = 0; toDraw.left = 0; toDraw.right = 578; toDraw.bottom = 495;
		int nImage = m_pTM->LoadTexture("Resource/KQ_TutorialBox.png");
		m_pTM->Draw(nImage,0,2,1.4f,1.2f,&toDraw);
		m_nScrollButtonID = m_pTM->LoadTexture("Resource/KQ_ScrollButton.png");
		m_pTM->Draw(m_nScrollButtonID,325,400,.4f,.3f);
		m_cFont.DrawTextA("Accept",350,425,.2f,.2f,D3DCOLOR_ARGB(255,255,0,0));
		m_cFont.DrawTextA("Tutorial",315,15,.4f,.4f,D3DCOLOR_ARGB(255,255,0,0));
		m_cFont.DrawTextA("Press Arrow Keys to move the camera. These /settings are adjustable through the options /menu. Click and hold the left mouse button./Drag the mouse over units to select them./Right click to move the selected units.//The objective here is to seek out all /enemies on the map and defeat them.",80,100,.25f,.25f,D3DCOLOR_ARGB(255,0,0,0));
	}		
}

string CGamePlayState::IntToString(int nNum)
{
	PROFILE("CGamePlayState::IntToString(int)");
	char szNumVal[10];
	itoa(nNum, szNumVal, 10);
	string szNum = szNumVal;
	return szNum;
}

RECT CGamePlayState::GetSelectionRect()
{
	PROFILE("CGamePlayState::GetSelectionRect()");
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
