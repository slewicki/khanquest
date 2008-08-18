//////////////////////////////////////////////////////
//	File	:	"CUnit.h"
//
//	Author	:	Sean Hamstra (SH)
//
//	Purpose	:	Store all information and functionality
//				of a unit.
//////////////////////////////////////////////////////
#pragma once
#include <vector>
#include "CBase.h"
#include "AnimInstance.h"
#include "CTile.h"
#include "CAISystem.h"
#include "CTileEngine.h"
using std::vector;
#define VISIBILITY 6
enum	// Unit States
{
	MOVEMENT,
	COMBAT,
	DYING,
	IDLE,
	RETREAT,

};
enum { NORTH, SOUTH, WEST, NORTH_WEST, SOUTH_WEST};

class CTile;
class CUnit : public CBase
{
private:
	// Unit Stats
	int				m_nMaxHP;			// Starting Hitpoints
	int				m_nCurrentHP;		// Current Hitpoints
	int				m_nAttack;			// Attack power
	int				m_nRange;			// Attack Range
	int				m_nCost;			// Cost
	float			m_fAttackSpeed;		// Attack Speed
	float			m_fMovementSpeed;	// Movement Speed
	float			m_fAttackTimer;		// Current time between attacks
	int				m_nBonus;			// Attack bonus or penalty

	bool			m_bIsPlayerUnit;	// Is the unit player controlled
	bool			m_bIsGrouped;		// Is the unit in a group
	bool			m_bIsAlive;			// Is the unit alive
	bool			m_bIsSelected;		// Is the unit selected by the player

	CTile*			m_pDestinationTile; // Tile the unit is traveling to
	CTile*			m_pCurrentTile;		// Current tile the unit is on
	CTile*			m_pNextTile;		// next tile to move to

	CUnit*			m_pTarget;			// Enemy unit to attack, if any

	int				m_nDirectionFacing; // The direction the unit is facing

	vector<CTile*>	m_vTilePath;		// The path the unit will take to 
										// get to the destination

	int				m_nState;			// The state the unit is in
	int				m_nSelectionID;
	CAnimInstance*	m_pAnimInstance;
	CTileEngine*	m_pTE;

	CAISystem*		m_pCAI;
	vector<CUnit*>	m_vAttackers;

	RECT			m_rLocalRect;
	RECT			m_rGlobalRect;
	RECT			m_rHealthRect;
	float			m_fMovementTimer;

	list<POINT>	m_vPath;				// unit path
	
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
	inline int	  GetMaxHP				(void) const { return m_nMaxHP; }
	inline int	  GetHealth			(void) const {return m_nCurrentHP;}
	inline int	  GetAttackPower	(void) const { return m_nAttack; }
	inline int	  GetRange			(void) const { return m_nRange; }
	inline float  GetAttackSpeed	(void) const { return m_fAttackSpeed; }
	inline float  GetSpeed			(void) const { return m_fMovementSpeed; }
	inline int	  GetCost			(void) const { return m_nCost; }
	inline CTile* GetDestTile		(void) const { return m_pDestinationTile; }
	inline CTile* GetCurrentTile		(void) const { return m_pCurrentTile; }
	inline CTile* GetNextTile		(void) const {return m_pNextTile;}
	inline bool IsPlayerUnit(void) {return m_bIsPlayerUnit;}

	inline CUnit* GetTarget			(void) const { return m_pTarget; }
	inline bool	  IsSelected		(void) const { return m_bIsSelected; }
	inline bool	  IsAlive			(void) const { if(this == NULL) return false;
													else return m_bIsAlive; }
	inline bool	  IsGrouped			(void) const { return m_bIsGrouped; }
	inline int    GetState			(void) const { return m_nState; }
	inline int    GetDirection		(void) const { return m_nDirectionFacing; }
	inline RECT   GetLocalRect	    (void) const { return m_rLocalRect; }
	inline RECT   GetGlobalRect	(void) const { return m_rGlobalRect; }
	inline vector<CUnit*>   GetAttackerList	(void) const { return m_vAttackers; }


