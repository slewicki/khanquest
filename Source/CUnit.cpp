#include "CUnit.h"
#include "CCamera.h"
#include "CSGD_Direct3D.h"
CUnit::CUnit(int nType)
{
	m_nHP				= 100;			
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
	m_pHealthBar = new CHealthBar();
	m_pHealthBar->SetHealth(m_nHP);
	m_pAnimInstance = new CAnimInstance(GetType());
	m_pAnimInstance->Play(m_nDirectionFacing, m_nState);
	
}

CUnit::~CUnit(void)
{
	delete m_pAnimInstance;
	delete m_pHealthBar;
}

void CUnit::Update(float fElapsedTime)
{
	m_pAnimInstance->Update(fElapsedTime);

	// Set Global Rect
	//-------------------------------
	m_rGlobalRect.left   = (int)GetPosX();
	m_rGlobalRect.top    = (int)GetPosY();
	m_rGlobalRect.right  = m_rGlobalRect.left + m_pAnimInstance->GetFrameWidth(m_nDirectionFacing, m_nState);
	m_rGlobalRect.bottom = m_rGlobalRect.top + m_pAnimInstance->GetFrameHeight(m_nDirectionFacing, m_nState);
	//-------------------------------

	POINT ptPos = CCamera::GetInstance()->TransformToScreen((int)GetPosX(), (int)GetPosY());
	m_pAnimInstance->SetX(ptPos.x);
	m_pAnimInstance->SetY(ptPos.y);

	// Set Local Rect
	//-------------------------------
	m_rLocalRect.left   = ptPos.x;
	m_rLocalRect.top    = ptPos.y;
	m_rLocalRect.right  = m_rLocalRect.left + m_pAnimInstance->GetFrameWidth(m_nDirectionFacing, m_nState);
	m_rLocalRect.bottom = m_rLocalRect.top + m_pAnimInstance->GetFrameHeight(m_nDirectionFacing, m_nState);


	// Set Health Rect
	//-------------------------------
	m_rHealthRect.left = ptPos.x;
	m_rHealthRect.top = ptPos.y - 10;
	m_rHealthRect.right = m_rHealthRect.left + 60;
	m_rHealthRect.bottom = m_rHealthRect.top - 5;
	int nSwap;
	if(m_rHealthRect.top > m_rHealthRect.bottom)
	{
		nSwap = m_rHealthRect.top;
		m_rHealthRect.top = m_rHealthRect.bottom;
		m_rHealthRect.bottom = nSwap;
	}
	if(m_rHealthRect.left > m_rHealthRect.right)
	{
		nSwap = m_rHealthRect.left;
		m_rHealthRect.left = m_rHealthRect.right;
		m_rHealthRect.right = nSwap;
	}
	//-------------------------------
	// AI
	if ( (m_pDestinationTile != NULL && m_pCurrentTile->ptAnchor.x != m_pDestinationTile->ptAnchor.x ) && ( m_pCurrentTile->ptAnchor.y != m_pDestinationTile->ptAnchor.y) )
	{
		if ( m_pCurrentTile->ptAnchor.x > m_pDestinationTile->ptAnchor.x)
			SetPosY( GetPosX() + GetVelX() );
		else if (m_pCurrentTile->ptAnchor.x < m_pDestinationTile->ptAnchor.x)
			SetPosY( GetPosX() - GetVelX() );

		if (m_pCurrentTile->ptAnchor.y > m_pDestinationTile->ptAnchor.y)
			SetPosY( GetPosY() + GetVelY() );
		else if (m_pCurrentTile->ptAnchor.x < m_pDestinationTile->ptAnchor.y)
			SetPosY( GetPosY() - GetVelY() );
	}
	//-------------------------------

}

void CUnit::Render(float fElapsedTime)
{
	if(m_bIsSelected)
	{
		//CSGD_Direct3D::GetInstance()->SpriteEnd();
		//CSGD_Direct3D::GetInstance()->DeviceEnd();

		CSGD_Direct3D::GetInstance()->DrawPrimitiveRect(m_rLocalRect,D3DCOLOR_ARGB(255,0,255,0));

		//CSGD_Direct3D::GetInstance()->DeviceBegin();
		//CSGD_Direct3D::GetInstance()->SpriteBegin();

		m_pHealthBar->Render(m_rHealthRect);
	}
	if(CCamera::GetInstance()->IsOnScreen(GetGlobalRect()))
		m_pAnimInstance->Render();
	
}

bool CUnit::CheckCollisions(CBase* pBase)
{
	return false;
}