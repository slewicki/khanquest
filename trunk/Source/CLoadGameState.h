//////////////////////////////////////////////////////////
//	File:	"CLoadGameState.h"
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
#include "CSGD_DirectInput.h"
#include "CGame.h"
#include "CBitmapFont.h"
#include "CParticleEngine.h"

class CGame;

class CLoadGameState : public IGameState
{
private:

	//	Wrappers
	CGame*									m_pCG;
	CSGD_WaveManager*						m_pWM;
	CSGD_TextureManager*					m_pTM;
	CSGD_DirectInput*						m_pDI;

	//Particles
	CParticleEngine*						m_pPE;
	int										m_nTorchID;
	int										m_nSmokeID1;
	int										m_nSmokeID2;
	int										m_nTorchID2;
	int										m_nTorchPicID;
	//fire sound
	int										m_nTorchSound;

	float									m_fTimer;
	int										m_nButtonID;
	int										m_nScrollButtonID;
	int										m_nClickID;
	int										m_nTickID;
	float									m_fJoyTimer;
	POINT									m_ptMousePos;
	
	CBitmapFont								m_cFont;
	
	int										m_nLucidiaWhiteID;
	bool									m_bTutorial;
	RECT									m_rTutorial;
	RECT									m_rClickRect[3];
	RECT									m_rAccept;
	RECT									m_rBack;
	
	int										m_nBackgroundID;
	bool									m_bIsNewGame;
	bool									m_bIsEmpty[3];

	int										m_nChosenSlot;



	////////////////////////////////////////////
	//	Function:	"CLoadGameState(Constructor)"
	//	Last Modified: July 23, 2008
	//	Purpose:	Proper singleton
	////////////////////////////////////////////
	CLoadGameState(void);
	CLoadGameState(const CLoadGameState&);
	CLoadGameState& operator=(const CLoadGameState&);

	////////////////////////////////////////////
	//	Function:	"~CLoadGameState(Destructor)"
	//	Last Modified: July 23, 2008
	//	Purpose:	Proper singleton
	////////////////////////////////////////////
	~CLoadGameState(void);

public:
	//////////////////////////////////////////////////////
	//	Function: “GetInstance”
	//	Last Modified: July 23, 2008
	//	Purpose: To return an instance of the class
	//////////////////////////////////////////////////////
	static CLoadGameState* GetInstance(void)
	{
		static CLoadGameState instance;						
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
	void IsNewGame(bool bChoseNewGame) { m_bIsNewGame = bChoseNewGame; }
};