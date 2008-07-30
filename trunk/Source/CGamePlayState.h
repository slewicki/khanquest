//////////////////////////////////////////////////////////
//	File:	"CGamePlayState.h"
//
//	Author:	Sean Hamstra (SH)
//
//	Purpose: To contain functionality of the gameplay state
//////////////////////////////////////////////////////////

#pragma once
#include <windows.h>
#include "IGameState.h"
#include "CUnit.h"
#include "CSGD_TextureManager.h"
#include "CSGD_WaveManager.h"
#include "CSGD_DirectInput.h"
#include "CBitmapFont.h"
#include "CTileEngine.h"


class CGamePlayState : public IGameState
{
private:

	//	Wrappers
	CSGD_WaveManager*						m_pWM;
	CSGD_TextureManager*					m_pTM;
	CSGD_DirectInput*						m_pDI;
	
	bool									m_bIsPaused;

	//Engines
	CTileEngine Map;

	// TEMP BUTTONS FOR DEMO - SHOW WORLD MAP AFTER WIN OR LOSS
	// REMOVE AFTER DEMO
	RECT									m_rVictoryButton;
	RECT									m_rRetreatButton;
	int										m_nButtonID;
	CBitmapFont								m_cFont;
	int										m_nLucidiaWhiteID;


	////////////////////////////////////////////
	//	Function:	"CGamePlayState(Constructor)"
	//	Last Modified: July 18, 2008
	//	Purpose:	Proper singleton
	////////////////////////////////////////////
	CGamePlayState(void);
	CGamePlayState(const CGamePlayState&);
	CGamePlayState& operator=(const CGamePlayState&);

	////////////////////////////////////////////
	//	Function:	"~CGamePlayState(Destructor)"
	//	Last Modified: July 18, 2008
	//	Purpose:	Proper singleton
	////////////////////////////////////////////
	~CGamePlayState(void);

public:
	//////////////////////////////////////////////////////
	//	Function: “GetInstance”
	//	Last Modified: July 18, 2008
	//	Purpose: To return an instance of the class
	//////////////////////////////////////////////////////
	static CGamePlayState* GetInstance(void)
	{
		static CGamePlayState instance;						
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

	///////////////////////////////////////////
	//  Function: Accessors
	//	Last Modified: July 23, 2008
	//  Purpose : Returns the specified type.
	///////////////////////////////////////////
	

	///////////////////////////////////////////
	//  Function: Modifiers
	//	Last Modified: July 23, 2008
	//  Purpose : Modifies the specified type.
	///////////////////////////////////////////
	

};