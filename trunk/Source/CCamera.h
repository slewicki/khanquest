//////////////////////////////////////////////////////
//	File	:	"CCamera.h"
//
//	Author	:	Sean Hamstra (SH)
//
//	Purpose	:	The base object for our class hierarchy
//				and implements our base interface.
//////////////////////////////////////////////////////

#pragma once
#include <windows.h>
#include "SGD_Math.h"

class CCamera
{
private:
	
	// position
	tVector2D m_vPos;
	tVector2D m_vVel;
	RECT		m_rScreenArea;
	CCamera(void){};
	~CCamera(void){};

public:
	//////////////////////////////////////////////////////
	//	Function: “GetInstance”
	//	Last Modified: July 18, 2008
	//	Purpose: To return an instance of the class
	//////////////////////////////////////////////////////
	static CCamera* GetInstance(void)
	{
		static CCamera instance;						
		return &instance;
	}

	void InitCamera(float fStartPosX, float fStartPosY);

	void CenterCamera(int nPosX, int nPosY);

	//////////////////////////////////////////////////////
	//	Function:	Accessors
	//
	//	Purpose: Gets the specified type
	//////////////////////////////////////////////////////
	float GetPosX() { return m_vPos.fX; }
	float GetPosY() { return m_vPos.fY; }
	RECT GetScreenArea() { return m_rScreenArea; }


	//////////////////////////////////////////////////////
	//	Function:	Mutators
	//
	//	Purpose: Sets the specified type
	//////////////////////////////////////////////////////
	inline void SetPosX(float fPosX)	{ m_vPos.fX = fPosX; }
	inline void SetPosY(float fPosY)	{ m_vPos.fY = fPosY; }
	inline void SetVelX(float nVelX)	{ m_vVel.fX = nVelX; }
	inline void SetVelY(float nVelY)	{ m_vVel.fY = nVelY; }
	
	////////////////////////////////////////
	//	Function:	"Update"
	//
	//	Purpose: To move the camera's 
	//			 position by its velocity.
	/////////////////////////////////////////
	void Update(float fElapsedTime);

	//////////////////////////////////////////////////////
	// Function: “TransformToScreen”
	//
	// Purpose: Takes in the position of an object in world
	//			space and returns its position to be drawn 
	//			in camera space
	//////////////////////////////////////////////////////
	tVector2D TransformToScreen(tVector2D vWorldPos);
	tVector2D TransformToScreen(float fWorldPosX, float fWorldPosY);
	POINT TransformToScreen(POINT ptWorldPos);
	POINT TransformToScreen(int nWorldPosX, int nWorldPosY);

	//////////////////////////////////////////////////////
	// Function: “TransformToGlobal”
	//
	// Purpose: Takes in the position of an object in screen
	//			space and returns its position to world space
	//////////////////////////////////////////////////////
	tVector2D TransformToGlobal(tVector2D vCamPos);
	tVector2D TransformToGlobal(float fCamPosX, float fCamPosY);
	POINT TransformToGlobal(POINT ptCamPos);
	POINT TransformToGlobal(int nCamPosX, int nCamPosY);


	//////////////////////////////////////////////////////
	// Function: “IsOnScreen”
	//
	// Purpose: Tells if the object's rect should be drawn
	//			to the screen (object rect in World Space);
	//////////////////////////////////////////////////////
	bool	IsOnScreen(RECT rObjectArea);


};