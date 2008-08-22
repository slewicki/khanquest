#pragma once
#include "IGameState.h"
#include <string>
#include "CBitmapFont.h"
class CSGD_WaveManager;
class CSGD_TextureManager;
class CSGD_DirectInput;

class CCreditState : public IGameState
{
	
	string					m_szTitles[10];
	string					m_szCoders[10];
	string					m_szResources[10];
	string					m_szInstructors[10];

	CBitmapFont					m_cFont;
	CSGD_TextureManager*		m_pTM;
	CSGD_DirectInput*			m_pDI;
	CSGD_WaveManager*			m_pWM;

	int							m_nNumInstructors;
	int							m_nNumCoders;
	int							m_nNumResources;
	int							m_nNumTitle;

	float						m_fTimer;
	int							m_nYpos;
	CCreditState(void);
	CCreditState(const CCreditState&);
	CCreditState& operator=(const CCreditState&);
	~CCreditState(void);

public:

	//////////////////////////////////////////////////////
	//	Function: “GetInstance”
	//	Last Modified: August 05, 2008
	//	Purpose: To return an instance of the class
	//////////////////////////////////////////////////////
	static CCreditState* GetInstance(void)
	{
		static CCreditState instance;						
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
	
	//////////////////////////////////////////////////////
	//	Function: “Render”
	//	Last Modified: August 05, 2008
	//	Purpose: Reads in the xml file containing the names of the credits
	//////////////////////////////////////////////////////
	bool Parse(char* szFileName);
};
