#pragma once
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
// File: �Animation.h� 
// Author: Dennis Wallace II (DWII) 
// Purpose: This is the file containing the information for the
//			Animation class and the frame structure that the animation hold.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
#include "CEventSystem.h"
#include <windows.h>
#include <vector>
#include <string>
#include "CProfile.h"
using namespace std;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
// Structure: �Frame� 
// Last Modified: July 21, 2008 
// Purpose: Holds data for a single frame of animation
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
struct Frame
{
	RECT rRender;
	RECT rPassive;
	RECT rActive;
	float fDuration;
	int ptAnchorX;
	int ptAnchorY;
	string szTriggerType;
	string szTriggerName;
	vector<POINT> ptAccessories;
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
// Class: �Animation� 
// Last Modified: July 21, 2008 
// Purpose: Holds data for a single frame of animation
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
class CAnimation
{
	string	m_szName;
	vector<Frame> m_vFrames;
	float	m_fSpeed;
	float	m_fFrameTimer;
	
	int		m_nPlayerAnimationID;
	int		m_nAIAnimationID;
	int		m_nAlpha;
	unsigned int m_nCurrentFrame;
	bool	m_bIsFading;
	bool	m_bIsPlayer;
	bool	m_bIsPlaying;
	bool	m_bIsLooping;
	float	m_fFadeTimer;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
// Function: �Reset� 
// Last Modified: July 21, 2008 
// Purpose: Resets the animation
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
	void Reset();

public:
	CAnimation(void);
	~CAnimation(void);
	
#pragma region Accessors / Mutators
	//////////////////////////////////////////////////////////////////////////
	//  Function: Accessors
	//	Last Modified: August 04, 2008
	//  Purpose : Returns the specified type.
	//////////////////////////////////////////////////////////////////////////
	string GetName(){return m_szName;}
	void SetName(string value){m_szName = value;}
	float GetSpeed(){return m_fSpeed;}
	void SetSpeed(float value){m_fSpeed = value;}
	vector<Frame> GetFrames(){return m_vFrames;}
	int GetCurrentFrame(){return m_nCurrentFrame;}
	void IsLooping(bool ToLoop){m_bIsLooping = ToLoop;}
	bool IsPlaying(){return m_bIsPlaying;}
	void StartFadeTimer() { m_bIsFading = true; }


	//////////////////////////////////////////////////////////////////////////
	// Function: Modifiers
	//	Last Modified: August 04, 2008
	//  Purpose: Modifies the specified type.
	//////////////////////////////////////////////////////////////////////////
	void SetPlayer(bool value){m_bIsPlayer = value;}
	void SetPlayerAnimationID(int nPlayerAnimationID) {m_nPlayerAnimationID = nPlayerAnimationID;}
	void SetAIAnimationID(int nAIAnimationID){m_nAIAnimationID = nAIAnimationID;}
	void SetDurration(float fDurr){m_vFrames[m_nCurrentFrame].fDuration = fDurr;}
#pragma endregion

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
// Function: �AddFrame� 
// Last Modified: June 21, 2008 
// Purpose: Adds a frame to the animation
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
	void AddFrame(Frame toAdd) {m_vFrames.push_back(toAdd); }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
// Function: �Play� 
// Last Modified: June 27, 2008 
// Purpose: Plays an animation
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
	void Play();

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
// Function: �Stop� 
// Last Modified: July 21, 2008
// Purpose: Stop an animation 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
	void Stop();

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
// Function: �Resume� 
// Last Modified: July 21, 2008 
// Purpose: Resume an animation
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
	void Resume();

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
// Function: �Update� 
// Last Modified: July 27, 2008 
// Purpose: Update the animation
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
	bool Update(float fElapsedTime);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
// Function: �Render� 
// Last Modified: July 27, 2008
// Purpose: Renders the animation
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
	void Render( int x, int y, float scale, DWORD color,  bool  isfliped,float rotation = 0);

};