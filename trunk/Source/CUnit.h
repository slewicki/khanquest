//////////////////////////////////////////////////////
//	File	:	"CUnit.h"
//
//	Author	:	Sean Hamstra (SH)
//
//	Purpose	:	Store all information and functionality
//				of a unit.
//////////////////////////////////////////////////////

#pragma once
#include "CBase.h"
#include "AnimInstance.h"
#include "CHealthBar.h"
enum	// Unit States
{
	//IDLE,
	MOVEMENT,
	COMBAT,
	DYING,
	RETREAT,
};

//= "North";
//
//
//= "South";
//
//
//= "West";
//
//
//= "North_West";
//
//
//= "South_West";
//

enum { NORTH, SOUTH, WEST, NORTH_WEST, SOUTH_WEST};

class CTile;
class CUnit : public CBase
{
private:
	// Unit Stats
	int				m_nHP;				// Current Hitpoints
	int				m_nAttack;			// Attack power
	int				m_nRange;			// Attack Range
	int				m_nCost;			// Cost
	float			m_fAttackSpeed;		// Attack Speed
	float			m_fMovementSpeed;	// Movement Speed

	int				m_nBonus;			// Attack bonus or penalty

	bool			m_bIsPlayerUnit;	// Is the unit player controlled
	bool			m_bIsGrouped;		// Is the unit in a group
	
	bool			m_bIsSelected;		// Is the unit selected by the player

	CTile*			m_pDestinationTile; // Tile the unit is traveling to
	CTile*			m_pCurrentTile;		// Current tile the unit is on

	CUnit*			m_pTarget;			// Enemy unit to attack, if any

	int				m_nDirectionFacing; // The direction the unit is facing

	vector<CTile*>	m_vTilePath;		// The path the unit will take to 
										// get to the destination

	int				m_nState;			// The state the unit is in
	CHealthBar*		m_pHealthBar;
	
	CAnimInstance*	m_pAnimInstance;

	RECT			m_rLocalRect;
	RECT			m_rGlobalRect;
	RECT			m_rHealthRect;
	
public:
	//////////////////////////////////////////////////////
	//	Function:	"CUnit(Constructor)"
	//	Last Modified: July 18, 2008
	//	Purpose:	Constructor
	//////////////////////////////////////////////////////
	CUnit() {}
	CUnit(int nType);

	//////////////////////////////////////////////////////
	//	Function:	"CUnit(Destructor)"
	//	Last Modified: July 18, 2008
	//	Purpose:	Destructor
	//////////////////////////////////////////////////////
	~CUnit(void);

	//////////////////////////////////////////////////////
	//	Function:	Accessors
	//	Last Modified: July 18, 2008
	//	Purpose: Gets the specified type
	//////////////////////////////////////////////////////
	// Get unit stats
	inline int	 GetHP			(void) const { return m_nHP; }
	inline int	 GetAttackPower	(void) const { return m_nAttack; }
	inline int	 GetRange		(void) const { return m_nRange; }
	inline float GetAttackSpeed	(void) const { return m_fAttackSpeed; }
	inline float GetSpeed		(void) const { return m_fMovementSpeed; }
	inline int GetCost		(void) const { return m_nCost; }
	inline CTile* GetDestTile	(void) const { return m_pDestinationTile; }
	inline CUnit* GetTarget		(void) const { return m_pTarget; }
	inline bool	  IsSelected	(void) const { return m_bIsSelected; }
	inline bool	  IsGrouped		(void) const { return m_bIsGrouped; }
	inline int    GetState		(void) const { return m_nState; }
	inline int    GetDirection	(void) const { return m_nDirectionFacing; }
	inline RECT    GetLocalRect	(void) const { return m_rLocalRect; }
	inline RECT    GetGlobalRect	(void) const { return m_rGlobalRect; }


	//////////////////////////////////////////////////////
	//	Function:	Mutators
	//	Last Modified: July 18, 2008
	//	Purpose: Sets the specified type
	//////////////////////////////////////////////////////
	inline void SetHP			(int nHP)			{ m_nHP = nHP; }
	inline void SetAttackPower	(int nAttack)		{ m_nAttack = nAttack; }
	inline void SetRange		(int nRange)		{ m_nRange = nRange; }
	inline void SetCost		(int nCost)		{ m_nCost = nCost; }
	inline void SetAttackSpeed	(float fAttackSpeed)	{ m_fAttackSpeed = fAttackSpeed; }
	inline void SetSpeed		(float fMovementSpeed){ m_fMovementSpeed = fMovementSpeed; }
	inline void SetBonus		(int nBonus)		{ m_nBonus = nBonus; }
	inline void SetSelected		(bool bIsSelected)	{ m_bIsSelected = bIsSelected; }
	inline void SetGrouped		(bool bIsGrouped)	{ m_bIsGrouped = bIsGrouped; }
	inline void SetDirection (int nDirectionFacing)	
	{ 
		m_nDirectionFacing = nDirectionFacing; 
		m_pAnimInstance->Play(GetDirection(), GetState());	
	}
	inline void SetState (int nState)				
	{ 
		m_nState = nState; 
		m_pAnimInstance->Play(GetDirection(), GetState());	
	}

	////////////////////////////////////////
	//	Function:	"Update"
	//	Last Modified: July 18, 2008
	//	Purpose: To move the unit's 
	//			 position by its velocity.
	/////////////////////////////////////////
	void Update(float fElapsedTime);

	//////////////////////////////////////////////////////
	// Function: “Render”
	//	Last Modified: July 18, 2008
	// Purpose: To render unit to the screen its position
	//////////////////////////////////////////////////////
	void Render(float fElapsedTime);

	//////////////////////////////////////////////////////
	// Function: “CheckCollisions”
	//	Last Modified: July 18, 2008
	// Purpose: To check if unit collided with anything
	//////////////////////////////////////////////////////
	bool CheckCollisions(CBase* pBase);

	inline void DamageUnit() { m_pHealthBar->DamageHealth(10); }
	
};