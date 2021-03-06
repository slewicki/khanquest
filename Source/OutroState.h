#pragma once
#include "CBitmapfont.h"
#include "IGameState.h"

class CSGD_TextureManager;
class CSGD_DirectInput;

class COutroState : public IGameState
{
	CSGD_TextureManager*	m_pTM;
	CSGD_DirectInput*		m_pDI;
	CBitmapFont				m_BF;
	IGameState*				m_pToSwitchTo;
	bool					m_bPaused;
	float					m_fTimer;
	float					m_fEscTimer;
	float					m_fJoyTimer;
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
		RECT rToClick;
	};
	TextToHold* Buttons;
	int m_nNumButtons;
	bool m_bAlpha;
	int m_nAlpha;

	enum
	{
		YES = 1,
		NO,
	};

	COutroState(void);
	COutroState(const COutroState&);
	COutroState& operator=(const COutroState&);
	~COutroState(void);
public:
static COutroState* GetInstance(void)
	{
		static COutroState instance;						
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
	/////////////////////////////////////////////////////
	//	Function: �Parse�
	//	Last Modified: Aug 2, 2008
	//	 Purpose: Reads in the outro state using xml
	//////////////////////////////////////////////////////
	bool Parse(char* szFilename);
	/////////////////////////////////////////////////////
	//	Function: �FadeOut�
	//	Last Modified: Aug 2, 2008
	//	 Purpose: Fades the current state out
	//////////////////////////////////////////////////////
	void FadeOut(float fElapsedTime);
};