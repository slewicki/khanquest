#pragma once
#include "IGameState.h"
#include "CBitmapFont.h"

class CSGD_WaveManager;
class CSGD_TextureManager;
class CSGD_DirectInput;

class CLoseGameState: public IGameState
{
	CSGD_TextureManager*	m_pTM;
	CSGD_DirectInput*		m_pDI;
	CSGD_WaveManager*		m_pWM;
	CBitmapFont				m_BF;
	
	float m_fTimer;
	float m_fEscTimer;
	bool m_bAlpha;
	bool m_bEsc;
	int m_nSongID;
	int m_nImageID;
	POINT m_ptImageLoc, m_ptImageSize;
	string m_szImageFile;
	int m_nAlpha;

	CLoseGameState(void);
	CLoseGameState(const CLoseGameState&);
	CLoseGameState& operator=(const CLoseGameState&);
	
	////////////////////////////////////////////
	//	Function:	"~CHUDState(Destructor)"
	//	Last Modified: Aug 2, 2008
	//	Purpose:	Proper singleton
	////////////////////////////////////////////
	~CLoseGameState(void);
public:

	static CLoseGameState* GetInstance(void)
	{
		static CLoseGameState instance;						
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

	void StartEsc();
};