	//////////////////////////////////////////////////////
	//	Function:	Mutators
	//	Last Modified: July 18, 2008
	//	Purpose: Sets the specified type
	//////////////////////////////////////////////////////
	inline void SetMaxHP			(int nHP)				{ m_nMaxHP = nHP; }
	inline void SetCurrentHP	(int nHP)					{ m_nCurrentHP = nHP; }
	inline void SetAttackPower	(int nAttack)			{ m_nAttack = nAttack; }
	inline void SetRange		(int nRange)			{ m_nRange = nRange; }
	inline void SetCost			(int nCost)				{ m_nCost = nCost; }
	inline void SetIsPlayerUnit (bool bIsPlayerUnit)	{m_bIsPlayerUnit = bIsPlayerUnit; m_pAnimInstance->SetPlayer(m_bIsPlayerUnit);}
	inline void SetAttackSpeed	(float fAttackSpeed)	{ m_fAttackSpeed = fAttackSpeed; }
	inline void SetSpeed		(float fMovementSpeed){ m_fMovementSpeed = fMovementSpeed; }
	inline void SetDestTile		(CTile* pDestTile)  { m_pDestinationTile = pDestTile; }
	inline void SetCurrentTile	(CTile* pCurrentTile)  { m_pCurrentTile = pCurrentTile; }
	inline void SetNextTile		(CTile* pNextTile)  { m_pNextTile = pNextTile; }
	inline void SetBonus		(int nBonus)		{ m_nBonus = nBonus; }
	inline void SetSelected		(bool bIsSelected)	{ m_bIsSelected = bIsSelected; }
	inline void SetGrouped		(bool bIsGrouped)	{ m_bIsGrouped = bIsGrouped; }
	inline void SetPath(list<POINT> vPath)		{m_vPath = vPath;}
	inline void ClearPath		(void)			{m_vPath.clear();}
	inline void AddAttacker(CUnit* pAttacker) { m_vAttackers.push_back(pAttacker); }
	void RemoveAttacker(CUnit* pAttacker)
	{
		vector<CUnit*>::iterator iter;
		for (iter = m_vAttackers.begin(); iter < m_vAttackers.end(); iter++)
		{
			if((*iter) == pAttacker)
			{
				m_vAttackers.erase(iter);
				break;
			}
		}
	}
	void SetTarget(CUnit* pTarget)		
	{
		// If we have a target tell him we no longer are attaking
		/*if(m_pTarget)
		{
			m_pTarget->RemoveAttacker(this);
		}*/
		// Set our new target
		m_pTarget = pTarget;
		// Tell him we are attacking if he is not NULL
		if(m_pTarget)
			m_pTarget->AddAttacker(this);
	
	}
	inline void SetDirection	(int nDirectionFacing)	
	{ 
		m_nDirectionFacing = nDirectionFacing; 
		m_pAnimInstance->Play(GetDirection(), GetState());	
	}
	inline void SetState (int nState)				
	{ 
		m_nState = nState; 
		//m_pAnimInstance->Play(GetDirection(), GetState());	
	}

	inline void ClearAttackerList()
	{
		m_vAttackers.clear();
	}


	////////////////////////////////////////
	//	Function:	"Update"
	//	Last Modified: July 18, 2008
	//	Purpose: To move the unit's 
	//			 position by its velocity.
	/////////////////////////////////////////
	void Update(float fElapsedTime);
	void RenderHealth();


	//////////////////////////////////////////////////////
	// Function: “Render”
	//	Last Modified: July 18, 2008
	// Purpose: To render unit to the screen its position
	//////////////////////////////////////////////////////
	void Render(float fElapsedTime);
	void RenderSelection();


	//////////////////////////////////////////////////////
	// Function: “CheckCollisions”
	//	Last Modified: July 18, 2008
	// Purpose: To check if unit collided with anything
	//////////////////////////////////////////////////////


	//////////////////////////////////////////////////////
	// Function: “ChangeDirection”
	//	Last Modified: July 18, 2008
	// Purpose: Change the direction the unit is facing
	//////////////////////////////////////////////////////
	void ChangeDirection(CTile* pTileFacing);
	
	//////////////////////////////////////////////////////
	// Function: “ResolveCombat”
	//	Last Modified: August 14, 2008
	// Purpose: Resove combat between units
	//////////////////////////////////////////////////////
	void ResolveCombat();

	//////////////////////////////////////////////////////
	// Function: “CalcAttackBonus”
	//	Last Modified: August 14, 2008
	// Purpose: Factor in unit attack bounces
	//////////////////////////////////////////////////////
	void CalcAttackBonus();

	//////////////////////////////////////////////////////
	// Function: “UpdateVisibility”
	//	Last Modified: July 18, 2008
	// Purpose: update visablity of the map
	//////////////////////////////////////////////////////
	void UpdateVisibility();

	CTile* PlaceOnSurrounding(CTile* pCenterTile);
	// True if we reach the next tile
	bool MoveUnit();

	bool IsTargetInRange();

	void ScanForEnemies();


};