#include "CUnit.h"
#include "CCamera.h"

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

	// Set Global Rect
	//-------------------------------
	m_rGlobalRect.left = (int)GetPosX();
	m_rGlobalRect.top = (int)GetPosY();
	m_rGlobalRect.right = m_rGlobalRect.left + m_pAnimInstance->GetFrameWidth(m_nDirectionFacing, m_nState);
	m_rGlobalRect.bottom = m_rGlobalRect.top + m_pAnimInstance->GetFrameHeight(m_nDirectionFacing, m_nState);
	//-------------------------------

	POINT ptPos = CCamera::GetInstance()->TransformToScreen((int)GetPosX(), (int)GetPosY());
	m_pAnimInstance->SetX(ptPos.x);
	m_pAnimInstance->SetY(ptPos.y);

	// Set Local Rect
	//-------------------------------
	m_rLocalRect.left = ptPos.x;
	m_rLocalRect.top = ptPos.y;
	m_rLocalRect.right = m_rGlobalRect.left + m_pAnimInstance->GetFrameWidth(m_nDirectionFacing, m_nState);
	m_rLocalRect.bottom = m_rGlobalRect.top + m_pAnimInstance->GetFrameHeight(m_nDirectionFacing, m_nState);
	//-------------------------------
	// AI
}

void CUnit::Render(float fElapsedTime)
{
	
	if(CCamera::GetInstance()->IsOnScreen(GetGlobalRect()))
		m_pAnimInstance->Render();
}

bool CUnit::CheckCollisions(CBase* pBase)
{
	return false;
}