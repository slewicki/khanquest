//////////////////////////////////////////////////////////
//	File:	"CPausedState.h"
//
//	Author:	Dennis Wallace (DW)
//
//	Purpose: To contain functionality of the CPausedState state
//////////////////////////////////////////////////////////
#ifndef _CPAUSEDSTATE_H_
#define _CPAUSEDSTATE_H_
#include "IGameState.h"
#include "CSGD_TextureManager.h"
#include "CSGD_DirectInput.h"
#include "CBitmapFont.h"
class CPausedState : public IGameState
{

private:

	int										m_nButtonID;
	int										m_nLucidiaWhiteID;

	RECT									m_rResumeButton;
	RECT									m_rRetreatButton;
	RECT									m_rQuitButton;


	CBitmapFont								m_cFont;
	CSGD_TextureManager*					m_pTM;
	CSGD_DirectInput*						m_pDI;
	////////////////////////////////////////////
	//	Function:	"CPausedState(Constructor)"
	//	Last Modified: August 05, 2008
	//	Purpose:	Proper singleton
	////////////////////////////////////////////
	CPausedState(void);
	CPausedState(const CPausedState&);
	CPausedState& operator=(const CPausedState&);
	////////////////////////////////////////////
	//	Function:	"~CPausedState(Destructor)"
	//	Last Modified: August 05, 2008
	//	Purpose:	Proper singleton
	////////////////////////////////////////////
	~CPausedState(void);

public:

	
	//////////////////////////////////////////////////////
	//	Function: “GetInstance”
	//	Last Modified: August 05, 2008
	//	Purpose: To return an instance of the class
	//////////////////////////////////////////////////////
	static CPausedState* GetInstance(void)
	{
		static CPausedState instance;						
		return &instance;
	}
	//////////////////////////////////////////////////////
	//	Function: “Enter”
	//	Last Modified: August 05, 2008
	//	Purpose: To load up all required information
	//////////////////////////////////////////////////////
	void Enter(void);

	//////////////////////////////////////////////////////
	//	Function: “Exit”
	//	Last Modified: August 05, 2008
	//	Purpose: To unload any needed information
	//////////////////////////////////////////////////////
	void Exit(void);
	
	//////////////////////////////////////////////////////
	//	Function: “Input”
	//	Last Modified: August 05, 2008
	//	Purpose: To get input from the user to interact
	//			with the state
	//////////////////////////////////////////////////////
	bool Input(float fElapsedTime);

	//////////////////////////////////////////////////////
	//	Function: “Update”
	//	Last Modified: August 05, 2008
	//	Purpose: To update our information after input
	//////////////////////////////////////////////////////
	void Update(float fElapsedTime);

	//////////////////////////////////////////////////////
	//	Function: “Render”
	//	Last Modified: August 05, 2008
	//	Purpose: To render our information to the screen
	//////////////////////////////////////////////////////
	void Render(float fElapsedTime);


};
#endif