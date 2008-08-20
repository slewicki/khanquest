#pragma once
#include "IGameState.h"
#include "CBitmapFont.h"
#include <string>
using std::string;

enum {CAMERA_UP = 0, CAMERA_DOWN, CAMERA_LEFT,CAMERA_RIGHT};
class CSGD_WaveManager;
class CSGD_DirectInput;
class CSGD_TextureManager;

class CKeyBindState : public IGameState
{
	CSGD_TextureManager*	m_pTM;
	CSGD_DirectInput*		m_pDI;
	CBitmapFont				m_cFont;
	CSGD_WaveManager*		m_pWM;

#pragma region Timers&Volumes
	float					m_fTimer;
	float					m_fEscTimer;
	float					m_JoyTimer;
#pragma endregion

#pragma region Background
	int m_nImageID;
	POINT m_ptImageLoc, m_ptImageSize;
	string m_szImageFile;
#pragma endregion

#pragma region Cursor
	int m_nCursorID;
	POINT m_ptCursorPosition;
	int m_nCurrentButton;
	float m_fCurScaleX, m_fCurScaleY;
	string m_szCursorName;
#pragma endregion

	struct TextToHold
	{
		POINT ptPosition;
		string Text;
		int alpha,red,green,blue,Action;			
		float fscalex,fscaley;
	};
	TextToHold* Buttons;
	int m_nNumButtons;
	int	m_nSwitchSoundID;
	bool m_bAlpha;
	int m_nAlpha;
	int m_nFontID;
	bool			m_bIsBinding;
	bool			m_bLeaving;

	DWORD			m_dwBoundKeys[4];	// KEYS IN DIK CODE
	string			m_szBoundKeys[4];	// string of Bound keys

	enum
	{
		Up = 0,
		Down,
		Left,
		Right,
		KeyBind,
		Back,
	};
	////////////////////////////////////////////
	//	Function:	"CControlState(Constructor)"
	////////////////////////////////////////////
	CKeyBindState(void);
	CKeyBindState(const CKeyBindState&);
	CKeyBindState& operator=(const CKeyBindState&);

	////////////////////////////////////////////
	//	Function:	"~CControlState(Destructor)"
	////////////////////////////////////////////
	~CKeyBindState(void);
	
public:
	//////////////////////////////////////////////////////
	// Function: “GetInstance”
	// Last Modified: August 11, 2008
	// Purpose: To return an instance of the class
	//////////////////////////////////////////////////////
	static CKeyBindState* GetInstance(void)
	{
		static CKeyBindState instance;
		return &instance;
	}
	
	//////////////////////////////////////////////////////
	// Function: “Enter”
	// Last Modified: August 11, 2008
	// Purpose: To load up all required information
	//////////////////////////////////////////////////////
	void Enter(void);

	//////////////////////////////////////////////////////
	// Function: “Exit”
	// Last Modified: August 11, 2008
	// Purpose: To unload any needed information
	//////////////////////////////////////////////////////
	void Exit(void);
	
	//////////////////////////////////////////////////////
	// Function: “Input”
	// Last Modified: August 11, 2008
	// Purpose: To get input from the user to interact
	//			with the state
	// Return:	False if something goes wrong or we
	//			want to exit the game
	//////////////////////////////////////////////////////
	bool Input(float fElapsedTime);

	//////////////////////////////////////////////////////
	// Function: “Update”
	// Last Modified: August 11, 2008
	// Purpose: To update our information after input
	//////////////////////////////////////////////////////
	void Update(float fElapsedTime);

	//////////////////////////////////////////////////////
	// Function: “Render”
	// Last Modified: August 11, 2008
	// Purpose: To render our information to the screen
	//////////////////////////////////////////////////////
	void Render(float fElapsedTime);

	///////////////////////////////////////////
	//  Function: GetBoundKey
	//  Last Modified: August 11, 2008
	//  Purpose : Returns the player's bound key (DIK_*) 
	//  for the passed in action. See CControlState 
	//	enum for possible actions.
	///////////////////////////////////////////
	DWORD GetBoundKey( int nID ) {return m_dwBoundKeys[nID]; }

	///////////////////////////////////////////
	//  Function: "BindKey"
	//  Last Modified: August 11, 2008
	//  Purpose : Binds a given key to the players control
	///////////////////////////////////////////
	bool BindKey(DWORD dwDIK, int nID);

	void FadeIn(float fElapsedTime);
	void FadeOut(float fElapsedTime);

	bool Parse(char* szFileName);
};