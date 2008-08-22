//////////////////////////////////////////////////////////
//	File:	"CWorldMapState.h"
//
//	Author:	Sean Hamstra (SH)
//
//	Purpose: To contain functionality of the world map state
//////////////////////////////////////////////////////////
#pragma once
#include <windows.h>
#include "IGameState.h"
#include "CSGD_TextureManager.h"
#include "CSGD_WaveManager.h"
#include "CSGD_DirectInput.h"
#include "CCity.h"
#include "CBitmapFont.h"

#define NUM_CHEATS 3

class CWorldMapState : public IGameState
{
private:

	//	Wrappers
	CSGD_WaveManager*				m_pWM;
	CSGD_TextureManager*			m_pTM;
	CSGD_DirectInput*				m_pDI;

	CBitmapFont						m_cFont;

	CCity**							m_pCities;
	CCity*							m_pSelectedCity;

	vector<CCity*>					m_pPlayerOwnedCities;
	POINT							m_ptMousePos;
	DWORD							m_dwSelectedColor;

	int								m_nWorldMapID;
	int								m_nAttackSymbolID;
	int								m_nTitleID;
	int								m_nLucidiaWhiteID;
	int								m_nClick;
	float							m_fJoyTimer;
	bool							m_bPaused;
	int								m_nScrollButtonID;
	bool							m_bTutorial;
	RECT							m_rTutorial;

	// cheat codes
	list<char> m_clistInputQueue;
	list<char> m_clistCheatCode[NUM_CHEATS];

	////////////////////////////////////////////
	//	Function:	"CWorldMapState(Constructor)"
	//	Last Modified: July 18, 2008
	//	Purpose:	Proper singleton
	////////////////////////////////////////////
	CWorldMapState(void);
	CWorldMapState(const CWorldMapState&);
	CWorldMapState& operator=(const CWorldMapState&);

	////////////////////////////////////////////
	//	Function:	"~CWorldMapState(Destructor)"
	//	Last Modified: July 18, 2008
	//	Purpose:	Proper singleton
	////////////////////////////////////////////
	~CWorldMapState(void);


public:
	//////////////////////////////////////////////////////
	// Function: “GetInstance”
	//	Last Modified: July 18, 2008
	// Purpose: To return an instance of the class
	//////////////////////////////////////////////////////
	static CWorldMapState* GetInstance(void)
	{
		static CWorldMapState instance;
								
		return &instance;
	}
	
	//////////////////////////////////////////////////////
	//	Function: “Enter”
	//	Last Modified: July 18, 2008
	//	Purpose: To load up all required information
	//////////////////////////////////////////////////////
	void Enter(void);

	//////////////////////////////////////////////////////
	//	Function: “Exit”
	//	Last Modified: July 18, 2008
	//	Purpose: To unload any needed information
	//////////////////////////////////////////////////////
	void Exit(void);
	
	//////////////////////////////////////////////////////
	//	Function: “Input”
	//	Last Modified: July 18, 2008
	//	Purpose: To get input from the user to interact
	//			with the state
	//	Return:	False if something goes wrong or we
	//			want to exit the game
	//////////////////////////////////////////////////////
	bool Input(float fElapsedTime);

	//////////////////////////////////////////////////////
	//	Function: “Update”
	//	Last Modified: July 18, 2008
	//	Purpose: To update our information after input
	//////////////////////////////////////////////////////
	void Update(float fElapsedTime);

	//////////////////////////////////////////////////////
	//	Function: “Render”
	//	Last Modified: July 18, 2008
	//	Purpose: To render our information to the screen
	//////////////////////////////////////////////////////
	void Render(float fElapsedTime);

	///////////////////////////////////////////
	//  Function: Accessors
	//	Last Modified: July 23, 2008
	//  Purpose : Returns the specified type.
	///////////////////////////////////////////
	inline CCity* GetSelectedCity() { return m_pSelectedCity; }

	///////////////////////////////////////////
	//  Function: Modifiers
	//	Last Modified: July 23, 2008
	//  Purpose : Modifies the specified type.
	///////////////////////////////////////////

	
	//////////////////////////////////////////////////////
	//	Function: “IntToString”
	//	Last Modified: July 23, 2008
	//	Purpose: Helper function, turn float to string
	//////////////////////////////////////////////////////
	string IntToString(int nNum);
	
	//////////////////////////////////////////////////////
	//	Function: “CheckCheats”
	//	Last Modified: August 06, 2008
	//	Purpose: Helper function, returns the int of the cheat that was entered
	//////////////////////////////////////////////////////

};