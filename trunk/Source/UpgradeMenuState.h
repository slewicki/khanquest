#pragma once
#include "CBitmapFont.h"
#include "IGameState.h"
class  CSGD_TextureManager;
class  CSGD_DirectInput;
class  CSGD_WaveManager;	
class  CCity;
#include "CUnit.h"

#define SPEED 400
#define ATTACK 500
#define ATTACKSPEED 500
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
	int										m_nCheckBoxID;
	int										m_nCheckMarkID;
	int										m_nClick;
	bool									m_bModified[6];
	bool									m_bAccept;
	CUnit									m_pUnit[6];
	CBitmapFont								m_cFont;
	int										m_nLucidiaWhiteID;
	int										m_nUnitCosts[6];
	int										m_nBackgroundID;
	RECT									m_rCheckBox[6][3];
	bool									m_bUpgraded[6][3];
	RECT									m_rAttackButton;
	RECT									m_rBackButton;
	float									m_fJoyTimer;
	POINT									m_ptMousePos;
	int										m_nGoldTotal;

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
