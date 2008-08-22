//////////////////////////////////////////////////////////
//	File:	"CWorldMapState.cpp"
//
//	Author:	Sean Hamstra (SH)
//
//	Purpose: To contain functionality of the world map state
//////////////////////////////////////////////////////////

#include "CWorldMapState.h"
#include "CGame.h"
#include "CUnitCreationState.h"
#include "CCityInfoState.h"
#include "CGamePlayState.h"
#include "MainMenuState.h"
// Temporary
#include "CSGD_Direct3d.h"
#include "CProfile.h"


CWorldMapState::CWorldMapState(void)
{
	PROFILE("CWorldMapState::CWorldMapState()");
	m_dwSelectedColor = D3DCOLOR_ARGB(100, 255, 255, 255);
	m_pSelectedCity = NULL;
	m_nTitleID = -1;
	m_bPaused = false;

	m_clistCheatCode[0].push_back('k');
	m_clistCheatCode[0].push_back('h');
	m_clistCheatCode[0].push_back('a');
	m_clistCheatCode[0].push_back('n');

	m_clistCheatCode[1].push_back('m');
	m_clistCheatCode[1].push_back('e');
	m_clistCheatCode[1].push_back('a');
	m_clistCheatCode[1].push_back('t');

	m_clistCheatCode[2].push_back('l');
	m_clistCheatCode[2].push_back('o');
	m_clistCheatCode[2].push_back('r');
	m_clistCheatCode[2].push_back('d');
	m_fJoyTimer = 0;
	m_ptMousePos.x = m_ptMousePos.y = 0;
}

CWorldMapState::~CWorldMapState(void)
{

}


void CWorldMapState::Enter(void)
{
	PROFILE("CWorldMapState::Enter()");
	// Get Our Managers Ready
	m_pTM = CSGD_TextureManager::GetInstance();
	m_pWM = CSGD_WaveManager::GetInstance();
	m_pDI = CSGD_DirectInput::GetInstance();
	// Register any Events with the CWorldMapState

	m_bPaused = false;

	// Get our city info from CGame
	this->m_pCities = CGame::GetInstance()->GetCities();
	for (int i = 0; i < 10; i++)
	{
		m_pCities[i]->SetAttackable(false);
	}
	// Figure out what our player owns
	m_pPlayerOwnedCities.clear();
	for (unsigned int i = 0; i < 10; i++)
	{
		if(m_pCities[i]->GetOwner() == PLAYER_CITY)
		{
			m_pPlayerOwnedCities.push_back(m_pCities[i]);
			vector<int> vAdjacent = m_pCities[i]->GetAdjacent();
			for (unsigned int i = 0; i < vAdjacent.size(); i++)
			{
				if(m_pCities[vAdjacent[i]]->GetOwner() != PLAYER_CITY)
					m_pCities[vAdjacent[i]]->SetAttackable(true);
			}
		}
	}
	// Load textures
	m_nWorldMapID = m_pTM->LoadTexture("Resource/KQ_WorldMap.png");
	m_nAttackSymbolID = m_pTM->LoadTexture("Resource/KQ_AttackSymbol.png");
	m_nTitleID = m_pTM->LoadTexture("Resource/KQ_Title.png");
	m_nLucidiaWhiteID = m_pTM->LoadTexture("Resource/KQ_FontLucidiaWhite.png");
	m_nClick = m_pWM->LoadWave("Resource/KQ_Click.wav");
	m_cFont.InitBitmapFont(m_nLucidiaWhiteID, ' ', 16, 128, 128);
	CGame::GetInstance()->SetSongPlay(CITYSELECT);
	m_fJoyTimer = 0;

}

void CWorldMapState::Exit(void)
{
	PROFILE("CWorldMapState::Exit()");
	if(m_pWM->IsWavePlaying(m_nClick))
		m_pWM->Stop(m_nClick);

	m_pWM->UnloadWave(m_nClick);

	m_pTM->ReleaseTexture(m_nWorldMapID);
	m_pTM->ReleaseTexture(m_nAttackSymbolID);
	m_pTM->ReleaseTexture(m_nTitleID);
	m_pTM->ReleaseTexture(m_nLucidiaWhiteID);
}

