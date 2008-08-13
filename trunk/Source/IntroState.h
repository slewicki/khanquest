//////////////////////////////////////////////////////////
//	File:	"CIntroState.h"
//
//	Author:	Dennis Wallace (DW)
//
//	Purpose: To contain functionality of the CIntroState state
//////////////////////////////////////////////////////////
#pragma once
#include "CBitmapfont.h"
#include "IGameState.h"

class CSGD_WaveManager;
class CSGD_TextureManager;
class CSGD_DirectInput;

class CIntroState : public IGameState
{
	CSGD_TextureManager*	m_pTM;
	CSGD_DirectInput*		m_pDI;
	CBitmapFont				m_BF;
	CSGD_WaveManager*		m_pWM;

	bool					m_bPaused;
	bool					m_bTitle;
	int						m_nVolume;
	int						m_nMaxVolume;
	float					m_fTimer;
	float					m_fEscTimer;
	int						m_nTitle;
	int m_nImageID;
	int m_nTitleID;
	POINT m_ptImageLoc, m_ptImageSize;
	char* m_szImageFile;

	int m_nSongID;
	bool m_bAlpha;
	int m_nAlpha;


	CIntroState(void);
	CIntroState(const CIntroState&);
	CIntroState& operator=(const CIntroState&);
	~CIntroState(void);
public:

	static CIntroState* GetInstance(void)
	{
		static CIntroState instance;						
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
	
	//////////////////////////////////////////////////////
	//	Function: “Parse”
	//	Last Modified: Aug 2, 2008
	//	 Purpose: To parce menu info
	//////////////////////////////////////////////////////
	bool Parse(char* szFilename);

	//////////////////////////////////////////////////////
	//	Function: “FadeOut”
	//	Last Modified: Aug 2, 2008
	//	 Purpose: fade out of screen
	//////////////////////////////////////////////////////
	void FadeOut(float fElapsedTime);
};