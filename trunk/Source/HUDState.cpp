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
	m_nInfantry = m_nCavalry = m_nCavalryArcher = m_nAxmen = m_nArcher = m_nWarElephant = 0;

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
	m_nInfantry = m_nCavalry = m_nCavalryArcher = m_nAxmen = m_nArcher = m_nWarElephant = 0;
	for(unsigned int i = 0; i < ObjectManager::GetInstance()->GetUnits().size(); ++i)
	{
		if(static_cast<CUnit*>(ObjectManager::GetInstance()->GetUnits()[i])->IsPlayerUnit())
		{
			switch(ObjectManager::GetInstance()->GetUnits()[i]->GetType())
			{
			case UNIT_INFANTRY:
				{
					m_nInfantry++;
				}break;
			case UNIT_CAVALRY:
				{
					m_nCavalry++;
				}break;
			case UNIT_CAVALRY_ARCHER:
				{
					m_nCavalryArcher++;
				}break;
			case UNIT_AXMEN:
				{
					m_nAxmen++;
				}break;
			case UNIT_ARCHER:
				{
					m_nArcher++;
				}break;
			case UNIT_WAR_ELEPHANT:
				{
					m_nWarElephant++;
				}break;
			}
		}
	}
}

void CHUDState::Render(float fElapsedTime)
{
	m_pTM->Draw(m_nHUDID,0,0);
	
	RECT InfIcon;
	InfIcon.top = 0; InfIcon.left = 200; InfIcon.right = 250; InfIcon.bottom = 50;
	m_pTM->Draw(m_nIconID,25,470,.75f,.75f,&InfIcon);
	char szInfantryCount[64];
	sprintf(szInfantryCount,"%d",m_nInfantry);
	m_BF.DrawTextA(szInfantryCount,25,500,.3f,.3f,D3DCOLOR_ARGB(255,0,0,0));

	RECT CavIcon;
	CavIcon.top = 100; CavIcon.left = 0; CavIcon.right = 50; CavIcon.bottom = 150;
	m_pTM->Draw(m_nIconID,85,470,.75f,.75f,&CavIcon);
	char szCavCount[64];
	sprintf(szCavCount,"%d",m_nCavalry);
	m_BF.DrawTextA(szCavCount,85,500,.3f,.3f,D3DCOLOR_ARGB(255,0,0,0));

	RECT CAIcon;
	CAIcon.top = 100; CAIcon.left = 200; CAIcon.right = 250; CAIcon.bottom = 150;
	m_pTM->Draw(m_nIconID,150,470,.75f,.75f,&CAIcon);
	char szCACount[64];
	sprintf(szCACount,"%d",m_nCavalryArcher);
	m_BF.DrawTextA(szCACount,150,500,.3f,.3f,D3DCOLOR_ARGB(255,0,0,0));

	RECT AXIcon;
	AXIcon.top = 0; AXIcon.left = 0; AXIcon.right = 50; AXIcon.bottom = 50;
	m_pTM->Draw(m_nIconID,25,535,.75f,.75f,&AXIcon);
	char szAXCount[64];
	sprintf(szAXCount,"%d",m_nAxmen);
	m_BF.DrawTextA(szAXCount,25,565,.3f,.3f,D3DCOLOR_ARGB(255,0,0,0));

	RECT ArcIcon;
	ArcIcon.top = 0; ArcIcon.left = 100; ArcIcon.right = 150; ArcIcon.bottom = 50;
	m_pTM->Draw(m_nIconID,85,535,.75f,.75f,&ArcIcon);
	char szArcCount[64];
	sprintf(szArcCount,"%d",m_nArcher);
	m_BF.DrawTextA(szArcCount,85,565,.3f,.3f,D3DCOLOR_ARGB(255,0,0,0));

	RECT EleIcon;
	EleIcon.top = 100; EleIcon.left = 100; EleIcon.right = 150; EleIcon.bottom = 150;
	m_pTM->Draw(m_nIconID,150,535,.75f,.75f,&EleIcon);
	char szWarElephantCount[64];
	sprintf(szWarElephantCount,"%d",m_nWarElephant);
	m_BF.DrawTextA(szWarElephantCount,150,565,.3f,.3f,D3DCOLOR_ARGB(255,0,0,0));

	if(m_vUnits.size() == 1)
	{
		switch(m_vUnits[0]->GetType())
		{
		case UNIT_INFANTRY:
			{

				RECT InfIcon;
				InfIcon.top = 0; InfIcon.left = 200; InfIcon.right = 250; InfIcon.bottom = 50;
				m_pTM->Draw(m_nIconID,225,475,1.f,1.f,&InfIcon);

				char idk[64];
				
				sprintf_s(idk,"Health: %d",m_vUnits[0]->GetHealth());
				m_BF.DrawTextA(idk, 300, 500,.2f,.2f,D3DCOLOR_ARGB(255,0,0,0));

				sprintf_s(idk,"Attack: %d",m_vUnits[0]->GetAttackPower());
				m_BF.DrawTextA(idk, 300, 520,.2f,.2f,D3DCOLOR_ARGB(255,0,0,0));

				sprintf_s(idk,"Range: %d",m_vUnits[0]->GetRange());
				m_BF.DrawTextA(idk, 300, 540,.2f,.2f,D3DCOLOR_ARGB(255,0,0,0));
			}
			
			break;
		case UNIT_CAVALRY:
			{
				RECT CavIcon;
				CavIcon.top = 100; CavIcon.left = 0; CavIcon.right = 50; CavIcon.bottom = 150;
				m_pTM->Draw(m_nIconID,225,475,1.f,1.f,&CavIcon);

				char idk[64];
				
				sprintf_s(idk,"Health: %d",m_vUnits[0]->GetHealth());
				m_BF.DrawTextA(idk, 300, 500,.2f,.2f,D3DCOLOR_ARGB(255,0,0,0));

				sprintf_s(idk,"Attack: %d",m_vUnits[0]->GetAttackPower());
				m_BF.DrawTextA(idk, 300, 520,.2f,.2f,D3DCOLOR_ARGB(255,0,0,0));

				sprintf_s(idk,"Range: %d",m_vUnits[0]->GetRange());
				m_BF.DrawTextA(idk, 300, 540,.2f,.2f,D3DCOLOR_ARGB(255,0,0,0));
			}
			break;
		case UNIT_CAVALRY_ARCHER:
			{
				RECT CAIcon;
				CAIcon.top = 100; CAIcon.left = 200; CAIcon.right = 250; CAIcon.bottom = 150;
				m_pTM->Draw(m_nIconID,225,475,1.f,1.f,&CAIcon);

				char idk[64];
				
				sprintf_s(idk,"Health: %d",m_vUnits[0]->GetHealth());
				m_BF.DrawTextA(idk, 300, 500,.2f,.2f,D3DCOLOR_ARGB(255,0,0,0));

				sprintf_s(idk,"Attack: %d",m_vUnits[0]->GetAttackPower());
				m_BF.DrawTextA(idk, 300, 520,.2f,.2f,D3DCOLOR_ARGB(255,0,0,0));

				sprintf_s(idk,"Range: %d",m_vUnits[0]->GetRange());
				m_BF.DrawTextA(idk, 300, 540,.2f,.2f,D3DCOLOR_ARGB(255,0,0,0));
			}
			break;
		case UNIT_AXMEN:
			{
				RECT AXIcon;
				AXIcon.top = 0; AXIcon.left = 0; AXIcon.right = 50; AXIcon.bottom = 50;
				m_pTM->Draw(m_nIconID,225,475,1.f,1.f,&AXIcon);

				char idk[64];
				
				sprintf_s(idk,"Health: %d",m_vUnits[0]->GetHealth());
				m_BF.DrawTextA(idk, 300, 500,.2f,.2f,D3DCOLOR_ARGB(255,0,0,0));

				sprintf_s(idk,"Attack: %d",m_vUnits[0]->GetAttackPower());
				m_BF.DrawTextA(idk, 300, 520,.2f,.2f,D3DCOLOR_ARGB(255,0,0,0));

				sprintf_s(idk,"Range: %d",m_vUnits[0]->GetRange());
				m_BF.DrawTextA(idk, 300, 540,.2f,.2f,D3DCOLOR_ARGB(255,0,0,0));
			}
			break;
		case UNIT_ARCHER:
			{
				RECT ArcIcon;
				ArcIcon.top = 0; ArcIcon.left = 100; ArcIcon.right = 150; ArcIcon.bottom = 50;
				m_pTM->Draw(m_nIconID,225,475,1.f,1.f,&ArcIcon);

				char idk[64];
				
				sprintf_s(idk,"Health: %d",m_vUnits[0]->GetHealth());
				m_BF.DrawTextA(idk, 300, 500,.2f,.2f,D3DCOLOR_ARGB(255,0,0,0));

				sprintf_s(idk,"Attack: %d",m_vUnits[0]->GetAttackPower());
				m_BF.DrawTextA(idk, 300, 520,.2f,.2f,D3DCOLOR_ARGB(255,0,0,0));

				sprintf_s(idk,"Range: %d",m_vUnits[0]->GetRange());
				m_BF.DrawTextA(idk, 300, 540,.2f,.2f,D3DCOLOR_ARGB(255,0,0,0));
			}
			break;
		case UNIT_WAR_ELEPHANT:
			{
				RECT EleIcon;
				EleIcon.top = 100; EleIcon.left = 100; EleIcon.right = 150; EleIcon.bottom = 150;
				m_pTM->Draw(m_nIconID,225,475,1.f,1.f,&EleIcon);

				char idk[64];
				
				sprintf_s(idk,"Health: %d",m_vUnits[0]->GetHealth());
				m_BF.DrawTextA(idk, 300, 500,.2f,.2f,D3DCOLOR_ARGB(255,0,0,0));

				sprintf_s(idk,"Attack: %d",m_vUnits[0]->GetAttackPower());
				m_BF.DrawTextA(idk, 300, 520,.2f,.2f,D3DCOLOR_ARGB(255,0,0,0));

				sprintf_s(idk,"Range: %d",m_vUnits[0]->GetRange());
				m_BF.DrawTextA(idk, 300, 540,.2f,.2f,D3DCOLOR_ARGB(255,0,0,0));
			}
		};
		float fHealth = ((float)m_vUnits[0]->GetHealth() / ((float)m_vUnits[0]->GetHP()));
		int nHealthBarLength = (int)Lerp(0.0f, 50.f, fHealth);
		CSGD_Direct3D::GetInstance()->DeviceEnd();
		CSGD_Direct3D::GetInstance()->SpriteEnd();
		RECT rHealthBar;
		rHealthBar.left = 225;
		rHealthBar.bottom = 530;
		rHealthBar.right = rHealthBar.left + 50;
		rHealthBar.top	= 525;
		CSGD_Direct3D::GetInstance()->DrawRect(rHealthBar, 255, 0, 0);
		rHealthBar.right = rHealthBar.left + nHealthBarLength;
		CSGD_Direct3D::GetInstance()->DrawRect(rHealthBar, 0, 255, 0);

		CSGD_Direct3D::GetInstance()->DeviceBegin();
		CSGD_Direct3D::GetInstance()->SpriteBegin();
		return;
	}
	int nPosY = 0;
	int nPosX = 0;
	for(unsigned int i = 0; i < m_vUnits.size(); ++i)
	{
		if(i>=4)
		{
			nPosX = 0;
			nPosY = 60;
		}
		switch(m_vUnits[i]->GetType())
		{
		case UNIT_INFANTRY:
			{

				RECT InfIcon;
				InfIcon.top = 0; InfIcon.left = 200; InfIcon.right = 250; InfIcon.bottom = 50;
				m_pTM->Draw(m_nIconID, 225 + (nPosX * 60),475+nPosY,1.f,1.f,&InfIcon);
				char idk[64];
				sprintf_s(idk,"%d",m_vUnits[i]->GetHealth());
				m_BF.DrawTextA(idk, 225 + (nPosX * 60), 500+nPosY,.25f,.25f,D3DCOLOR_ARGB(255,255,255,255));

				
				float fHealth = ((float)m_vUnits[i]->GetHealth() / ((float)m_vUnits[i]->GetHP()));
				int nHealthBarLength = (int)Lerp(0.0f, 50.f, fHealth);
				CSGD_Direct3D::GetInstance()->DeviceEnd();
				CSGD_Direct3D::GetInstance()->SpriteEnd();
				RECT rHealthBar;
				rHealthBar.left =	225+(nPosX*60);
				rHealthBar.bottom = 530+nPosY;
				rHealthBar.right = rHealthBar.left + 50;
				rHealthBar.top	= rHealthBar.bottom - 5;
				CSGD_Direct3D::GetInstance()->DrawRect(rHealthBar, 255, 0, 0);
				rHealthBar.right = rHealthBar.left + nHealthBarLength;
				CSGD_Direct3D::GetInstance()->DrawRect(rHealthBar, 0, 255, 0);

				CSGD_Direct3D::GetInstance()->DeviceBegin();
				CSGD_Direct3D::GetInstance()->SpriteBegin();
				
			}
			break;
		case UNIT_CAVALRY:
			{
				RECT CavIcon;
				CavIcon.top = 100; CavIcon.left = 0; CavIcon.right = 50; CavIcon.bottom = 150;
				m_pTM->Draw(m_nIconID, 225 + (nPosX * 60),475+nPosY,1.f,1.f,&CavIcon);
				char idk[64];
				sprintf_s(idk,"%d",m_vUnits[i]->GetHealth());
				m_BF.DrawTextA(idk, 225 + (nPosX * 60), 500+nPosY,.25f,.25f,D3DCOLOR_ARGB(255,255,255,255));

				
				float fHealth = ((float)m_vUnits[i]->GetHealth() / ((float)m_vUnits[i]->GetHP()));
				int nHealthBarLength = (int)Lerp(0.0f, 50.f, fHealth);
				CSGD_Direct3D::GetInstance()->DeviceEnd();
				CSGD_Direct3D::GetInstance()->SpriteEnd();
				RECT rHealthBar;
				rHealthBar.left =	225+(nPosX*60);
				rHealthBar.bottom = 530+nPosY;
				rHealthBar.right = rHealthBar.left + 50;
				rHealthBar.top	= rHealthBar.bottom - 5;
				CSGD_Direct3D::GetInstance()->DrawRect(rHealthBar, 255, 0, 0);
				rHealthBar.right = rHealthBar.left + nHealthBarLength;
				CSGD_Direct3D::GetInstance()->DrawRect(rHealthBar, 0, 255, 0);

				CSGD_Direct3D::GetInstance()->DeviceBegin();
				CSGD_Direct3D::GetInstance()->SpriteBegin();
			}
			break;
		case UNIT_CAVALRY_ARCHER:
			{
				RECT CAIcon;
				CAIcon.top = 100; CAIcon.left = 200; CAIcon.right = 250; CAIcon.bottom = 150;
				m_pTM->Draw(m_nIconID, 225 + (nPosX * 60),475+nPosY,1.f,1.f,&CAIcon);
				char idk[64];
				sprintf_s(idk,"%d",m_vUnits[i]->GetHealth());
				m_BF.DrawTextA(idk, 225 + (nPosX * 60), 500+nPosY,.25f,.25f,D3DCOLOR_ARGB(255,255,255,255));

				
				float fHealth = ((float)m_vUnits[i]->GetHealth() / ((float)m_vUnits[i]->GetHP()));
				int nHealthBarLength = (int)Lerp(0.0f, 50.f, fHealth);
				CSGD_Direct3D::GetInstance()->DeviceEnd();
				CSGD_Direct3D::GetInstance()->SpriteEnd();
				RECT rHealthBar;
				rHealthBar.left =	225+(nPosX*60);
				rHealthBar.bottom = 530+nPosY;
				rHealthBar.right = rHealthBar.left + 50;
				rHealthBar.top	= rHealthBar.bottom - 5;
				CSGD_Direct3D::GetInstance()->DrawRect(rHealthBar, 255, 0, 0);
				rHealthBar.right = rHealthBar.left + nHealthBarLength;
				CSGD_Direct3D::GetInstance()->DrawRect(rHealthBar, 0, 255, 0);

				CSGD_Direct3D::GetInstance()->DeviceBegin();
				CSGD_Direct3D::GetInstance()->SpriteBegin();
			}
			break;
		case UNIT_AXMEN:
			{
				RECT AXIcon;
				AXIcon.top = 0; AXIcon.left = 0; AXIcon.right = 50; AXIcon.bottom = 50;
				m_pTM->Draw(m_nIconID, 225 + (nPosX * 60),475+nPosY,1.f,1.f,&AXIcon);
				char idk[64];
				sprintf_s(idk,"%d",m_vUnits[i]->GetHealth());
				m_BF.DrawTextA(idk, 225 + (nPosX * 60), 500+nPosY,.25f,.25f,D3DCOLOR_ARGB(255,255,255,255));

				
				float fHealth = ((float)m_vUnits[i]->GetHealth() / ((float)m_vUnits[i]->GetHP()));
				int nHealthBarLength = (int)Lerp(0.0f, 50.f, fHealth);
				CSGD_Direct3D::GetInstance()->DeviceEnd();
				CSGD_Direct3D::GetInstance()->SpriteEnd();
				RECT rHealthBar;
				rHealthBar.left =	225+(nPosX*60);
				rHealthBar.bottom = 530+nPosY;
				rHealthBar.right = rHealthBar.left + 50;
				rHealthBar.top	= rHealthBar.bottom - 5;
				CSGD_Direct3D::GetInstance()->DrawRect(rHealthBar, 255, 0, 0);
				rHealthBar.right = rHealthBar.left + nHealthBarLength;
				CSGD_Direct3D::GetInstance()->DrawRect(rHealthBar, 0, 255, 0);

				CSGD_Direct3D::GetInstance()->DeviceBegin();
				CSGD_Direct3D::GetInstance()->SpriteBegin();
			}
			break;
		case UNIT_ARCHER:
			{
				RECT ArcIcon;
				ArcIcon.top = 0; ArcIcon.left = 100; ArcIcon.right = 150; ArcIcon.bottom = 50;
				m_pTM->Draw(m_nIconID, 225 + (nPosX * 60),475+nPosY,1.f,1.f,&ArcIcon);
				char idk[64];
				sprintf_s(idk,"%d",m_vUnits[i]->GetHealth());
				m_BF.DrawTextA(idk, 225 + (nPosX * 60), 500+nPosY,.25f,.25f,D3DCOLOR_ARGB(255,255,255,255));

				
				float fHealth = ((float)m_vUnits[i]->GetHealth() / ((float)m_vUnits[i]->GetHP()));
				int nHealthBarLength = (int)Lerp(0.0f, 50.f, fHealth);
				CSGD_Direct3D::GetInstance()->DeviceEnd();
				CSGD_Direct3D::GetInstance()->SpriteEnd();
				RECT rHealthBar;
				rHealthBar.left =	225+(nPosX*60);
				rHealthBar.bottom = 530+nPosY;
				rHealthBar.right = rHealthBar.left + 50;
				rHealthBar.top	= rHealthBar.bottom - 5;
				CSGD_Direct3D::GetInstance()->DrawRect(rHealthBar, 255, 0, 0);
				rHealthBar.right = rHealthBar.left + nHealthBarLength;
				CSGD_Direct3D::GetInstance()->DrawRect(rHealthBar, 0, 255, 0);

				CSGD_Direct3D::GetInstance()->DeviceBegin();
				CSGD_Direct3D::GetInstance()->SpriteBegin();
			}
			break;
		case UNIT_WAR_ELEPHANT:
			{
				RECT EleIcon;
				EleIcon.top = 100; EleIcon.left = 100; EleIcon.right = 150; EleIcon.bottom = 150;
				m_pTM->Draw(m_nIconID, 225 + (nPosX * 60),475+nPosY,1.f,1.f,&EleIcon);
				char idk[64];
				sprintf_s(idk,"%d",m_vUnits[i]->GetHealth());
				m_BF.DrawTextA(idk, 225 + (nPosX * 60), 500+nPosY,.25f,.25f,D3DCOLOR_ARGB(255,255,255,255));

				
				float fHealth = ((float)m_vUnits[i]->GetHealth() / ((float)m_vUnits[i]->GetHP()));
				int nHealthBarLength = (int)Lerp(0.0f, 50.f, fHealth);
				CSGD_Direct3D::GetInstance()->DeviceEnd();
				CSGD_Direct3D::GetInstance()->SpriteEnd();
				RECT rHealthBar;
				rHealthBar.left =	225+(nPosX*60);
				rHealthBar.bottom = 530+nPosY;
				rHealthBar.right = rHealthBar.left + 50;
				rHealthBar.top	= rHealthBar.bottom - 5;
				CSGD_Direct3D::GetInstance()->DrawRect(rHealthBar, 255, 0, 0);
				rHealthBar.right = rHealthBar.left + nHealthBarLength;
				CSGD_Direct3D::GetInstance()->DrawRect(rHealthBar, 0, 255, 0);

				CSGD_Direct3D::GetInstance()->DeviceBegin();
				CSGD_Direct3D::GetInstance()->SpriteBegin();
			}
			break;
		default:
			{
				//Shouldnt get here;
			}
		};
		nPosX++;
	}
}


void CHUDState::UpdateSelected()
{
	m_vUnits.clear();
	if(ObjectManager::GetInstance()->GetSelectedUnits().size() == 0)
	{
		m_vUnits.clear();
		return;
	}
	for(unsigned int i = 0; i < ObjectManager::GetInstance()->GetSelectedUnits().size(); i++)
		m_vUnits.push_back(static_cast<CUnit*>(ObjectManager::GetInstance()->GetSelectedUnits()[i]));
}