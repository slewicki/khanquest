//////////////////////////////////////////////////////////
//	File:	"CHUDState.h"
//
//	Author:	Dennis Wallace (DW)
//
//	Purpose: To contain functionality of the CHUDState state
//////////////////////////////////////////////////////////
#include "HUDState.h"
#include "CUnit.h"
#include "ObjectManager.h"

CHUDState::CHUDState(void)
{
}

CHUDState::~CHUDState(void)
{
}
void CHUDState::Enter(void)
{
	m_pTM = CSGD_TextureManager::GetInstance();
	int nFontID = m_pTM->LoadTexture("Resource/KQ_FontLucidiaWhite.png");
	m_BF.InitBitmapFont(nFontID,' ',16,128,128);
	m_nHUDID = m_pTM->LoadTexture("Resource/KQ_HUD.png");
	m_nIconID = m_pTM->LoadTexture("Resource/KQ_UnitIcons.png");
}

void CHUDState::Exit(void)
{
	UpdateSelected();
}

bool CHUDState::Input(float fElapsedTime)
{

	return true;
}

void CHUDState::Update(float fElapsedTime)
{

}

void CHUDState::Render(float fElapsedTime)
{
	m_pTM->Draw(m_nHUDID,0,0);

	for(unsigned int i = 0; i < m_vUnits.size(); ++i)
	{
		switch(m_vUnits[i]->GetType())
		{
		case UNIT_INFANTRY:
			{

				RECT InfIcon;
				InfIcon.top = 0; InfIcon.left = 200; InfIcon.right = 250; InfIcon.bottom = 50;
				m_pTM->Draw(m_nIconID,225 + (i * 50),475,.75f,.75f,&InfIcon);
				char idk[64];
				sprintf_s(idk,"%d",m_vUnits[i]->GetHealth());
				m_BF.DrawTextA(idk, 225 + (i * 50), 500,.25f,.25f,D3DCOLOR_ARGB(255,255,255,255));
			}
			break;
		case UNIT_CAVALRY:
			{
				RECT CavIcon;
				CavIcon.top = 100; CavIcon.left = 0; CavIcon.right = 50; CavIcon.bottom = 150;
				m_pTM->Draw(m_nIconID,225 + (i * 50),475,.75f,.75f,&CavIcon);		
				char idk[64];
				sprintf_s(idk,"%d",m_vUnits[i]->GetHealth());
				m_BF.DrawTextA(idk, 225 + (i * 50), 500,.25f,.25f,D3DCOLOR_ARGB(255,255,255,255));
			}
			break;
		case UNIT_CAVALRY_ARCHER:
			{
				RECT CAIcon;
				CAIcon.top = 100; CAIcon.left = 200; CAIcon.right = 250; CAIcon.bottom = 150;
				m_pTM->Draw(m_nIconID,225 + (i * 50),475,.75f,.75f,&CAIcon);		
				char idk[64];
				sprintf_s(idk,"%d",m_vUnits[i]->GetHealth());
				m_BF.DrawTextA(idk, 225 + (i * 50), 500,.25f,.25f,D3DCOLOR_ARGB(255,255,255,255));
			}
			break;
		case UNIT_AXMEN:
			{
				RECT AXIcon;
				AXIcon.top = 0; AXIcon.left = 0; AXIcon.right = 50; AXIcon.bottom = 50;
				m_pTM->Draw(m_nIconID,225 + (i * 50),475,.75f,.75f,&AXIcon);	
				char idk[64];
				sprintf_s(idk,"%d",m_vUnits[i]->GetHealth());
				m_BF.DrawTextA(idk, 225 + (i * 50), 500,.25f,.25f,D3DCOLOR_ARGB(255,255,255,255));
			}
			break;
		case UNIT_ARCHER:
			{
				RECT ArcIcon;
				ArcIcon.top = 0; ArcIcon.left = 100; ArcIcon.right = 150; ArcIcon.bottom = 50;
				m_pTM->Draw(m_nIconID,225 + (i * 50),475,.75f,.75f,&ArcIcon);	
				char idk[64];
				sprintf_s(idk,"%d",m_vUnits[i]->GetHealth());
				m_BF.DrawTextA(idk, 225 + (i * 50), 500,.25f,.25f,D3DCOLOR_ARGB(255,255,255,255));
			}
			break;
		case UNIT_WAR_ELEPHANT:
			{
				RECT EleIcon;
				EleIcon.top = 100; EleIcon.left = 100; EleIcon.right = 150; EleIcon.bottom = 150;
				m_pTM->Draw(m_nIconID,225 + (i * 50),475,.75f,.75f,&EleIcon);	
				char idk[64];
				sprintf_s(idk,"%d",m_vUnits[i]->GetHealth());
				m_BF.DrawTextA(idk, 225 + (i * 50), 500,.25f,.25f,D3DCOLOR_ARGB(255,255,255,255));
			}
			break;
		default:
			{
				//Shouldnt get here;
			}
		};
	}
}

void CHUDState::UpdateSelected()
{
	if(ObjectManager::GetInstance()->GetSelectedUnits().size() == 0)
	{
		m_vUnits.clear();
		return;
	}
	for(unsigned int i = 0; i < ObjectManager::GetInstance()->GetSelectedUnits().size(); i++)
		m_vUnits.push_back(static_cast<CUnit*>(ObjectManager::GetInstance()->GetSelectedUnits()[i]));
}