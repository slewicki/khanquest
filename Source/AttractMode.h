#pragma once
#include "IGameState.h"
#include "DirectShow.h"

class CAttractMode : public IGameState
{	
	CDirectShow*	m_pDS;
	bool			m_bPaused;

	CAttractMode(void);
	CAttractMode(const CAttractMode&);
	CAttractMode& operator=(const CAttractMode&);
	~CAttractMode(void);

public:
	//////////////////////////////////////////////////////
	// Function: �GetInstance�
	// Last Modified: Aug 2, 2008
	// Purpose: To return an instance of the class
	//////////////////////////////////////////////////////
	static CAttractMode* GetInstance(void)
	{
		static CAttractMode instance;						
		return &instance;
	}

	//////////////////////////////////////////////////////
	//	Function: �Enter�
	//	Last Modified: Aug 2, 2008
	//	Purpose: To load up all required information
	//////////////////////////////////////////////////////
	void Enter(void);

	//////////////////////////////////////////////////////
	//	Function: �Exit�
	//	Last Modified: Aug 2, 2008
	//	Purpose: To unload any needed information
	//////////////////////////////////////////////////////
	void Exit(void);
	
	//////////////////////////////////////////////////////
	//	Function: �Input�
	//	Last Modified: Aug 2, 2008
	//	Purpose: To get input from the user to interact
	//			with the state
	//////////////////////////////////////////////////////
	bool Input(float fElapsedTime);

	//////////////////////////////////////////////////////
	//	Function: �Update�
	//	Last Modified: Aug 2, 2008
	//	Purpose: To update our information after input
	//////////////////////////////////////////////////////
	void Update(float fElapsedTime);

	//////////////////////////////////////////////////////
	//	Function: �Render�
	//	Last Modified: Aug 2, 2008
	//	 Purpose: To render our information to the screen
	//////////////////////////////////////////////////////
	void Render(float fElapsedTime);

	///////////////////////////////////////////
	//  Function: Accessors
	//	Last Modified: Aug 2, 2008
	//  Purpose : Returns the specified type.
	///////////////////////////////////////////
	
};