#include "CUnit.h"

CUnit::CUnit(int nType)
{
	m_nHP				= 0;			
	m_nAttack			= 0;			
	m_nRange			= 0;			
	m_fAttackSpeed		= 0.f;		
	m_fMovementSpeed	= 0.f;	
	m_nCost				= 0;

	m_nBonus			= 0;			

	m_bIsPlayerUnit		= false;	
	m_bIsGrouped		= false;		

	m_bIsSelected		= false;		

	m_pDestinationTile	= NULL; 
	m_pCurrentTile		= NULL;		

	m_pTarget			= NULL;		

	m_nDirectionFacing	= SOUTH_WEST; 
	m_nState			= MOVEMENT;	


	SetType(nType);
	m_pAnimInstance = new CAnimInstance(GetType());
	m_pAnimInstance->Play(m_nDirectionFacing, m_nState);
	
}

CUnit::~CUnit(void)
{
	delete m_pAnimInstance;
}

void CUnit::Update(float fElapsedTime)
{
	m_pAnimInstance->Update(fElapsedTime);
	m_pAnimInstance->SetX((int)GetPosX());
	m_pAnimInstance->SetY((int)GetPosY());
	// AI
}

void CUnit::Render(float fElapsedTime)
{
	m_pAnimInstance->Render();
}

bool CUnit::CheckCollisions(CBase* pBase)
{
	return false;
}