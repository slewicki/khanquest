#pragma once
#include "IGameState.h"
#include "CBitmapFont.h"
#include <string>
using namespace std;

class CSGD_TextureManager;
class CSGD_DirectInput;
class CSGD_WaveManager;

class CMainMenuState : public IGameState
{	
	CSGD_TextureManager*	m_pTM;
	CSGD_DirectInput*		m_pDI;
	CBitmapFont				m_BF;
	CSGD_WaveManager*		m_pWM;
	IGameState*				m_pToSwitchTo;
	bool					m_bPaused;
	int						m_nVolume;
	int						m_nMaxVolume;
	float					m_fTimer;
	float					m_fEscTimer;

	int m_nImageID;
	POINT m_ptImageLoc, m_ptImageSize;
	string m_szImageFile;

	int m_nCursorID;
	POINT m_ptCursorPosition;
	int m_nCurrentButton;
	float m_fCurScaleX, m_fCurScaleY;
	string m_szCursorName;

	struct TextToHold
	{
		POINT ptPosition;
		string Text;
		int alpha,red,green,blue,Action;			
		float fscalex,fscaley;
	};
	TextToHold* Buttons;
	int m_nNumButtons;
	int m_nSongID;


	bool m_bAlpha;
	int m_nAlpha;
	
	
	enum
	{
		WorldMapState = 1,
		Load,
		Options,
		ExitGame,
	};

	////////////////////////////////////////////
	//	Function:	"CHUDState(Constructor)"
	//	Last Modified: Aug 2, 2008
	//	Purpose:	Proper singleton
	////////////////////////////////////////////
	CMainMenuState(void);
	CMainMenuState(const CMainMenuState&);
	CMainMenuState& operator=(const CMainMenuState&);

	////////////////////////////////////////////
	//	Function:	"~CHUDState(Destructor)"
	//	Last Modified: Aug 2, 2008
	//	Purpose:	Proper singleton
	////////////////////////////////////////////
	~CMainMenuState(void);

public:
	//////////////////////////////////////////////////////
	// Function: “GetInstance”
	// Last Modified: Aug 2, 2008
	// Purpose: To return an instance of the class
	//////////////////////////////////////////////////////
	static CMainMenuState* GetInstance(void)
	{
		static CMainMenuState instance;						
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
	void SetPause(bool value){m_bPaused = value;}


	bool Parse(char* szFileName);

	void FadeIn(float fElpasedTime);
	void FadeOut(float fElpasedTime);

};
