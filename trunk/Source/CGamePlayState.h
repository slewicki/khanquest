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
#include "CSGD_Direct3D.h"
#include "CBitmapFont.h"
#include "CTileEngine.h"
#include "CEventSystem.h"
#include "CCamera.h"
#include "ObjectManager.h"
#include "CParticleEngine.h"

class CHUDState;
class CGame;
class CGamePlayState : public IGameState
{
private:

	//	Wrappers
	CSGD_Direct3D*							m_pD3D;
	CSGD_WaveManager*						m_pWM;
	CSGD_TextureManager*					m_pTM;
	CSGD_DirectInput*						m_pDI;
	CEventSystem*							m_pES;
	CGame*									m_pCG;
	CCamera*								m_pCamera;
	ObjectManager*							m_pOM;

	CParticleEngine*						m_pPE;
	int m_nTestEmitter;
	int m_nSkyCloudID;
	int m_nSkyCloudID2;
	bool									m_bIsPaused;
	bool									m_bButtonDown;
	CHUDState*								m_pHUD;

	POINT									m_ptBoxLocation;
	POINT									m_ptCurrentLocation;
	RECT									m_rSelectionBox;
	//Engines
	CTileEngine*							Map;
	//CTile*									Tile;

	// TEMP BUTTONS FOR DEMO - SHOW WORLD MAP AFTER WIN OR LOSS
	// REMOVE AFTER DEMO
	RECT									m_rVictoryButton;
	//------------------------------------------------------
	int										m_nButtonID;
	int										m_nHUD_ID;
	CBitmapFont								m_cFont;
	int										m_nLucidiaWhiteID;
	int										m_nTerrorLevel;
	int										m_nSelectionID;
	float									m_fJoyTimer;
	POINT									m_ptMousePos;
	// Create a vector of list of selected units (CUnit*) and make an accessor
	// for the HUD to use.  The HUD should disply up to 8 units, the max that can be selected.

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
	
	//////////////////////////////////////////////
	// Function:	"GetSelectionRect"
	// Last Modified: August 05, 2008
	// Purpose: Create a Selection box for Multiple Unit selection.
	//////////////////////////////////////////////
	RECT GetSelectionRect();
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
	int GetTerrorLevel() { return m_nTerrorLevel; }
	int GetSelectionID() { return m_nSelectionID; }

	///////////////////////////////////////////
	//  Function: Modifiers
	//	Last Modified: July 23, 2008
	//  Purpose : Modifies the specified type.
	///////////////////////////////////////////
	void SetTerrorLevel(int nTerrorLevel) { m_nTerrorLevel = nTerrorLevel; }
	void SetPaused(bool bPaused)		  { m_bIsPaused = bPaused; }

	//////////////////////////////////////////////////////
	//	Function: “IntToString”
	//	Last Modified: July 23, 2008
	//	Purpose: Helper function, turn int to string
	//////////////////////////////////////////////////////
	string IntToString(int nNum);
	
};