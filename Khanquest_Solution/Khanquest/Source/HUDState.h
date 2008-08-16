//////////////////////////////////////////////////////////
//	File:	"CHUDState.h"
//
//	Author:	Dennis Wallace (DW)
//
//	Purpose: To contain functionality of the CHUDState state
//////////////////////////////////////////////////////////
#pragma once
#include "IGameState.h"
#include "CSGD_TextureManager.h"
#include "CBitmapFont.h"

class CUnit;

class CHUDState : public IGameState
{
	CSGD_TextureManager* m_pTM;
	CBitmapFont			 m_BF;
	int					 m_nHUDID;
	int					 m_nIconID;
	
	vector<CUnit*>		 m_vUnits;

	int m_nInfantry;
	int m_nCavalry;
	int m_nCavalryArcher;
	int m_nAxmen;
	int m_nArcher;
	int m_nWarElephant;

	enum 	// Type of object
	{
		UNIT_INFANTRY = 0,
		UNIT_CAVALRY,
		UNIT_CAVALRY_ARCHER,
		UNIT_AXMEN,
		UNIT_ARCHER,
		UNIT_WAR_ELEPHANT,
	};


	////////////////////////////////////////////
	//	Function:	"CHUDState(Constructor)"
	//	Last Modified: Aug 2, 2008
	//	Purpose:	Proper singleton
	////////////////////////////////////////////
	CHUDState(void);
	CHUDState(const CHUDState&);
	CHUDState& operator=(const CHUDState&);

	////////////////////////////////////////////
	//	Function:	"~CHUDState(Destructor)"
	//	Last Modified: Aug 2, 2008
	//	Purpose:	Proper singleton
	////////////////////////////////////////////
	~CHUDState(void);

public:
	//////////////////////////////////////////////////////
	// Function: “GetInstance”
	// Last Modified: Aug 2, 2008
	// Purpose: To return an instance of the class
	//////////////////////////////////////////////////////
	static CHUDState* GetInstance(void)
	{
		static CHUDState instance;						
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

	///////////////////////////////////////////
	//  Function: Accessors
	//	Last Modified: Aug 2, 2008
	//  Purpose : Returns the specified type.
	///////////////////////////////////////////
	
	///////////////////////////////////////////
	//  Function: Modifiers
	//	Last Modified: Aug 2, 2008
	//  Purpose : Modifies the specified type.
	///////////////////////////////////////////
	void UpdateSelected();
};
