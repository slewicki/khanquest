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

#include "IGameState.h"
#include <windows.h>
#include "CUnit.h"
#include "CCity.h"
#include "../resource.h"	//  For icon/cursor ID

using std::string;

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
	// Time and Frame Variables
	DWORD					m_dwPreviousTime;
	int						m_nFrameCounter;	//counts number of frames per second
	int						m_nFPS;				// prints number
	DWORD					m_dwFrameTimer;
	
	bool					m_bIsWindowed;		// Is the game windowed?
	float					m_fGameTime;		// Time since the game began
	int						m_nSFXVolume;		// Sound Effects Volume
	int						m_nMusicVolume;		// Music Volume
	vector<IGameState*>		m_vStates;			// Stack based states

	CUnit					m_pPlayerUnitInfo[6];	// Info for player's units
	CUnit					m_pCPUUnitInfo[6];		// Info for CPU's units

	int						m_nGold;				// The player's current gold amount

	HWND					m_hWnd;
	HINSTANCE				m_hInstance;

	POINT					m_ptWindOffset;
	POINT					m_ptMousePos;

	CCity*					m_pCities[10];
	CCity*					m_pSelectedCity;

	char					m_chXiaCount;
	char					m_chKCount;
	char					m_chJinCount;

	vector<int>				m_vConqueredCities;
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
	//	Function: �GetInstance�
	//	Last Modified: July 18, 2008
	//	Purpose: To return an instance of the class
	//////////////////////////////////////////////////////
	static CGame* GetInstance(void);

	//////////////////////////////////////////////////////
	//	Function: �Initialize�
	//	Last Modified: July 18, 2008
	//	Purpose: To initialize any needed information
	//////////////////////////////////////////////////////	
	bool Initialize(HWND hWnd, HINSTANCE hInstance,
					   int nScreenWidth, int nScreenHeight, bool bIsWindowed);
	
	//////////////////////////////////////////////////////
	//	Function: �Main�
	//	Last Modified: July 18, 2008
	//	Purpose: Our main game loop.  This gets input,
	//			updates, and renders the game
	//////////////////////////////////////////////////////	
	bool Main(void);

	//////////////////////////////////////////////////////
	//	Function: �Shutdown�
	//	Last Modified: July 18, 2008
	//	Purpose: To shutdown any needed information
	//////////////////////////////////////////////////////	
	void Shutdown(void);

	//////////////////////////////////////////////////////
	//	Function: �ChangeState�
	//	Last Modified: July 18, 2008
	//	Purpose: To exit all states on the stack and enter
	//			the new state
	//////////////////////////////////////////////////////	
	void ChangeState(IGameState* pNewState);

	//////////////////////////////////////////////////////
	//	Function: �PushState�
	//	Last Modified: July 18, 2008
	//	Purpose: To push on a new state to the stack
	//			This state is now the only one getting
	//			input
	//////////////////////////////////////////////////////	
	void PushState(IGameState* pNewState);

	//////////////////////////////////////////////////////
	//	Function: �PopCurrentState�
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
	inline int IsWarElephantUnlocked() { return m_chKCount>=3; }
	inline bool IsMouseInRect(RECT rIsMouseHere)
	{
		return (m_ptMousePos.x <= rIsMouseHere.right && m_ptMousePos.x >= rIsMouseHere.left &&
				m_ptMousePos.y >= rIsMouseHere.top && m_ptMousePos.y <= rIsMouseHere.bottom);
	}

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
	inline void SetMusicVolume(int nMusicVolume) {m_nMusicVolume = nMusicVolume; }
	inline void SetSFXVolume(int nSFXVolume) { m_nSFXVolume = nSFXVolume; }
	inline void SetWindowOffset(int nX, int nY) { m_ptWindOffset.x = nX; m_ptWindOffset.y = nY; }
	inline void SetCursorNormal() { SetCursor(LoadCursor(m_hInstance, MAKEINTRESOURCE(IDC_CURSOR_NORMAL))); }
	inline void SetCursorClick() { SetCursor(LoadCursor(m_hInstance, MAKEINTRESOURCE(IDC_CURSOR_CLICK))); }
	inline void SetCursorAttack() { SetCursor(LoadCursor(m_hInstance, MAKEINTRESOURCE(IDC_CURSOR_ATTACK))); }
	inline void SetSelectedCity(CCity* pSelectedCity) { m_pSelectedCity = pSelectedCity; }
	inline void SetTotalGold(int nTotalGold) { m_nGold = nTotalGold; }



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
	//  Function: InitCities
	//	Last Modified: July 18, 2008
	//  Purpose : Initializes city info
	///////////////////////////////////////////
	void	InitCities();

	///////////////////////////////////////////
	//  Function: LoadGame
	//	Last Modified: July 18, 2008
	//  Purpose : Load game info from file
	///////////////////////////////////////////
	bool	LoadGame(const char* szSaveFile);

	///////////////////////////////////////////
	//  Function: GetPlayerUnitInfo
	//	Last Modified: July 18, 2008
	//  Purpose : Returns the player's unit
	//			  info of the specified type.
	//			  The stats of these units should
	//			  reflect any upgrades purchased.
	///////////////////////////////////////////
	CUnit	GetPlayerUnitInfo	(int nType);

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

};