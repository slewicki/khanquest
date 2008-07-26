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

enum	// Unit States
{
	IDLE,
	MOVEMENT,
	COMBAT,
	RETREAT,
};

enum { NORTH, SOUTH, EAST, WEST, NE, NW, SE, SW};

class CTile;
class CUnit : public CBase
{
private:
	// Unit Stats
	int				m_nHP;				// Current Hitpoints
	int				m_nAttack;			// Attack power
	int				m_nRange;			// Attack Range
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

public:
	//////////////////////////////////////////////////////
	//	Function:	"CUnit(Constructor)"
	//	Last Modified: July 18, 2008
	//	Purpose:	Constructor
	//////////////////////////////////////////////////////
	CUnit(void);

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
	
	inline CTile* GetDestTile	(void) const { return m_pDestinationTile; }
	inline CUnit* GetTarget		(void) const { return m_pTarget; }
	inline bool	  IsSelected	(void) const { return m_bIsSelected; }
	inline bool	  IsGrouped		(void) const { return m_bIsGrouped; }
	inline int    GetState		(void) const { return m_nState; }


	//////////////////////////////////////////////////////
	//	Function:	Mutators
	//	Last Modified: July 18, 2008
	//	Purpose: Sets the specified type
	//////////////////////////////////////////////////////
	inline void SetHP			(int nHP)			{ m_nHP = nHP; }
	inline void SetAttackPower	(int nAttack)		{ m_nAttack = nAttack; }
	inline void SetRange		(int nRange)		{ m_nRange = nRange; }
	inline void SetAttackSpeed	(float fAttackSpeed)	{ m_fAttackSpeed = fAttackSpeed; }
	inline void SetSpeed		(float fMovementSpeed){ m_fMovementSpeed = fMovementSpeed; }
	inline void SetBonus		(int nBonus)		{ m_nBonus = nBonus; }
	inline void SetSelected		(bool bIsSelected)	{ m_bIsSelected = bIsSelected; }
	inline void SetGrouped		(bool bIsGrouped)	{ m_bIsGrouped = bIsGrouped; }
	
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

	

};