bool CWorldMapState::Input(float fElapsedTime)
{
	PROFILE("CWorldMapState::Input(float)");
	// Only get input if we aren't paused
	if(m_bPaused)
		return true;
	
	m_fJoyTimer = fElapsedTime;
#pragma region Controller to Mouse
	
	if(m_pDI->GetJoystickDir(JOYSTICK_UP) && m_pDI->GetJoystickDir(JOYSTICK_LEFT))
	{
		if(m_fJoyTimer > .002f)
		{
			GetCursorPos(&m_ptMousePos);
			SetCursorPos(m_ptMousePos.x-3,m_ptMousePos.y-3);
			m_fJoyTimer = 0;
		}
	}
	else if(m_pDI->GetJoystickDir(JOYSTICK_UP) && m_pDI->GetJoystickDir(JOYSTICK_RIGHT))
	{
		if(m_fJoyTimer > .002f)
		{
			GetCursorPos(&m_ptMousePos);
			SetCursorPos(m_ptMousePos.x+3,m_ptMousePos.y-3);
			m_fJoyTimer = 0;
		}	
	}
	else if(m_pDI->GetJoystickDir(JOYSTICK_DOWN) && m_pDI->GetJoystickDir(JOYSTICK_LEFT))
	{
		if(m_fJoyTimer > .002f)
		{
			GetCursorPos(&m_ptMousePos);
			SetCursorPos(m_ptMousePos.x-3,m_ptMousePos.y+3);
			m_fJoyTimer = 0;
		}	
	}
	else if(m_pDI->GetJoystickDir(JOYSTICK_DOWN) && m_pDI->GetJoystickDir(JOYSTICK_RIGHT))
	{
		if(m_fJoyTimer > .002f)
		{
			GetCursorPos(&m_ptMousePos);
			SetCursorPos(m_ptMousePos.x+3,m_ptMousePos.y+3);
			m_fJoyTimer = 0;
		}	
	}
	else if(m_pDI->GetJoystickDir(JOYSTICK_UP))
	{
		if(m_fJoyTimer > .002f)
		{
			GetCursorPos(&m_ptMousePos);
			SetCursorPos(m_ptMousePos.x,m_ptMousePos.y-3);
			m_fJoyTimer = 0;
		}
	}
	else if(m_pDI->GetJoystickDir(JOYSTICK_DOWN))
	{
		if(m_fJoyTimer > .002f)
		{
			GetCursorPos(&m_ptMousePos);
			SetCursorPos(m_ptMousePos.x,m_ptMousePos.y+3);
			m_fJoyTimer = 0;
		}
	}
	else if(m_pDI->GetJoystickDir(JOYSTICK_LEFT))
	{
		if(m_fJoyTimer > .002f)
		{
			GetCursorPos(&m_ptMousePos);
			SetCursorPos(m_ptMousePos.x-3,m_ptMousePos.y);
			m_fJoyTimer = 0;
		}
	}
	else if(m_pDI->GetJoystickDir(JOYSTICK_RIGHT))
	{
		if(m_fJoyTimer > .002f)
		{
			GetCursorPos(&m_ptMousePos);
			SetCursorPos(m_ptMousePos.x+3,m_ptMousePos.y);
			m_fJoyTimer = 0;
		}
	}


#pragma endregion
	// Exit game for now
	//-----------------------------------------
	if(m_pDI->GetBufferedKey(DIK_ESCAPE) || m_pDI->GetJoystickButton(JOYSTICK_Y))
		CGame::GetInstance()->ChangeState(CMainMenuState::GetInstance());
	//-----------------------------------------

	// Cheat Codes
	//-----------------------------------------
	char keyPressed = m_pDI->CheckBufferedKeys();

	if(keyPressed)
	{
		m_clistInputQueue.push_back(keyPressed);

		if(m_clistInputQueue.size() > 4) 
			m_clistInputQueue.pop_front();

		if(m_clistInputQueue == m_clistCheatCode[0])
		{
			CGame::GetInstance()->SetTerrorLevel(100);
		}
		else if(m_clistInputQueue == m_clistCheatCode[1])
		{
		}
		else if (m_clistInputQueue == m_clistCheatCode[2])
		{
			CGame::GetInstance()->AddGold(500);
		}
	}
	//-----------------------------------------

	// Mouse Input
	//-----------------------------------------

	if(m_pDI->GetBufferedMouseButton(M_BUTTON_LEFT) || m_pDI->GetBufferedJoyButton(JOYSTICK_X))
	{
		// Dont bother with main city (i = 0)
		for (int i = 1; i < 10; i++)
		{
			if(	CGame::GetInstance()->IsMouseInRect(m_pCities[i]->GetClickRect()) && 
				m_pCities[i]->IsAttackable())
			{
				m_pWM->Play(m_nClick);
				// We have a selected city!
				CGame::GetInstance()->SetSelectedCity(m_pCities[i]);
				m_bPaused = true;

				if(CGame::GetInstance()->GetTerrorLevel() < 100)
				{
					// Push on CCityInfoState
					CGame::GetInstance()->PushState(CCityInfoState::GetInstance());
					break;
				}
				else
				{
					CGame::GetInstance()->SetCityConquered(m_pCities[i]);
					
					m_pPlayerOwnedCities.clear();
					for (unsigned int i = 0; i < 10; i++)
					{
						if(m_pCities[i]->GetOwner() == PLAYER_CITY)
						{
							m_pPlayerOwnedCities.push_back(m_pCities[i]);
							vector<int> vAdjacent = m_pCities[i]->GetAdjacent();
							for (unsigned int i = 0; i < vAdjacent.size(); i++)
							{
								if(m_pCities[vAdjacent[i]]->GetOwner() != PLAYER_CITY)
									m_pCities[vAdjacent[i]]->SetAttackable(true);
							}
						}
					}
					CGame::GetInstance()->SetTerrorLevel(0);
					m_bPaused = false;
					break;
				}
			}
		}
	}
	return true;
}

