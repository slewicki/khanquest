#pragma once
#include "IGameState.h"
#include "CBitmapFont.h"

class CSGD_TextureManager;
class CSGD_DirectInput;
class CSGD_WaveManager;

class COptionsMenuState : public IGameState
{
	CSGD_TextureManager*	m_pTM;
	CSGD_DirectInput*		m_pDI;
	CBitmapFont				m_BF;
	CSGD_WaveManager*		m_pWM;	
	
	float					m_JoyTimer;

	int m_nImageID;
	POINT m_ptImageLoc, m_ptImageSize;
	string m_szImageFile;
	
	bool					m_bPaused;
	float m_fPlayTimer;

	int m_nCursorID;
	POINT m_ptCursorPosition;
	int m_nCurrentButton;
	float m_fCurScaleX, m_fCurScaleY;
	string m_szCursorName;

	int						m_nClick;
	int						m_nCheckBoxID;
	int						m_nCheckMarkID;

	float					m_fTimer;
	float					m_fEscTimer;
	
	bool					m_bAlpha;
	int						m_nAlpha;
	IGameState*				m_pToSwitchTo;

	struct TextToHold
	{
		POINT ptPosition;
		string Text;
		int alpha,red,green,blue,Action;			
		float fscalex,fscaley;
		RECT rToClick;
	};
	TextToHold* Buttons;
	int m_nNumButtons;
	
	
	enum
	{
		KEYBIND = 0,
		MUSIC,
		SFX,
		CREDITS,
		BACK,
		FULLSCREEN,
		FPS,
		STATEBOXES,
	};

	COptionsMenuState(void);
	COptionsMenuState(const COptionsMenuState&);
	COptionsMenuState& operator=(const COptionsMenuState&);

	////////////////////////////////////////////
	//	Function:	"~CHUDState(Destructor)"
	//	Last Modified: Aug 2, 2008
	//	Purpose:	Proper singleton
	////////////////////////////////////////////
	~COptionsMenuState(void);

	void SaveOptions();
public:
	//////////////////////////////////////////////////////
	// Function: �GetInstance�
	// Last Modified: Aug 2, 2008
	// Purpose: To return an instance of the class
	//////////////////////////////////////////////////////
	static COptionsMenuState* GetInstance(void)
	{
		static COptionsMenuState instance;						
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
	void SetPause(bool value){m_bPaused = value;}
	///////////////////////////////////////////
	//  Function: Modifiers
	//	Last Modified: Aug 2, 2008
	//  Purpose : Modifies the specified type.
	///////////////////////////////////////////
	bool Parse(char* szFileName);
	/////////////////////////////////////////////////////
	//	Function: �FadeIn�
	//	Last Modified: Aug 2, 2008
	//	 Purpose: Fades the current state in
	//////////////////////////////////////////////////////
	void FadeIn(float fElapsedTime);
	/////////////////////////////////////////////////////
	//	Function: �FadeIn�
	//	Last Modified: Aug 2, 2008
	//	 Purpose: Fades the current state out
	//////////////////////////////////////////////////////
	void FadeOut(float fElapsedTime);
};
