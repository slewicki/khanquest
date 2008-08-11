#include "CUnit.h"
#include "CCamera.h"
#include "CSGD_Direct3D.h"
#include "SGD_Math.h"


CUnit::CUnit(int nType)
{
	m_nHP				= 100;			
	m_nAttack			= 0;			
	m_nRange			= 0;			
	m_fAttackSpeed		= 0.f;		
	m_fMovementSpeed	= 0.f;	
	m_nCost				= 0;

	m_nBonus			= 0;			

	m_bIsPlayerUnit		= true;	
	m_bIsGrouped		= false;		
	m_bIsAlive			= true;
	m_bIsSelected		= false;		

	m_pTarget			= NULL;		

	m_nDirectionFacing	= SOUTH_WEST; 
	m_nState			= IDLE;	
	
	SetType(nType);
	m_pHealthBar = new CHealthBar();
	m_pHealthBar->SetHealth(m_nHP);
//	m_pCAI = NULL;
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
	
	if(m_bIsAlive)
	{
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
		if(GetHP() <= 0)
		{
			m_pAnimInstance->Stop(m_nDirectionFacing, m_nState);
			m_bIsAlive = false;
			m_nState = DYING;
			m_pAnimInstance->Play(m_nDirectionFacing, m_nState);
			m_pAnimInstance->SetLooping(false);
		}
	}
	//-------------------------------
	// AI movement
	if ( GetPosX() == GetDestTile().ptLocalAnchor.x  && GetPosY() == GetDestTile().ptLocalAnchor.y  )
	{
		SetCurrentTile( GetDestTile() );
		SetDestTile( GetCurrentTile() );
		SetState(MOVEMENT);
	}
	else if( GetState() == MOVEMENT )
	{
		//m_pCAI = CAISystem::GetInstance();
		//m_pCAI->FindPath(m_pCurrentTile, m_pDestinationTile);

		if ( GetPosX() >  GetDestTile().ptLocalAnchor.x)
			SetPosX(GetPosX() - GetVelX() );

		else if ( GetPosX() < GetDestTile().ptLocalAnchor.x)
			SetPosX( GetPosX() +  GetVelX() );

		if ( GetPosY() >  GetDestTile().ptLocalAnchor.y)
			SetPosY( GetPosY() - GetVelY() );

		else if ( GetPosY() <  GetDestTile().ptLocalAnchor.y)
			SetPosY( GetPosY() +  GetVelY() );
	}
	//-------------------------------
}

void CUnit::Render(float fElapsedTime)
{
	if(m_bIsAlive)
	{
		if(m_bIsSelected)
		{
			CSGD_Direct3D::GetInstance()->SpriteEnd();
			CSGD_Direct3D::GetInstance()->DeviceEnd();

			CSGD_Direct3D::GetInstance()->DrawPrimitiveRect(m_rLocalRect,D3DCOLOR_ARGB(255,0,255,0));

			CSGD_Direct3D::GetInstance()->DeviceBegin();
			CSGD_Direct3D::GetInstance()->SpriteBegin();

			m_pHealthBar->Render(m_rHealthRect);
		}
	}
	if(CCamera::GetInstance()->IsOnScreen(GetGlobalRect()))
		m_pAnimInstance->Render();
}

bool CUnit::CheckCollisions(CBase* pBase)
{
	return false;
}
void CUnit::ChangeDirection(POINT pMousePos)
{
	if(pMousePos.y < GetLocalRect().top && pMousePos.x < GetLocalRect().left)
	{
		m_pAnimInstance->Stop(m_nDirectionFacing, m_nState);
		m_nDirectionFacing = NORTH_WEST;
		m_pAnimInstance->SetFlip(false);
		m_pAnimInstance->Play(m_nDirectionFacing, m_nState);
		return;
	}
	else if(pMousePos.y < GetLocalRect().top && pMousePos.x > GetLocalRect().right)
	{
		m_pAnimInstance->Stop(m_nDirectionFacing, m_nState);
		m_nDirectionFacing = NORTH_WEST;
		m_pAnimInstance->SetFlip(true);
		m_pAnimInstance->Play(m_nDirectionFacing, m_nState);
		return;
	}
	else if(pMousePos.y > GetLocalRect().bottom && pMousePos.x < GetLocalRect().left)
	{
		m_pAnimInstance->Stop(m_nDirectionFacing, m_nState);
		m_nDirectionFacing = SOUTH_WEST;
		m_pAnimInstance->SetFlip(false);
		m_pAnimInstance->Play(m_nDirectionFacing, m_nState);
		return;
	}
	else if(pMousePos.y > GetLocalRect().bottom && pMousePos.x > GetLocalRect().right)
	{
		m_pAnimInstance->Stop(m_nDirectionFacing, m_nState);
		m_nDirectionFacing = SOUTH_WEST;
		m_pAnimInstance->SetFlip(true);
		m_pAnimInstance->Play(m_nDirectionFacing, m_nState);
		return;
	}
	else if(pMousePos.x < GetLocalRect().left)
	{
		m_pAnimInstance->Stop(m_nDirectionFacing, m_nState);
		m_nDirectionFacing = WEST;
		m_pAnimInstance->SetFlip(false);
		m_pAnimInstance->Play(m_nDirectionFacing, m_nState);
		return;
	}
	else if(pMousePos.x > GetLocalRect().right)
	{
		m_pAnimInstance->Stop(m_nDirectionFacing, m_nState);
		m_nDirectionFacing = WEST;
		m_pAnimInstance->SetFlip(true);
		m_pAnimInstance->Play(m_nDirectionFacing, m_nState);
		return;
	}
	else if(pMousePos.y < GetLocalRect().top)
	{
		m_pAnimInstance->Stop(m_nDirectionFacing, m_nState);
		m_nDirectionFacing = NORTH;
		m_pAnimInstance->SetFlip(false);
		m_pAnimInstance->Play(m_nDirectionFacing, m_nState);
		return;
	}
	else if(pMousePos.y > GetLocalRect().top)
	{	
		m_pAnimInstance->Stop(m_nDirectionFacing, m_nState);
		m_nDirectionFacing = SOUTH;
		m_pAnimInstance->SetFlip(false);
		m_pAnimInstance->Play(m_nDirectionFacing, m_nState);
		return;
	}
}