void CWorldMapState::Update(float fElapsedTime)
{
	PROFILE("CWorldMapState::Update(float)");
	if(m_bPaused)
		return;
	this->m_ptMousePos = CGame::GetInstance()->GetCursorPosition();
}

void CWorldMapState::Render(float fElapsedTime)
{
	PROFILE("CWorldMapState::Render(float)");
	m_pTM->Draw(m_nWorldMapID, 0, 0);
	POINT ptAttackSymbol;

	for (int i = 0; i < 10; i++)
	{
		m_pTM->Draw(m_pCities[i]->GetImageID(), 0, 0, 1, 1, 0, 0, 0, 0, m_pCities[i]->GetColor());

		if(!m_bPaused && CGame::GetInstance()->IsMouseInRect(m_pCities[i]->GetClickRect()) && m_pCities[i]->IsAttackable())
		{
			m_pTM->Draw(m_pCities[i]->GetImageID(), 0, 0, 1, 1, 0, 0, 0, 0, this->m_dwSelectedColor);
			CGame::GetInstance()->SetCursorClick();
			switch(m_pCities[i]->GetOwner())
			{
			case XIA_CITY: // Xia Territory"
				m_cFont.DrawTextA("Xia Territory", 20, 20, .25f, .25f, D3DCOLOR_ARGB(255, 255, 128, 0));
				break;
			case JIN_CITY:
				m_cFont.DrawTextA("Jin Territory", 20, 20, .25f, .25f, D3DCOLOR_ARGB(255, 178, 0, 255));
				break;
			case K_CITY:
				m_cFont.DrawTextA("Khwarezmian Territory", 20, 20, .25f, .25f, D3DCOLOR_ARGB(255, 0, 255, 0));
				break;
			}
		}
		if(m_pCities[i]->IsAttackable())
		{
			ptAttackSymbol = m_pCities[i]->GetAttackPoint();
			m_pTM->Draw(m_nAttackSymbolID, ptAttackSymbol.x, ptAttackSymbol.y);
		}
	}
	m_cFont.DrawTextA("Mongolia", 300, 155, .25, .25, D3DCOLOR_ARGB(255, 0, 0, 0));

	//------------------------------------
	// Display player's total Food and Gold Currently
	char szG[10];
	char szF[10];
	itoa(CGame::GetInstance()->GetTotalGold(), szG, 10);
	itoa(CGame::GetInstance()->GetTotalFoodTribute(), szF, 10);
	string szFood = "Food:";
	string szGold = "Gold:";
	string szGoldVal = szG;
	string szFoodVal = szF;

	string szTerrorLevel = "Terror: ";
	m_cFont.DrawTextA(szTerrorLevel + IntToString(CGame::GetInstance()->GetTerrorLevel()) + "%", 50,550, 0.2f,0.2f, D3DCOLOR_ARGB(255,255,0,0));
	m_cFont.DrawTextA(szFood + szFoodVal, 50, 500, .2f, .2f, D3DCOLOR_ARGB(255, 120, 60, 0));
	m_cFont.DrawTextA(szGold + szGoldVal, 55, 525, .2f, .2f, D3DCOLOR_ARGB(255, 255, 255, 0));

	//------------------------------------

}
string CWorldMapState::IntToString(int nNum)
{
	PROFILE("CWorldMapState::IntToString(int)");
	char szNumVal[10];
	itoa(nNum, szNumVal, 10);
	string szNum = szNumVal;
	return szNum;
}
