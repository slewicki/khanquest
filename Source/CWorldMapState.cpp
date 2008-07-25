//////////////////////////////////////////////////////////
//	File:	"CWorldMapState.cpp"
//
//	Author:	Sean Hamstra (SH)
//
//	Purpose: To contain functionality of the world map state
//////////////////////////////////////////////////////////

#include "CWorldMapState.h"
#include "CUnitCreationState.h"
#include "CCityInfoState.h"
#include "CGame.h"

// Temporary
#include "CSGD_Direct3d.h"


CWorldMapState::CWorldMapState(void)
{
	m_dwSelectedColor = D3DCOLOR_ARGB(100, 255, 255, 255);
	m_pSelectedCity = NULL;
	m_nTitleID = -1;
	m_bPaused = false;
}

CWorldMapState::~CWorldMapState(void)
{
	
}


void CWorldMapState::Enter(void)
{
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
	m_cFont.InitBitmapFont(m_nLucidiaWhiteID, ' ', 16, 128, 128);
}

void CWorldMapState::Exit(void)
{
	m_pTM->ReleaseTexture(m_nWorldMapID);
	m_pTM->ReleaseTexture(m_nAttackSymbolID);
	m_pTM->ReleaseTexture(m_nTitleID);
	m_pTM->ReleaseTexture(m_nLucidiaWhiteID);
}

bool CWorldMapState::Input(float fElapsedTime)
{
	// Only get input if we aren't paused
	if(m_bPaused)
		return true;

	if(m_pDI->GetBufferedMouseButton(M_BUTTON_LEFT))
	{
		// Dont bother with main city (i = 0)
		for (int i = 1; i < 10; i++)
		{
			if(	CGame::GetInstance()->IsMouseInRect(m_pCities[i]->GetClickRect()) && 
				m_pCities[i]->IsAttackable())
			{
				// We have a selected city!
				CGame::GetInstance()->SetSelectedCity(m_pCities[i]);
				m_bPaused = true;

				// Push on CCityInfoState
				CGame::GetInstance()->PushState(CCityInfoState::GetInstance());
				break;
			}
		}
	}
	return true;
}

void CWorldMapState::Update(float fElapsedTime)
{
	if(m_bPaused)
		return;
	this->m_ptMousePos = CGame::GetInstance()->GetCursorPosition();
}

void CWorldMapState::Render(float fElapsedTime)
{

	m_pTM->Draw(m_nWorldMapID, 0, 0);//, 1, 1, 0, 0, 0, 0, D3DCOLOR_ARGB(255, 0, 0, 0));

	POINT ptAttackSymbol;
	
	m_pTM->Draw(m_nTitleID, 20, 20, .75, .75);

	for (int i = 0; i < 10; i++)
	{
		m_pTM->Draw(m_pCities[i]->GetImageID(), 0, 0, 1, 1, 0, 0, 0, 0, m_pCities[i]->GetColor());

		if(!m_bPaused && CGame::GetInstance()->IsMouseInRect(m_pCities[i]->GetClickRect()) && m_pCities[i]->IsAttackable())
		{
			m_pTM->Draw(m_pCities[i]->GetImageID(), 0, 0, 1, 1, 0, 0, 0, 0, this->m_dwSelectedColor);
			CGame::GetInstance()->SetCursorClick();
		}
		if(m_pCities[i]->IsAttackable())
		{
			ptAttackSymbol = m_pCities[i]->GetAttackPoint();
			m_pTM->Draw(m_nAttackSymbolID, ptAttackSymbol.x, ptAttackSymbol.y);
		}
	}
	m_cFont.DrawTextA("Mongolia", 240, 155, .25, .25, D3DCOLOR_ARGB(255, 0, 0, 0));

	//------------------------------------
	char szG[10];
	char szF[10];
	itoa(CGame::GetInstance()->GetTotalGold(), szG, 10);
	itoa(CGame::GetInstance()->GetTotalFoodTribute(), szF, 10);
	string szFood = "Food:";
	string szGold = "/Gold:";
	string szGoldVal = szG;
	string szFoodVal = szF;
	m_cFont.DrawTextA(szFood + szFoodVal, 50, 500, .2f, .3f, D3DCOLOR_ARGB(255, 255, 0, 0));
	m_cFont.DrawTextA(szGold + szGoldVal, 50, 505, .2f, .3f, D3DCOLOR_ARGB(255, 255, 255, 0));

	//------------------------------------

}
