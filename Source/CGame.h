//////////////////////////////////////////////////////////
//	File:	"CGame.h"
//
//	Author:	Sean Hamstra (SH)
//
//	Purpose: To contain all game functionality.
//////////////////////////////////////////////////////////

#pragma once 

//	SGD wrapper includes
#include "CSGD_Direct3D.h"
#include "CSGD_TextureManager.h"
#include "CSGD_DirectSound.h"
#include "CSGD_WaveManager.h"
#include "CSGD_DirectInput.h"
#include "AnimationManager.h"
#include "MainMenuState.h"

#include "CTileEngine.h"
#include "IGameState.h"
#include <windows.h>
#include "CUnit.h"
#include "CCity.h"
#include "CGamePlayState.h"
#include "resource.h"	//  For icon/cursor ID
#include "CBitmapFont.h"
//Pixel Shader Testing
#include "CPixelShader.h"
#include "CProfile.h"

using std::string;
#define TOTAL_CITIES 10

//Enum for playlist added this order;
enum{INTRO,LOSEBATTLE,LOSEGAME,WONBATTLE,WONGAME,CITYSELECT,CREDITS,BATTLESTATE};
// Saving slots
enum{SLOT1, SLOT2, SLOT3};

class CGame	
{
private:
	//	Pointers to SGD Wrappers:
	CSGD_Direct3D*			m_pD3D;
	CSGD_TextureManager*	m_pTM;
	CSGD_DirectSound*		m_pDS;
	CSGD_WaveManager*		m_pWM;
	CSGD_DirectInput*		m_pDI;
	CAnimationManager*		m_pAM;
	CBitmapFont				m_BF;
	//CTileEngine*			m_pMap;
	// Time and Frame Variables
	DWORD					m_dwPreviousTime;
	int						m_nFrameCounter;	//counts number of frames per second
	int						m_nFPS;				// prints number
	DWORD					m_dwFrameTimer;
	int						m_nFontID;
	bool					m_bIsWindowed;		// Is the game windowed?
	bool					m_bFPS;				// FPS on
	float					m_fGameTime;		// Time since the game began
	int						m_nSFXVolume;		// Sound Effects Volume
	int						m_nMusicVolume;		// Music Volume
	bool					m_bUpGrades[6][3];
	vector<IGameState*>		m_vStates;			// Stack based states
	int						m_nAttackSoundID[6];
	int						m_nDeathSoundID[6];
	int*					m_nPlayList;
	int						m_nNumSongs;
	int						m_nCurrentSong;
	int						m_nPreviousSong;
	CUnit					m_pPlayerUnitInfo[6];	// Info for player's units
	CUnit					m_pCPUUnitInfo[6];		// Info for CPU's units
	bool					m_bShowBoxes;
	char					m_szPlayerName[6];		// Name of player's load slot (3 letters)
	bool					m_bTutorial;
	int						m_nCurrentSaveSlot;
	
	int						m_nTerrorLevel;			// Terror Level
	
	int						m_nGold;				// The player's current gold amount
	int						m_nFood;				// The player's current food amount.
	int						m_nWins;				// The player's current wins.
	int						m_nLoses;				// The player's current loses.
	HWND					m_hWnd;
	HINSTANCE				m_hInstance;

	POINT					m_ptWindOffset;
	POINT					m_ptMousePos;

	CCity*					m_pCities[TOTAL_CITIES];
	CCity*					m_pSelectedCity;

	char					m_chXiaCount;
	char					m_chKCount;
	char					m_chJinCount;

	vector<int>				m_vConqueredCities;

	//Pixel Shader Test variables
	CPixelShader			m_PixelShader;
	LPDIRECT3DTEXTURE9		m_pRenderTarget;
	LPDIRECT3DTEXTURE9		m_pVisibleTarget;
	LPDIRECT3DSURFACE9		m_pBackBuffer;
	int						m_nScreenWidth;
	int						m_nScreenHeight;
	D3DXMATRIX				m_ViewMatrix;
	D3DXMATRIX				m_WorldMatrix;
	D3DXMATRIX				m_ProjectionMatrix;

	
	//	Proper singleton:

	////////////////////////////////////////////
	//	Function:	"CGame(Constructor)"
	//	Last Modified: July 18, 2008
	//	Purpose:	Constructor
	////////////////////////////////////////////
	CGame(void);
	CGame(const CGame&);
	CGame& operator=(const CGame&);

	////////////////////////////////////////////
	//	Function:	"~CGame(Destructor)"
	//	Last Modified: July 18, 2008
	//	Purpose:	Destructor
	////////////////////////////////////////////
	~CGame(void);

public:

	//////////////////////////////////////////////////////
	//	Function: “GetInstance”
	//	Last Modified: July 18, 2008
	//	Purpose: To return an instance of the class
	//////////////////////////////////////////////////////
	static CGame* GetInstance(void);

	//////////////////////////////////////////////////////
	//	Function: “Initialize”
	//	Last Modified: July 18, 2008
	//	Purpose: To initialize any needed information
	//////////////////////////////////////////////////////	
	bool Initialize(HWND hWnd, HINSTANCE hInstance,
					   int nScreenWidth, int nScreenHeight, bool bIsWindowed);
	
	//////////////////////////////////////////////////////
	//	Function: “Main”
	//	Last Modified: July 18, 2008
	//	Purpose: Our main game loop.  This gets input,
	//			updates, and renders the game
	//////////////////////////////////////////////////////	
	bool Main(void);

	//////////////////////////////////////////////////////
	//	Function: “Shutdown”
	//	Last Modified: July 18, 2008
	//	Purpose: To shutdown any needed information
	//////////////////////////////////////////////////////	
	void Shutdown(void);

	//////////////////////////////////////////////////////
	//	Function: “ChangeState”
	//	Last Modified: July 18, 2008
	//	Purpose: To exit all states on the stack and enter
	//			the new state
	//////////////////////////////////////////////////////	
	void ChangeState(IGameState* pNewState);

	//////////////////////////////////////////////////////
	//	Function: “PushState”
	//	Last Modified: July 18, 2008
	//	Purpose: To push on a new state to the stack
	//			This state is now the only one getting
	//			input
	//////////////////////////////////////////////////////	
	void PushState(IGameState* pNewState);

	//////////////////////////////////////////////////////
	//	Function: “PopCurrentState”
	//	Last Modified: July 18, 2008
	//	Purpose: To pop off the current state and move to
	//			the next one if there is one
	//////////////////////////////////////////////////////	
	void PopCurrentState();

	///////////////////////////////////////////
	//  Function: Accessors
	//	Last Modified: July 18, 2008
	//  Purpose : Returns the specified type.
	///////////////////////////////////////////
	inline int GetMusicVolume() { return m_nMusicVolume; }
	inline int GetSFXVolume() { return m_nSFXVolume; }
	inline CCity* GetSelectedCity() { return m_pSelectedCity; }
	inline CCity** GetCities() { return m_pCities; }
	inline CUnit*	GetPlayerUnitInfo() { return m_pPlayerUnitInfo; }
	inline int GetTotalGold() { return m_nGold; }
	inline int IsArcherUnlocked() { return m_chJinCount>=3; }
	inline int IsAxmenUnlocked() { return m_chXiaCount>=3; }
	inline POINT GetMousePos() { return m_ptMousePos; }
	inline int IsWarElephantUnlocked() { return m_chKCount>=3; }
	bool GetIsWindowed(){return m_bIsWindowed;}
	HWND GetWindowHandle(){return m_hWnd;}
	HINSTANCE GetHInstance() { return m_hInstance;}
	bool GetFPSDisplay(){return m_bFPS;}
	inline bool IsMouseInRect(RECT rIsMouseHere)
	{
		return (m_ptMousePos.x <= rIsMouseHere.right && m_ptMousePos.x >= rIsMouseHere.left &&
				m_ptMousePos.y >= rIsMouseHere.top && m_ptMousePos.y <= rIsMouseHere.bottom);
	}
	int GetAttackSound(int nUnitType){return m_nAttackSoundID[nUnitType];}
	int GetDeathSound(int nUnitType){return m_nDeathSoundID[nUnitType];}
	int GetTerrorLevel() { return m_nTerrorLevel; }
	int	GetLoses()		{ return m_nLoses; }
	bool GetUpgraded(int j,int i){return m_bUpGrades[j][i];}
	bool GetTutorialMode(){return m_bTutorial;}
	inline bool GetBoxes(){return m_bShowBoxes;}

