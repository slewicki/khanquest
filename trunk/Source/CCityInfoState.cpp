//////////////////////////////////////////////////////////
//	File:	"CCityInfoState.cpp"
//
//	Author:	Sean Hamstra (SH)
//
//	Purpose: To contain functionality of the city info state
//////////////////////////////////////////////////////////

#include "CCityInfoState.h"
#include "CWorldMapState.h"
#include "CUnitCreationState.h"
#include "CGame.h"


CCityInfoState::CCityInfoState(void)
{
	m_pSelectedCity = NULL;
	m_bRetract = false;
	m_bClickInvade = false;
}


CCityInfoState::~CCityInfoState(void)
{
	
}


void CCityInfoState::Enter(void)
{
	// Get Our Managers Ready
	m_bRetract = false;
	m_bClickInvade = false;
	m_pTM = CSGD_TextureManager::GetInstance();
	m_pWM = CSGD_WaveManager::GetInstance();
	m_pDI = CSGD_DirectInput::GetInstance();

	// Invade Button Rectangle
	m_rInvade.left = 357;
	m_rInvade.top = 470;
	m_rInvade.right = 487;
	m_rInvade.bottom = 530;

	// Cancel Button Rectangle
	m_rCancel.left = 570;
	m_rCancel.top = 470;
	m_rCancel.right = 700;
	m_rCancel.bottom = 530;

	m_pSelectedCity = CGame::GetInstance()->GetSelectedCity();

	m_fPositionX = 800.f;
	// Register any Events with the CCityInfoState
	switch(m_pSelectedCity->GetOwner())
	{
	case K_CITY:
		m_nDisplayID = m_pTM->LoadTexture("Resource/KQ_KInfoBkg.png");
		m_szTitle = " KHWAREZMIAN";
		m_szDescription = 
		"The Khwarezmian Empire controls this city.  Their /strength lies in their entirely mounted army.  Be /wary of their War Elephants, Khan, for their power /is unmatched in the known world.";
		break;
	case XIA_CITY:
		m_nDisplayID = m_pTM->LoadTexture("Resource/KQ_XiaInfoBkg.png");
		m_szTitle = "          XIA";
		m_szDescription = 
		"The Xia Dynasty controls this city.  Their axmen are /vicious barbarians, thirsty for blood.  They travel /only by foot, Sire, and we should take advantage /of that."; 
		
		break;
	case JIN_CITY:
		m_nDisplayID = m_pTM->LoadTexture("Resource/KQ_JinInfoBkg.png");
		m_szTitle = "          JIN";
		m_szDescription = 
		"The Jin Dynasty controls this city.  The Jin boast of /their untouchable, ranged army.  They live and die /by their archers, my lord, surely we will put them to /the test.";
		break;
	default:
		// We got a problem here
		CGame::GetInstance()->PopCurrentState();
	}
	m_nButtonID = m_pTM->LoadTexture("Resource/KQ_ScrollButton.png");
	m_nFontID = m_pTM->LoadTexture("Resource/KQ_FontLucidiaWhite.png");
	m_cFont.InitBitmapFont(m_nFontID, ' ', 16, 128, 128);
	
}

void CCityInfoState::Exit(void)
{
	m_pTM->ReleaseTexture(m_nDisplayID);
	m_pTM->ReleaseTexture(m_nButtonID);
	m_pTM->ReleaseTexture(m_nFontID);
}

bool CCityInfoState::Input(float fElapsedTime)
{

	if(m_fPositionX <= 270.f && !m_bRetract)
	{
		if(CGame::GetInstance()->IsMouseInRect(m_rInvade))
		{
//			CGame::GetInstance()->SetCursorClick();
			if(m_pDI->GetBufferedMouseButton(M_BUTTON_LEFT))
			{
				// This function is used only if the city is conquered after battle
				//--------------------
				//CGame::GetInstance()->SetCityConquered(m_pSelectedCity);
				//--------------------
				m_bRetract = true;
				m_bClickInvade = true;
				
			}
		}
		else if(CGame::GetInstance()->IsMouseInRect(m_rCancel))
		{
//			CGame::GetInstance()->SetCursorClick();
			if(m_pDI->GetBufferedMouseButton(M_BUTTON_LEFT))
			{
				m_bRetract = true;
				// This function is used only if the player lost 2 battles
				//--------------------
				//CGame::GetInstance()->LoseLastCity();
				//--------------------
			}
		}

	}
	return true;
}

void CCityInfoState::Update(float fElapsedTime)
{
	if(m_bRetract)
	{
		m_fPositionX  += 500*fElapsedTime;
		if(m_fPositionX > 805)
		{
			if(m_bClickInvade)
				CGame::GetInstance()->ChangeState(CUnitCreationState::GetInstance());
			else
				CGame::GetInstance()->ChangeState(CWorldMapState::GetInstance());
		}
	}
	else if(m_fPositionX >= 270)
		m_fPositionX  -= 500*fElapsedTime;
	
}

void CCityInfoState::Render(float fElapsedTime)
{
	m_pTM->Draw(m_nDisplayID, (int)m_fPositionX, 20);
	m_pTM->Draw(m_nButtonID, (int)m_fPositionX+87, 465, .4f, .3f);
	m_cFont.DrawTextA("INVADE", (int)m_fPositionX+99, 489, .2f, .2f, D3DCOLOR_ARGB(255, 255, 0, 0));
	m_pTM->Draw(m_nButtonID, (int)m_fPositionX+300, 465, .4f, .3f);
	m_cFont.DrawTextA("CANCEL", (int)m_fPositionX+318, 489, .2f, .2f, D3DCOLOR_ARGB(255, 255, 0, 0));
	
	m_cFont.DrawTextA(m_szTitle, (int)m_fPositionX+90, 40, .3f, .3f, D3DCOLOR_ARGB(255, 0, 0, 0));
	
	char szG[10];
	char szF[10];
	itoa(m_pSelectedCity->GetGoldTribute(), szG, 10);
	itoa(CGame::GetInstance()->GetNextFoodTribute(), szF, 10);

	// Print Food and Gold values gained from conquering this city
	string szFood = "Food Tribute:  ";
	string szGold = "/Sackable Gold: ";
	string szGoldVal = szG;
	string szFoodVal = szF;
	m_cFont.DrawTextA(szFood + szFoodVal + szGold + szGoldVal, (int)m_fPositionX+50, 275, .15f, .15f, D3DCOLOR_ARGB(255, 0, 0, 0));
	
	// Scout Report: Hints about the city
	m_cFont.DrawTextA("Scout's Report", (int)m_fPositionX+190, 325, .18f, .18f, D3DCOLOR_ARGB(255, 0, 0, 0));
	m_cFont.DrawTextA(m_szDescription, (int)m_fPositionX+50, 355, .15f, .15f, D3DCOLOR_ARGB(255, 0, 0, 0));

}
