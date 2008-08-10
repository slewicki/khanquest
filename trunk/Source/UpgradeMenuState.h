#pragma once
#include "CBitmapFont.h"
#include "IGameState.h"
class  CSGD_TextureManager;
class  CSGD_DirectInput;
class  CSGD_WaveManager;	
class  CCity;
class  CUnit;

class CUpgradeMenuState : public IGameState
{
	//	Wrappers
	CSGD_WaveManager*						m_pWM;
	CSGD_TextureManager*					m_pTM;
	CSGD_DirectInput*						m_pDI;
	
	int										m_nDisplayID;
	int										m_nUnitID[6];
	float									m_fTimer;
	int										m_nScrollButtonID;
	

	CCity*									m_pSelectedCity;
	CBitmapFont								m_cFont;
	CUnit*									m_pPlayerUnitInfo;
	int										m_nLucidiaWhiteID;
	int										m_nUnitCosts[6];
	int										m_nPlusButtonID;
	int										m_nMinusButtonID;
	int										m_nBackgroundID;
	RECT									m_rPlusButtons[6];
	RECT									m_rMinusButtons[6];
	RECT									m_rAttackButton;
	RECT									m_rBackButton;

	int										m_nNumUnits[6];	// Number of units purchased
	int										m_nTotalUnits;	// Total units

	int										m_nFoodTotal;
	CUpgradeMenuState(void);
	~CUpgradeMenuState(void);
	CUpgradeMenuState(const CUpgradeMenuState&);
	CUpgradeMenuState& operator=(const CUpgradeMenuState&);

public:
	
	static CUpgradeMenuState* GetInstance(void)
	{
		static CUpgradeMenuState instance;						
		return &instance;
	}

	//////////////////////////////////////////////////////
	//	Function: “Enter”
	//	Last Modified: Aug 2, 2008
	//	Purpose: To load up all required information
	//////////////////////////////////////////////////////
	void Enter(void);

	//////////////////////////////////////////////////////
	//	Function: “Exit”
	//	Last Modified: Aug 2, 2008
	//	Purpose: To unload any needed information
	//////////////////////////////////////////////////////
	void Exit(void);
	
	//////////////////////////////////////////////////////
	//	Function: “Input”
	//	Last Modified: Aug 2, 2008
	//	Purpose: To get input from the user to interact
	//			with the state
	//////////////////////////////////////////////////////
	bool Input(float fElapsedTime);

	//////////////////////////////////////////////////////
	//	Function: “Update”
	//	Last Modified: Aug 2, 2008
	//	Purpose: To update our information after input
	//////////////////////////////////////////////////////
	void Update(float fElapsedTime);

	//////////////////////////////////////////////////////
	//	Function: “Render”
	//	Last Modified: Aug 2, 2008
	//	 Purpose: To render our information to the screen
	//////////////////////////////////////////////////////
	void Render(float fElapsedTime);
	void StartEsc();
	string IntToString(int value);
	string FloatToString(float value);

};