	///////////////////////////////////////////
	//  Function: "GetNumConquered"
	//	Last Modified: July 18, 2008
	//  Purpose : Returns the number of conquered cities
	///////////////////////////////////////////
	int GetNumConquered();

	///////////////////////////////////////////
	//  Function: "GetNextFoodTribute"
	//	Last Modified: July 18, 2008
	//  Purpose : Returns the the food tribute 
	//	of the next conquered city
	///////////////////////////////////////////
	int GetNextFoodTribute();

	///////////////////////////////////////////
	//  Function: "GetTotalFoodTribute"
	//	Last Modified: July 18, 2008
	//  Purpose : Returns the total food tribute
	///////////////////////////////////////////
	int GetTotalFoodTribute();

	///////////////////////////////////////////
	//  Function: "GetWindowPosition"
	//	Last Modified: July 18, 2008
	//  Purpose : Returns top-left point of the window
	///////////////////////////////////////////
	POINT GetWindowPosition();

	///////////////////////////////////////////
	//  Function: "GetCursorPosition"
	//	Last Modified: July 18, 2008
	//  Purpose : Returns top-left point of the cursor
	//			  inside of the drawn screen
	///////////////////////////////////////////
	POINT GetCursorPosition();

	///////////////////////////////////////////
	//  Function: Modifiers
	//	Last Modified: July 18, 2008
	//  Purpose : Modifies the specified type.
	///////////////////////////////////////////
	inline void SetIsWindowed(bool bIsWindowed){m_bIsWindowed = bIsWindowed;}
	inline void SetMusicVolume(int nMusicVolume) 
	{
		m_nMusicVolume = nMusicVolume;
		for(int i = 0; i < m_nNumSongs; i++)
			CSGD_WaveManager::GetInstance()->SetVolume(m_nPlayList[i],m_nMusicVolume);
	}
	inline void SetSFXVolume(int nSFXVolume) { m_nSFXVolume = nSFXVolume; }
	inline void SetWindowOffset(int nX, int nY) { m_ptWindOffset.x = nX; m_ptWindOffset.y = nY; }
	inline void SetCursorNormal() { SetCursor(LoadCursor(m_hInstance, MAKEINTRESOURCE(IDC_CURSOR_NORMAL))); }
	inline void SetCursorClick() { SetCursor(LoadCursor(m_hInstance, MAKEINTRESOURCE(IDC_CURSOR_CLICK))); }
	inline void SetCursorAttack() { SetCursor(LoadCursor(m_hInstance, MAKEINTRESOURCE(IDC_CURSOR_ATTACK))); }
	inline void SetSelectedCity(CCity* pSelectedCity) { m_pSelectedCity = pSelectedCity; }
	inline void SetTotalGold(int nTotalGold) { m_nGold = nTotalGold; }
	inline void SetFPSDisplay(bool bFPS) {m_bFPS = bFPS;}
	inline void AddGold(int nGold)  { m_nGold += nGold; }
	void SetSongPlay(int ntoPlay){m_nCurrentSong = ntoPlay;}
	void SetTerrorLevel(int nTerrorLevel) { m_nTerrorLevel = nTerrorLevel; }
	void SetPlayerUnitInfo(CUnit toChange, int nType){m_pPlayerUnitInfo[nType] = toChange;}
	void SetUpgraded(int j,int i,bool value){m_bUpGrades[j][i] = value;}
	void SetTutorialMode(bool value){m_bTutorial = value;}
	inline void UnlockArcher() { m_chJinCount = 3;}
	inline void UnlockAxmen() { m_chXiaCount = 3;}
	inline void UnlockWarElephant() { m_chKCount = 3;}
	void SetBoxes(bool value){m_bShowBoxes = value;}

