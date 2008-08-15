#include "CUnit.h"
#include "CCamera.h"
#include "CSGD_Direct3D.h"
#include "SGD_Math.h"
#include "CTileEngine.h"
#include "ObjectManager.h"
#include "CAISystem.h"
CUnit::CUnit(int nType)
{
	m_nHP				= 100;			
	m_nAttack			= 0;			
	m_nRange			= 1;			
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
	m_fAttackTimer		= 0.f;

	SetType(nType);
	SetNextTile(NULL);
	SetDestTile(NULL);
	m_pHealthBar = new CHealthBar();
	m_pHealthBar->SetHealth(m_nHP);
	m_pAnimInstance = new CAnimInstance(GetType());
	m_pAnimInstance->Play(m_nDirectionFacing, m_nState);

	m_pTE = CTileEngine::GetInstance();
	m_pCAI = CAISystem::GetInstance();

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
	m_rLocalRect = m_pAnimInstance->GetRenderRect(m_nDirectionFacing, m_nState);
	//POINT ptTL = CCamera::GetInstance()->TransformToScreen(m_rLocalRect.left, m_rLocalRect.top);
	//POINT ptBR = CCamera::GetInstance()->TransformToScreen(m_rLocalRect.right, m_rLocalRect.bottom);
	//m_rLocalRect.left = ptTL.x;
	//m_rLocalRect.top = ptTL.y;
	//m_rLocalRect.right = ptBR.x;
	//m_rLocalRect.bottom = ptBR.y;
	/*m_rLocalRect.left   = ptPos.x + m_pAnimInstance->GetOffsetX(m_nDirectionFacing, m_nState);
	m_rLocalRect.top    = ptPos.y - m_pAnimInstance->GetOffsetY(m_nDirectionFacing, m_nState);
	m_rLocalRect.right  = m_rLocalRect.left + m_pAnimInstance->GetFrameWidth(m_nDirectionFacing, m_nState);
	m_rLocalRect.bottom = m_rLocalRect.top + m_pAnimInstance->GetFrameHeight(m_nDirectionFacing, m_nState);*/
	if(!m_bIsAlive)
		return;
	
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
	if(m_pHealthBar->GetHealth() <= 0 && m_bIsAlive == true)
	{
		m_bIsAlive = false;
		m_bIsSelected = false;
		CTileEngine* Map = CTileEngine::GetInstance();
		POINT nMapPoint = Map->IsoMouse(GetCurrentTile()->ptPos.x, GetCurrentTile()->ptPos.y, 0);	

		m_pAnimInstance->Stop(m_nDirectionFacing, m_nState);
		m_nState = DYING;
		m_pAnimInstance->Play(m_nDirectionFacing, m_nState);
		m_pAnimInstance->SetLooping(false);
		m_pAnimInstance->SetPlayer(IsPlayerUnit());
	}

	// Make 6 tiles around unit visible
	UpdateVisibility();
	

	if(GetState() == COMBAT)
	{
		m_fAttackTimer += fElapsedTime;
		ResolveCombat();
	}
	//-------------------------------
	// AI movement
	// if current position is == to dest position set current to to dest, dest to current and next to current
	else if (GetState() == MOVEMENT && GetDestTile())
	{
		// We are at the destination tile
		if(GetDestTile() == GetCurrentTile())
		{
			m_vPath.clear();
			SetNextTile(NULL);
			SetDestTile(NULL);
			SetState(IDLE);
			return;
		}
		else if(GetDestTile() == GetNextTile() && GetDestTile()->bIsOccupied)
		{
			m_vPath.clear();
			SetNextTile(NULL);
			SetDestTile(NULL);
			SetState(IDLE);
			return;
		}
		if(!GetNextTile() && m_vPath.size())
		{
			POINT& path = m_vPath.back();
			SetNextTile( m_pTE->GetTile(0, path.x, path.y )  );
			m_vPath.pop_back();
			if(GetNextTile()->bIsOccupied && GetNextTile()->pUnit != this)
			{
				SetPath(CAISystem::GetInstance()->FindPath(this->GetCurrentTile(), GetDestTile()));
				return;
			}
		}
		else if(!GetNextTile())
		{
			SetPath(CAISystem::GetInstance()->FindPath(this->GetCurrentTile(), GetDestTile()));
			return;
		}
		
		else
		{
			// We reached the anchor of the next tile
			if(GetPosX() == GetNextTile()->ptLocalAnchor.x && GetPosY() == GetNextTile()->ptLocalAnchor.y)
			{
				GetCurrentTile()->bIsOccupied = false;	
				GetCurrentTile()->pUnit = NULL;	
				SetCurrentTile(GetNextTile());
				GetCurrentTile()->bIsOccupied = true;	
				GetCurrentTile()->pUnit = this;	
				if(m_vPath.size())
				{
					POINT& path = m_vPath.back();
					SetNextTile( m_pTE->GetTile(0, path.x, path.y )  );
					m_vPath.pop_back();
					if(GetNextTile()->bIsOccupied && GetNextTile()->pUnit != this)
						CAISystem::GetInstance()->FindPath(this->GetCurrentTile(), GetDestTile());
				}
				else
				{
					SetNextTile(NULL);
					SetDestTile(NULL);
					SetState(IDLE);
				}
				
				if(GetNextTile())
					ChangeDirection(GetNextTile());
				else
					return;
			}
			
			if ( GetPosX() >  GetNextTile()->ptLocalAnchor.x)
				SetPosX(GetPosX() - GetVelX() );

			else if ( GetPosX() < GetNextTile()->ptLocalAnchor.x)
				SetPosX( GetPosX() +  GetVelX() );

			if ( GetPosY() >  GetNextTile()->ptLocalAnchor.y)
				SetPosY( GetPosY() - GetVelY() );

			else if ( GetPosY() <  GetNextTile()->ptLocalAnchor.y)
				SetPosY( GetPosY() +  GetVelY() );
			
		}
		
			
		//// if you're at dest
		//if ( GetPosX() == GetDestTile()->ptLocalAnchor.x  && GetPosY() == GetDestTile()->ptLocalAnchor.y  )
		//{
		//	m_pTE->SetOccupy(GetCurrentTile()->ptTileLoc.x, GetCurrentTile()->ptTileLoc.y, false , NULL); 
		//	SetCurrentTile( GetDestTile() );
		//	SetDestTile( GetCurrentTile() );
		//	SetNextTile(GetCurrentTile() );
		//	m_pTE->SetOccupy(GetCurrentTile()->ptTileLoc.x, GetCurrentTile()->ptTileLoc.y, true, this); 
		//	m_pTE->SetVisible(GetCurrentTile()->ptTileLoc.x, GetCurrentTile()->ptTileLoc.y, true, this);
		//	SetState(IDLE);
		//	return;

		//}

		//if (GetNextTile()->ptLocalAnchor.x < 0)
		//{
		//	if (m_vPath.size() > 0)
		//	{
		//		POINT& path = m_vPath.front();
		//		SetNextTile( m_pTE->GetTile(0, path.x, path.y )  ); 
		//		m_vPath.pop_front();
		//	}
		//}

		//// if moving & current position is != dest position & current position != next position then move to next
		//if( GetPosX() != GetNextTile()->ptLocalAnchor.x || GetPosY() != GetNextTile()->ptLocalAnchor.y )
		//{
		//	if ( GetPosX() >  GetNextTile()->ptLocalAnchor.x)
		//		SetPosX(GetPosX() - GetVelX() );

		//	else if ( GetPosX() < GetNextTile()->ptLocalAnchor.x)
		//		SetPosX( GetPosX() +  GetVelX() );

		//	if ( GetPosY() >  GetNextTile()->ptLocalAnchor.y)
		//		SetPosY( GetPosY() - GetVelY() );

		//	else if ( GetPosY() <  GetNextTile()->ptLocalAnchor.y)
		//		SetPosY( GetPosY() +  GetVelY() );
		//}
		//// if your at next your next
		//else if (  GetPosX() == GetNextTile()->ptLocalAnchor.x && GetNextTile()->ptLocalAnchor.y == GetPosY()  )
		//{
		//	if (m_vPath.size() > 0)
		//	{
		//		m_pTE->SetOccupy(GetCurrentTile()->ptTileLoc.x, GetCurrentTile()->ptTileLoc.y, false, NULL); 
		//		SetCurrentTile( GetNextTile());
		//		m_pTE->SetOccupy(GetCurrentTile()->ptTileLoc.x, GetCurrentTile()->ptTileLoc.y, true, this);
		//		m_pTE->SetVisible(GetCurrentTile()->ptTileLoc.x, GetCurrentTile()->ptTileLoc.y, true, this);
		//		POINT& path = m_vPath.front();
		//		SetNextTile( m_pTE->GetTile(0, path.x, path.y )  ); 
		//		m_vPath.pop_front();
		//	}
		//}
		

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

void CUnit::CheckCollisions()
{
	ObjectManager* pOM = ObjectManager::GetInstance();
	// If we are dead or already in combat, do not check
	if(!m_bIsAlive || this->GetState() == COMBAT || GetState() == RETREAT)
		return;
	//if(this->IsPlayerUnit() && this->GetState() != IDLE)
	//	return false;
	
	CTileEngine* Map = CTileEngine::GetInstance();
	//tile positionx+range, tile positiony+range
	//to
	//tile positionx-range, tile positiony-range

	//00-22-44
	POINT ptTileID = GetCurrentTile()->ptTileLoc;
	POINT ptTopLeft;
	// Start at top left corner and check all surrounding
	ptTopLeft.x = ptTileID.x-m_nRange;
	ptTopLeft.y = ptTileID.y-m_nRange;
	for (int x = ptTopLeft.x; x < ptTopLeft.x + m_nRange*2; x++)
	{
		for (int y = ptTopLeft.y; y < ptTopLeft.y + m_nRange*2; y++)
		{
			// Don't go out of bounds
			if(x >= Map->GetMapWidth() || x < 0)
				continue;
			// Don't go out of bounds
			if(y >= Map->GetMapHeight() || y < 0)
				continue;

			if(Map->GetTile(0, x, y)->bIsOccupied)
			{
				CUnit* pUnit = Map->GetTile(0, x, y)->pUnit;
				if(pUnit == NULL)
					break;
				// Ignore allies
				if(pUnit->IsPlayerUnit() == this->IsPlayerUnit())
					continue;
				// Ignore dead and self
				if(!pUnit->IsAlive() || pUnit == this)
					continue;
				
				// If the unit is moving and is in range of his target, then attack
				if(pUnit == this->GetTarget())
				{
					SetState(COMBAT);
					//Resolve combat
					return;
				}
				
				else if(this->GetTarget() == NULL)
				{
					// If the unit is IDLE, and finds an enemy, set the target
					SetTarget(pUnit);
					SetState(COMBAT);
					return;
				}
			}
		}
	}


	/*POINT nMapPoint = Map->IsoMouse(this->GetCurrentTile().ptLocalAnchor.x, this->GetCurrentTile().ptLocalAnchor.y, 0);
	int nDistanceX = 3 * m_nRange + nMapPoint.x;
	int nDistanceY = 3 * m_nRange + nMapPoint.y;
	for(int i = nMapPoint.x - m_nRange; i < nDistanceX; ++i)
	{
		for(int j = nMapPoint.y - m_nRange; j < nDistanceY; ++j)
		{
			if(i >= Map->GetMapWidth() || i < 0)
				continue;
			if(j >= Map->GetMapHeight() || j < 0)
				continue;

			if(Map->GetTile(0,i,j).bIsOccupied)
			{
				if(Map->GetTile(0,i,j).nUnitIndex > -1)
				{
					if (static_cast<CUnit*>(pOM->GetSelectedUnit(Map->GetTile(0,i,j).nUnitIndex))->IsAlive())
					{
						if(m_bIsPlayerUnit == static_cast<CUnit*>(pOM->GetSelectedUnit(Map->GetTile(0,i,j).nUnitIndex))->IsPlayerUnit())
							break;
					}
					if(static_cast<CUnit*>(pOM->GetSelectedUnit(Map->GetTile(0,i,j).nUnitIndex)) != this)
						if(static_cast<CUnit*>(pOM->GetSelectedUnit(Map->GetTile(0,i,j).nUnitIndex))->IsAlive())
						{

							if(m_nState != COMBAT)
							{
								ChangeDirection(Map->GetTile(0,i,j).ptPos);
								m_pAnimInstance->Stop(m_nDirectionFacing, m_nState);
								m_nState = COMBAT;
								m_pAnimInstance->Play(m_nDirectionFacing, m_nState);
								m_pAnimInstance->SetPlayer(IsPlayerUnit());

							}	
							if(static_cast<CUnit*>(pOM->GetSelectedUnit(Map->GetTile(0,i,j).nUnitIndex)) != 0)
							{
								static_cast<CUnit*>(pOM->GetSelectedUnit(Map->GetTile(0,i,j).nUnitIndex))->DamageUnit( GetAttackPower() );
								return true;
							}
						}
				}
			}

		}

	}*/
	
}
void CUnit::UpdateVisibility()
{
	CTileEngine* Map = CTileEngine::GetInstance();

	POINT ptTileID = GetCurrentTile()->ptTileLoc;
	POINT ptTopLeft;
	int m_nVisibility = 6;
	// Start at top left corner and check all surrounding
	ptTopLeft.x = ptTileID.x-m_nVisibility;
	ptTopLeft.y = ptTileID.y-m_nVisibility;
	for (int x = ptTopLeft.x; x < ptTopLeft.x + m_nVisibility*2; x++)
	{
		for (int y = ptTopLeft.y; y < ptTopLeft.y + m_nVisibility*2; y++)
		{
			// Don't go out of bounds
			if(x >= Map->GetMapWidth() || x < 0)
				continue;
			// Don't go out of bounds
			if(y >= Map->GetMapHeight() || y < 0)
				continue;
			Map->SetVisible(x, y, true, NULL);
		}
	}
}

void CUnit::ChangeDirection(CTile* pTileFacing)
{
	if(pTileFacing == GetCurrentTile())
		return;
	if(pTileFacing->ptLocalAnchor.y < GetCurrentTile()->ptLocalAnchor.y && pTileFacing->ptLocalAnchor.x < GetCurrentTile()->ptLocalAnchor.x)
	{
		if(m_nDirectionFacing == NORTH_WEST && !m_pAnimInstance->IsFlipped())
			return;
		m_pAnimInstance->Stop(m_nDirectionFacing, m_nState);
		m_nDirectionFacing = NORTH_WEST;
		m_pAnimInstance->SetFlip(false);
		m_pAnimInstance->Play(m_nDirectionFacing, m_nState);
		m_pAnimInstance->SetPlayer(IsPlayerUnit());
		return;
	}
	else if(pTileFacing->ptLocalAnchor.y < GetCurrentTile()->ptLocalAnchor.y && pTileFacing->ptLocalAnchor.x > GetCurrentTile()->ptLocalAnchor.x)
	{
		if(m_nDirectionFacing == NORTH_WEST && m_pAnimInstance->IsFlipped())
			return;
		m_pAnimInstance->Stop(m_nDirectionFacing, m_nState);
		m_nDirectionFacing = NORTH_WEST;
		m_pAnimInstance->SetFlip(true);
		m_pAnimInstance->Play(m_nDirectionFacing, m_nState);
		m_pAnimInstance->SetPlayer(IsPlayerUnit());

		return;
	}
	else if(pTileFacing->ptLocalAnchor.y > GetCurrentTile()->ptLocalAnchor.y && pTileFacing->ptLocalAnchor.x < GetCurrentTile()->ptLocalAnchor.x)
	{
		if(m_nDirectionFacing == SOUTH_WEST && !m_pAnimInstance->IsFlipped())
			return;
		m_pAnimInstance->Stop(m_nDirectionFacing, m_nState);
		m_nDirectionFacing = SOUTH_WEST;
		m_pAnimInstance->SetFlip(false);
		m_pAnimInstance->Play(m_nDirectionFacing, m_nState);
		m_pAnimInstance->SetPlayer(IsPlayerUnit());

		return;
	}
	else if(pTileFacing->ptLocalAnchor.y > GetCurrentTile()->ptLocalAnchor.y && pTileFacing->ptLocalAnchor.x > GetCurrentTile()->ptLocalAnchor.x)
	{
		if(m_nDirectionFacing == SOUTH_WEST && m_pAnimInstance->IsFlipped())
			return;
		m_pAnimInstance->Stop(m_nDirectionFacing, m_nState);
		m_nDirectionFacing = SOUTH_WEST;
		m_pAnimInstance->SetFlip(true);
		m_pAnimInstance->Play(m_nDirectionFacing, m_nState);
		m_pAnimInstance->SetPlayer(IsPlayerUnit());

		return;
	}
	else if(pTileFacing->ptLocalAnchor.x < GetCurrentTile()->ptLocalAnchor.x)
	{
		if(m_nDirectionFacing == WEST && !m_pAnimInstance->IsFlipped())
			return;
		m_pAnimInstance->Stop(m_nDirectionFacing, m_nState);
		m_nDirectionFacing = WEST;
		m_pAnimInstance->SetFlip(false);
		m_pAnimInstance->Play(m_nDirectionFacing, m_nState);
		m_pAnimInstance->SetPlayer(IsPlayerUnit());

		return;
	}
	else if(pTileFacing->ptLocalAnchor.x > GetCurrentTile()->ptLocalAnchor.x)
	{
		if(m_nDirectionFacing == WEST && m_pAnimInstance->IsFlipped())
			return;
		m_pAnimInstance->Stop(m_nDirectionFacing, m_nState);
		m_nDirectionFacing = WEST;
		m_pAnimInstance->SetFlip(true);
		m_pAnimInstance->Play(m_nDirectionFacing, m_nState);
		m_pAnimInstance->SetPlayer(IsPlayerUnit());

		return;
	}
	else if(pTileFacing->ptLocalAnchor.y < GetCurrentTile()->ptLocalAnchor.y)
	{
		if(m_nDirectionFacing == NORTH && !m_pAnimInstance->IsFlipped())
			return;
		m_pAnimInstance->Stop(m_nDirectionFacing, m_nState);
		m_nDirectionFacing = NORTH;
		m_pAnimInstance->SetFlip(false);
		m_pAnimInstance->Play(m_nDirectionFacing, m_nState);
		m_pAnimInstance->SetPlayer(IsPlayerUnit());

		return;
	}
	else if(pTileFacing->ptLocalAnchor.y > GetLocalRect().top)
	{	
		if(m_nDirectionFacing == SOUTH && !m_pAnimInstance->IsFlipped())
			return;
		m_pAnimInstance->Stop(m_nDirectionFacing, m_nState);
		m_nDirectionFacing = SOUTH;
		m_pAnimInstance->SetFlip(false);
		m_pAnimInstance->Play(m_nDirectionFacing, m_nState);
		m_pAnimInstance->SetPlayer(IsPlayerUnit());

		return;
	}
}

void CUnit::ResolveCombat()
{
	if(!m_pTarget)
		return;
	if(!m_pTarget->IsAlive())
	{
		m_pTarget = NULL;
		SetState(IDLE);
		return;
	}
	ChangeDirection(m_pTarget->GetCurrentTile());
	if(m_fAttackTimer >= m_fAttackSpeed)
	{
		m_pTarget->DamageUnit(GetAttackPower()+m_nBonus);
		m_fAttackTimer = 0.f;
	}
}

void CUnit::CalcAttackBonus()
{
	if(!m_pTarget)
		return;
	m_nBonus = 0;
	switch (GetType())
	{
	case UNIT_INFANTRY:
		// Unit Type bonuses
		if(m_pTarget->GetType() == UNIT_AXMEN)
			m_nBonus = 1;
		break;
	case UNIT_CAVALRY:
		if(m_pTarget->GetType() == UNIT_ARCHER || m_pTarget->GetType() == UNIT_INFANTRY)
			m_nBonus = 1;
		break;
	case UNIT_CAVALRY_ARCHER:
		if(m_pTarget->GetType() == UNIT_ARCHER || m_pTarget->GetType() == UNIT_CAVALRY_ARCHER)
			m_nBonus = 1;
		break;
	case UNIT_AXMEN:
		if(m_pTarget->GetType() == UNIT_CAVALRY)
			m_nBonus = 1;
		break;
	case UNIT_ARCHER:
		if(m_pTarget->GetType() == UNIT_WAR_ELEPHANT)
			m_nBonus = 3;
		break;
	case UNIT_WAR_ELEPHANT:
		if(m_pTarget->GetType() == UNIT_CAVALRY || m_pTarget->GetType() == UNIT_INFANTRY || m_pTarget->GetType() == UNIT_AXMEN)
			m_nBonus = 1;
		break;
	default:
		break;
	}
	// Mountain bonus	
	if(GetCurrentTile()->nType == MOUNTAIN)
		m_nBonus++;
	// Forest enemy defense bonus
	if(GetTarget()->GetCurrentTile()->nType == FOREST)
		m_nBonus--;
}
