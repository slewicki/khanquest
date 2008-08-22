//////////////////////////////////////////////////////
//	File	:	"CCity.cpp"
//
//	Author	:	Sean Hamstra (SH)
//
//	Purpose	:	Store all information and functionality
//				of a city.
//////////////////////////////////////////////////////
#include "CCity.h"

CCity::CCity() 
{
	PROFILE("CCity::CCity()");
	m_nImageID = -1;
	m_nGoldTribute = 0;
	m_nCurrentOwner = -1;
	m_dwColor = 0;
	m_bIsAttackable = false;
	m_ptAttackPoint.x = 0;
	m_ptAttackPoint.y = 0;
}
CCity::~CCity() 
{
	PROFILE("CCity::~CCity()");
	CSGD_TextureManager::GetInstance()->ReleaseTexture(m_nImageID);
}

void CCity::SetDefaultColor(int nCityID)
{
	PROFILE("CCity::SetDefaultColor(int)");
	switch(nCityID)
		{
		case PCITY:
			m_dwColor = D3DCOLOR_ARGB(255, 0, 255, 255); 
			break;
		case KCITY1:
			m_dwColor = D3DCOLOR_ARGB(255, 120, 255, 120); 
			break;
		case KCITY2:
			m_dwColor = D3DCOLOR_ARGB(255, 0, 255, 0); 
			break;
		case KCITY3:
			m_dwColor = D3DCOLOR_ARGB(255, 0, 170, 0); 
			break;
		case XCITY1:
			m_dwColor = D3DCOLOR_ARGB(255, 255, 180, 0); 
			break;
		case XCITY2:
			m_dwColor = D3DCOLOR_ARGB(255, 255, 128, 0); 
			break;
		case XCITY3:
			m_dwColor = D3DCOLOR_ARGB(255, 255, 90, 0); 
			break;
		case JCITY1:
			m_dwColor = D3DCOLOR_ARGB(255, 209, 147, 255); 
			break;
		case JCITY2:
			m_dwColor = D3DCOLOR_ARGB(255, 178, 0, 255); 
			break;
		case JCITY3:
			m_dwColor = D3DCOLOR_ARGB(255, 132, 0, 140); 
			break;
		}
}