	///////////////////////////////////////////
	//  Function: ParseXMLUnitInfo
	//	Last Modified: July 18, 2008
	//  Purpose : Reads in base unit stats
	///////////////////////////////////////////
	bool	ParseXMLUnitInfo (const char* szFile);

	///////////////////////////////////////////
	//  Function: ParseBinaryUnitInfo
	//	Last Modified: July 18, 2008
	//  Purpose : Reads in base unit stats
	///////////////////////////////////////////
	bool	ParseBinaryUnitInfo (const char* szFile);


	///////////////////////////////////////////
	//  Function: ParsePlayList
	//	Last Modified: August 22, 2008
	//  Purpose : Load music playlist
	///////////////////////////////////////////
	bool	ParsePlayList		(const char* szFileName);

	///////////////////////////////////////////
	//  Function: InitCities
	//	Last Modified: July 18, 2008
	//  Purpose : Initializes city info
	///////////////////////////////////////////
	void	InitCities();


	///////////////////////////////////////////
	//  Function: GetPlayerUnitInfo
	//	Last Modified: July 18, 2008
	//  Purpose : Returns the player's unit
	//			  info of the specified type.
	//			  The stats of these units should
	//			  reflect any upgrades purchased.
	///////////////////////////////////////////
	CUnit	GetUnitInfo	(int nType);

	///////////////////////////////////////////
	//  Function: GetCPUUnitInfo
	//	Last Modified: July 18, 2008
	//  Purpose : Returns the CPU's unit
	//			  info of the specified type.
	///////////////////////////////////////////
	CUnit	GetCPUUnitInfo		(int nType);

	///////////////////////////////////////////
	//  Function: SetCityConquerored
	//	Last Modified: July 24, 2008
	//  Purpose : Updates all needed values when
	//			  a city has been conquered.
	///////////////////////////////////////////
	void SetCityConquered(CCity* pCity);

	///////////////////////////////////////////
	//  Function: LoseLastCity
	//	Last Modified: July 24, 2008
	//  Purpose : Removes the player's last 
	//	conquered city from their control
	///////////////////////////////////////////
	void LoseLastCity();

	///////////////////////////////////////////
	//  Function: AddWins
	//	Last Modified: August 22, 2008
	//  Purpose : Adds to player wins
	///////////////////////////////////////////
	void AddWins(bool bUsedTerror);

	///////////////////////////////////////////
	//  Function: AddLoses
	//	Last Modified: August 22, 2008
	//  Purpose : Adds to player loses
	///////////////////////////////////////////
	void AddLoses();
	
	///////////////////////////////////////////
	//  Function: LoadSlot
	//	Last Modified: August 20, 2008
	//  Purpose : Loads data from slot
	//	Returns true if loaded, false for empty
	///////////////////////////////////////////
	bool LoadSlot(int nSlot);

	///////////////////////////////////////////
	//  Function: LoadSlot
	//	Last Modified: August 20, 2008
	//  Purpose : saves data to current slot
	//	Returns true if saved
	///////////////////////////////////////////
	bool Save(bool bNewSave);

	///////////////////////////////////////////
	//  Function: NewGame
	//	Last Modified: August 20, 2008
	//  Purpose : Puts all values to start value
	//	This slot will now be saved over
	///////////////////////////////////////////
	void NewGame(int nSlot);

	///////////////////////////////////////////
	//  Function: GetSaveName
	//	Last Modified: August 20, 2008
	//  Purpose : Get save game info
	///////////////////////////////////////////
	string GetSaveName(int nSlot, bool bTitle);

	///////////////////////////////////////////
	//  Function: ParseOptions
	//	Last Modified: August 20, 2008
	//  Purpose : Parse options data
	///////////////////////////////////////////
	void ParseOptions(char* szFileName);

	void SwitchFullScreen(bool bIsWindowed)
	{
		Shutdown();
		this->Initialize(m_hWnd, m_hInstance, 800, 600, bIsWindowed);
		if(!bIsWindowed)
			SetWindowOffset(0, 0);
		else
			SetWindowOffset(8, 31);
		ChangeState(CMainMenuState::GetInstance());
	}

};