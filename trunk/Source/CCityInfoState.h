//////////////////////////////////////////////////////////
//	File:	"CCityInfoState.h"
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

class CCity;

class CCityInfoState : public IGameState
{
private:

	//	Wrappers
	CSGD_WaveManager*						m_pWM;
	CSGD_TextureManager*					m_pTM;
	CSGD_DirectInput*						m_pDI;
	
	// Unit sprite IDs (Unit animations later)
	CCity*									m_pSelectedCity;
	int										m_nDisplayID;
	float									m_fPositionX;
	bool									m_bRetract;
	////////////////////////////////////////////
	//	Function:	"CCityInfoState(Constructor)"
	//	Last Modified: July 23, 2008
	//	Purpose:	Proper singleton
	////////////////////////////////////////////
	CCityInfoState(void);
	CCityInfoState(const CCityInfoState&);
	CCityInfoState& operator=(const CCityInfoState&);

	////////////////////////////////////////////
	//	Function:	"~CCityInfoState(Destructor)"
	//	Last Modified: July 23, 2008
	//	Purpose:	Proper singleton
	////////////////////////////////////////////
	~CCityInfoState(void);

public:
	//////////////////////////////////////////////////////
	// Function: “GetInstance”
	// Last Modified: July 23, 2008
	// Purpose: To return an instance of the class
	//////////////////////////////////////////////////////
	static CCityInfoState* GetInstance(void)
	{
		static CCityInfoState instance;						
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
	//	 Purpose: To render our information to the screen
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