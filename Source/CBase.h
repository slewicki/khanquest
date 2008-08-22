//////////////////////////////////////////////////////
//	File	:	"CBase.h"
//
//	Author	:	Sean Hamstra (SH)
//
//	Purpose	:	The base object for our class hierarchy
//				and implements our base interface.
//////////////////////////////////////////////////////

#pragma once
#include "IBaseInterface.h"
#include <windows.h>
#include "SGD_Math.h"
#include <vector>
#include "CProfile.h"
using std::vector;

enum 	// Type of object
{
	UNIT_INFANTRY = 0,
	UNIT_CAVALRY,
	UNIT_CAVALRY_ARCHER,
	UNIT_AXMEN,
	UNIT_ARCHER,
	UNIT_WAR_ELEPHANT,
};



class CBase : public IBaseInterface
{
private:
	// Reference count:
	unsigned int m_unRefCount;	//	how many objects have pointers to me.
	
	// position
	tVector2D m_vPos;
	tVector2D m_vVel;

	int m_nType;
	
	bool m_bIsOnScreen;

	int m_nAttackSoundID;
	int m_nDeathSoundID;


public:
	CBase(void);
	virtual ~CBase(void);

	//////////////////////////////////////////////////////
	//	Function:	Accessors
	//
	//	Purpose: Gets the specified type
	//////////////////////////////////////////////////////
	inline float GetPosX(void) const	{ return m_vPos.fX; }
	inline float GetPosY(void) const	{ return m_vPos.fY; }
	inline float GetVelX(void) const	{ return m_vVel.fX; }
	inline float GetVelY(void) const	{ return m_vVel.fY; }
	inline int	GetType(void) const	{ return m_nType; }
	
	inline int GetAttackSoundID(void) const { return m_nAttackSoundID; }
	inline int GetDeathSoundID(void) const { return m_nDeathSoundID; }


	//////////////////////////////////////////////////////
	//	Function:	Mutators
	//
	//	Purpose: Sets the specified type
	//////////////////////////////////////////////////////
	inline void SetPosX(float fPosX)	{ m_vPos.fX = fPosX; }
	inline void SetPosY(float fPosY)	{ m_vPos.fY = fPosY; }
	inline void SetVelX(float nVelX)	{ m_vVel.fX = nVelX; }
	inline void SetVelY(float nVelY)	{ m_vVel.fY = nVelY; }
	inline void SetType(int nType)		{ m_nType = nType;	}
	inline void SetOnScreen (bool bIsOnScreen) { m_bIsOnScreen = bIsOnScreen; }
	inline void SetAttackSoundID(int nAttackSoundID) { m_nAttackSoundID = nAttackSoundID; }
	inline void SetDeathSoundID(int nDeathSoundID) { m_nDeathSoundID = nDeathSoundID; }


	
	////////////////////////////////////////
	//	Function:	"Update"
	//
	//	Purpose: To move the object's 
	//			 position by its velocity.
	/////////////////////////////////////////
	virtual void Update(float fElapsedTime);

	//////////////////////////////////////////////////////
	// Function: “Render”
	//
	// Purpose: To render object to the screen by its
	//			sprite section and position
	//////////////////////////////////////////////////////
	virtual void Render(float fElapsedTime);

	//////////////////////////////////////////////////////
	// Function: “CheckCollisions”
	//
	// Purpose: To check if object collided with anything
	// Return: True if collided
	//////////////////////////////////////////////////////
	virtual bool CheckCollisions(CBase* pBase);

	//////////////////////////////////////////////////////
	// Function: “AddRef”
	//
	// Purpose: To increase our reference counter
	//////////////////////////////////////////////////////
	void AddRef(void)
	{
		m_unRefCount++;
	}

	//////////////////////////////////////////////////////
	// Function: “Release”
	//
	// Purpose: To decrease our reference counter
	//			If we have no friends we kill ourselves
	//////////////////////////////////////////////////////
	void Release(void)
	{
		m_unRefCount--;

		//	Delete self when no more references exist
		if (m_unRefCount == 0)
			delete this;
	}

};