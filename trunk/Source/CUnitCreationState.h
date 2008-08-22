//////////////////////////////////////////////////////////
//	File:	"CUnitCreationState.h"
//
//	Author:	Sean Hamstra (SH)
//
//	Purpose: To contain functionality of the unit creation state
//////////////////////////////////////////////////////////

#pragma once
#include <windows.h>
#include "IGameState.h"
#include "CUnit.h"
#include "CSGD_TextureManager.h"
#include "CSGD_WaveManager.h"
#include "CSGD_DirectInput.h"
#include "CCity.h"
#include "CGame.h"
#include "CBitmapFont.h"
#define	MAX_UNITS 40

class CUnitCreationState : public IGameState
{
private:

	//	Wrappers
	CSGD_WaveManager*						m_pWM;
	CSGD_TextureManager*					m_pTM;
	CSGD_DirectInput*						m_pDI;
	
	int										m_nDisplayID;
	int										m_nUnitID[6];
	float									m_fTimer;
	int										m_nScrollButtonID;
	int										m_nClick;
	float									m_fJoyTimer;
	POINT									m_ptMousePos;
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
	RECT									m_rUpgradeButton;
	RECT									m_rAttackButton;
	RECT									m_rBackButton;

	int										m_nNumUnits[6];	// Number of units purchased
	int										m_nTotalUnits;	// Total units

	int										m_nFoodTotal;

	bool									m_bPaused;
	bool									m_bTutorial;
	RECT									m_rTutorial;

	////////////////////////////////////////////
	//	Function:	"CUnitCreationState(Constructor)"
	//	Last Modified: July 23, 2008
	//	Purpose:	Proper singleton
	////////////////////////////////////////////
	CUnitCreationState(void);
	CUnitCreationState(const CUnitCreationState&);
	CUnitCreationState& operator=(const CUnitCreationState&);

	////////////////////////////////////////////
	//	Function:	"~CUnitCreationState(Destructor)"
	//	Last Modified: July 23, 2008
	//	Purpose:	Proper singleton
	////////////////////////////////////////////
	~CUnitCreationState(void);

public:
	//////////////////////////////////////////////////////
	//	Function: “GetInstance”
	//	Last Modified: July 23, 2008
	//	Purpose: To return an instance of the class
	//////////////////////////////////////////////////////
	static CUnitCreationState* GetInstance(void)
	{
		static CUnitCreationState instance;						
		return &instance;
	}

	//////////////////////////////////////////////////////
	//	Function: “Enter”
	//	Last Modified: July 23, 2008
	//	Purpose: To load up all required information
	//////////////////////////////////////////////////////
	void Enter(void);

	//////////////////////////////////////////////////////
	//	Function: “Exit”
	//	Last Modified: July 23, 2008
	//	Purpose: To unload any needed information
	//////////////////////////////////////////////////////
	void Exit(void);
	
	//////////////////////////////////////////////////////
	//	Function: “Input”
	//	Last Modified: July 23, 2008
	//	Purpose: To get input from the user to interact
	//			with the state
	//////////////////////////////////////////////////////
	bool Input(float fElapsedTime);

	//////////////////////////////////////////////////////
	//	Function: “Update”
	//	Last Modified: July 23, 2008
	//	Purpose: To update our information after input
	//////////////////////////////////////////////////////
	void Update(float fElapsedTime);

	//////////////////////////////////////////////////////
	//	Function: “Render”
	//	Last Modified: July 23, 2008
	//	Purpose: To render our information to the screen
	//////////////////////////////////////////////////////
	void Render(float fElapsedTime);

	//////////////////////////////////////////////////////
	//	Function: “IntToString”
	//	Last Modified: July 23, 2008
	//	Purpose: Helper function, turn int to string
	//////////////////////////////////////////////////////
	string IntToString(int nNum);

	//////////////////////////////////////////////////////
	//	Function: “IntToString”
	//	Last Modified: July 23, 2008
	//	Purpose: Helper function, turn float to string
	//////////////////////////////////////////////////////
	string FloatToString(float fNum);

	///////////////////////////////////////////
	//  Function: Accessors
	//	Last Modified: July 23, 2008
	//  Purpose : Returns the specified type.
	///////////////////////////////////////////
	bool GetPaused(){return m_bPaused;}

	///////////////////////////////////////////
	//  Function: Modifiers
	//	Last Modified: July 23, 2008
	//  Purpose : Modifies the specified type.
	///////////////////////////////////////////
	void SetPause(bool bPause){m_bPaused = bPause;}

};