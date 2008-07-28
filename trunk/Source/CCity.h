//////////////////////////////////////////////////////
//	File	:	"CCity.h"
//
//	Author	:	Sean Hamstra (SH)
//
//	Purpose	:	Store all information and functionality
//				of a city.
//////////////////////////////////////////////////////

#pragma once
#include "CSGD_TextureManager.h"

enum { PLAYER_CITY, XIA_CITY, K_CITY, JIN_CITY };
enum { PCITY, KCITY1, KCITY2, KCITY3, XCITY1, XCITY2, XCITY3, JCITY1, JCITY2, JCITY3 };

class CCity
{
private:
	int			m_nImageID;
	int			m_nCityID;
	int			m_nGoldTribute;
	int			m_nCurrentOwner;
	int			m_nOriginalOwner;
	DWORD		m_dwColor;
	RECT		m_rClickRect;
	bool		m_bIsAttackable;
	POINT		m_ptAttackPoint;
	vector<int>	m_vAdjacentCities;
public:
	//////////////////////////////////////////////////////
	//	Function:	"CCity(Constructor)"
	//	Last Modified: July 22, 2008
	//	Purpose:	Constructor
	//////////////////////////////////////////////////////
	CCity();

	//////////////////////////////////////////////////////
	//	Function:	"CCity(Constructor)"
	//	Last Modified: July 22, 2008
	//	Purpose:	Constructor
	//////////////////////////////////////////////////////
	~CCity();

	//////////////////////////////////////////////////////
	//	Function:	"SetDefaultColor
	//	Last Modified: July 22, 2008
	//	Purpose:	Sets the city color to its default based
	//				on its ID.
	//////////////////////////////////////////////////////
	void SetDefaultColor(int nCityID);
	
	//////////////////////////////////////////////////////
	//	Function:	Accessors
	//	Last Modified: July 22, 2008
	//	Purpose:	Gets the specified type
	//////////////////////////////////////////////////////
	inline int GetGoldTribute() { return m_nGoldTribute; }
	inline int GetOwner() { return m_nCurrentOwner; }
	inline int GetImageID() { return m_nImageID; }
	inline int GetColor() { return m_dwColor; }
	inline int GetID() { return m_nCityID; }
	inline RECT GetClickRect() { return m_rClickRect; }
	inline bool IsAttackable() { return m_bIsAttackable; }
	inline POINT GetAttackPoint() { return m_ptAttackPoint; }
	inline vector<int> GetAdjacent() { return m_vAdjacentCities; }


	//////////////////////////////////////////////////////
	//	Function:	Mutators
	//	Last Modified: July 22, 2008
	//	Purpose:	Sets the specified type
	//////////////////////////////////////////////////////
	inline void SetGoldTribute(int nGoldTribute) { m_nGoldTribute = nGoldTribute; }
	inline void SetColor(DWORD dwColor) { m_dwColor = dwColor; }
	inline void SetID(int nCityID) { m_nCityID = nCityID; }
	inline void SetAttackable(bool nIsAttackable) { m_bIsAttackable = nIsAttackable; }
	inline void SetAttackPoint(int nX, int nY) { m_ptAttackPoint.x = nX; m_ptAttackPoint.y = nY; }
	inline void AddAdjacent(int nCityID) { m_vAdjacentCities.push_back(nCityID); }
	inline void SetOwner( int nCityType) { 
		m_nCurrentOwner = nCityType; 
		if(m_nCurrentOwner == PLAYER_CITY) 
		{
			m_bIsAttackable = false;
			SetColor(D3DCOLOR_ARGB(255, 0, 255, 255));
		}
	}
	inline void SetOriginalOwner(int nCityType)
	{
		m_nOriginalOwner = nCityType;
		SetOwner(nCityType);
	}
	inline void Revolt() 
	{ 
		m_nCurrentOwner = m_nOriginalOwner; 
		SetDefaultColor(this->GetID());
	}
	inline void SetImageID( int nImageID) { m_nImageID = nImageID; }
	inline void SetClickRect( RECT rClickRect) { m_rClickRect = rClickRect; }
	inline void SetClickRect( int nLeft, int nTop, int nWidth, int nHeight)
	{
		RECT rect = {nLeft, nTop, nLeft+nWidth, nTop+nHeight};
		m_rClickRect = rect;
	}
};