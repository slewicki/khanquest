#include "CUnit.h"

CUnit::CUnit(void)
{
	m_nHP				= 0;			
	m_nAttack			= 0;			
	m_nRange			= 0;			
	m_fAttackSpeed		= 0.f;		
	m_fMovementSpeed	= 0.f;	

	m_nBonus			= 0;			

	m_bIsPlayerUnit		= false;	
	m_bIsGrouped		= false;		

	m_bIsSelected		= false;		

	m_pDestinationTile	= NULL; 
	m_pCurrentTile		= NULL;		

	m_pTarget			= NULL;			

	m_nDirectionFacing	= SOUTH; 
	m_nState			= IDLE;			
	
}

CUnit::~CUnit(void)
{

}

void CUnit::Update(float fElapsedTime)
{
	// AI
}

void CUnit::Render(float fElapsedTime)
{

}

bool CUnit::CheckCollisions(CBase* pBase)
{
	return false;
}