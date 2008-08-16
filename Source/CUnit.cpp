#include "CUnit.h"
#include "CCamera.h"
#include "CSGD_Direct3D.h"
#include "SGD_Math.h"
#include "CTileEngine.h"
#include "ObjectManager.h"
#include "CAISystem.h"
#include "CSGD_TextureManager.h"
#include "CGamePlayState.h"
CUnit::CUnit(int nType)
{
	m_nMaxHP				= 0;
	m_nCurrentHP		= 0;
	m_nAttack			= 0;			
	m_nRange			= 1;			
	m_fAttackSpeed		= 0.f;		
	m_fMovementSpeed	= 0.f;	
	m_nCost				= 0;
	m_fMovementTimer	= 0.f;
	m_nBonus			= 0;			

	m_bIsPlayerUnit		= true;	
	m_bIsGrouped		= false;		
	m_bIsAlive			= true;
	m_bIsSelected		= false;		

	m_pTarget			= NULL;		

	m_nDirectionFacing	= SOUTH_WEST; 
	m_nState			= IDLE;	
	m_fAttackTimer		= 0.f;
	m_nSelectionID = -1;
	SetType(nType);
	SetNextTile(NULL);
	SetDestTile(NULL);
	m_pAnimInstance = new CAnimInstance(GetType());
	m_pAnimInstance->Play(m_nDirectionFacing, m_nState);

	m_pTE = CTileEngine::GetInstance();
	m_pCAI = CAISystem::GetInstance();

}

CUnit::~CUnit(void)
{
	delete m_pAnimInstance;

	//CSGD_TextureManager::GetInstance()->ReleaseTexture(m_nSelectionID);

}

void CUnit::RenderHealth()
{
	POINT ptPos = CCamera::GetInstance()->TransformToScreen((int)GetPosX(), (int)GetPosY());
	// Set Health Rect
	//-------------------------------
	
	float fHealth = ((float)GetHealth() / ((float)GetMaxHP()));
	int nHealthBarLength = (int)Lerp(0.0f, 50.f, fHealth);
	DWORD dwColor;
	if ( fHealth <= .2f )
		dwColor = D3DCOLOR_ARGB(255, 255, 0, 0);
	else if ( fHealth <= .4f && fHealth > .2f )
		dwColor = D3DCOLOR_ARGB(255, 255, 128, 0);
	else if ( fHealth <= .6f && fHealth > .4f)
		dwColor = D3DCOLOR_ARGB(255, 255, 255, 0);
	else if ( fHealth <= .8f && fHealth > .6f )
		dwColor = D3DCOLOR_ARGB(255, 0, 240, 0);
	else
		dwColor = D3DCOLOR_ARGB(255, 0, 190, 0);

	RECT rHealthBar;
	CSGD_Direct3D::GetInstance()->DeviceEnd();
	CSGD_Direct3D::GetInstance()->SpriteEnd();
	rHealthBar.left =	m_rLocalRect.left + (int)((m_rLocalRect.right - m_rLocalRect.left)*.5f) - 25;
	rHealthBar.bottom = m_rLocalRect.top-10;
	//rHealthBar.right = rHealthBar.left + 50;
	rHealthBar.top	= rHealthBar.bottom - 3;
	//CSGD_Direct3D::GetInstance()->DrawRect(rHealthBar, 255, 0, 0);
	rHealthBar.right = rHealthBar.left + nHealthBarLength;
	//CSGD_Direct3D::GetInstance()->DrawRect(rHealthBar, 0, 255, 0);
	CSGD_Direct3D::GetInstance()->DrawRect(rHealthBar, dwColor);
	CSGD_Direct3D::GetInstance()->DeviceBegin();
	CSGD_Direct3D::GetInstance()->SpriteBegin();
	if(IsPlayerUnit() && IsSelected() && m_pTarget)
	{
		m_pTarget->RenderHealth();
	}

	
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
	/*SetPosX(ptPos.x);
	SetPosY(ptPos.y);*/

	m_pAnimInstance->SetX(ptPos.x);
	m_pAnimInstance->SetY(ptPos.y);
	// Set Local Rect
	//-------------------------------
	m_rLocalRect = m_pAnimInstance->GetRenderRect(m_nDirectionFacing, m_nState);
	//----------------------------------------
	if(!m_bIsAlive)
		return;
	

	// If dead, start death animation and set dead
	if(GetHealth() <= 0 && m_bIsAlive == true)
	{
		m_bIsAlive = false;
		m_bIsSelected = false;

		m_pAnimInstance->Stop(m_nDirectionFacing, m_nState);
		m_nState = DYING;
		m_pAnimInstance->Play(m_nDirectionFacing, m_nState);
		m_pAnimInstance->SetLooping(false);
		m_pAnimInstance->SetPlayer(IsPlayerUnit());
		return;
	}
	// If we aren't moving then make sure we are on the anchor point!
	if(GetState() != MOVEMENT)
	{
		SetPosX((float)GetCurrentTile()->ptLocalAnchor.x);
		SetPosY((float)GetCurrentTile()->ptLocalAnchor.y);
	}
	

	// Don't occupy an occupied tile!!
	if(GetState() == IDLE && GetCurrentTile()->pUnit != this)
	{
		SetDestTile(PlaceOnSurrounding(GetCurrentTile()));
		SetPath(CAISystem::GetInstance()->FindPath(this->GetCurrentTile(), GetDestTile()));
		SetState(MOVEMENT);
		
	}
	// Make 6 tiles around unit visible
	UpdateVisibility();

	// AI
	//---------------------------------------------------------------------------
	if(IsPlayerUnit() && GetState() == IDLE && !m_pTarget)
	{
		// Find a target if there is one visible and the PLAYER unit is IDLE
		ScanForEnemies();
	}
	else if(!IsPlayerUnit() && !m_pTarget)
	{
		// If CPU unit, always scan for enemies if we dont have a target
		ScanForEnemies();
	}
	// If our target is dead and we didn't know it, set it to null and relax
	if(m_pTarget && m_pTarget->GetHealth() <= 0)
	{
		m_pTarget = NULL;
		SetState(IDLE);
	}
	// Do we have a target? if so, is it in range? than attack
	else if(m_pTarget->IsAlive() && IsTargetInRange())
	{
		if(GetState() != COMBAT)
		{
			SetState(COMBAT);
			ChangeDirection(m_pTarget->GetCurrentTile());
		}
		m_fAttackTimer += fElapsedTime;
		ResolveCombat();
	}
	// Do we have a target that is out of range? Then move into range
	else if(m_pTarget->IsAlive() && !IsTargetInRange())
	{
		// Move towards the target and update path each time,
		// If we are in range then movement will stop and we'll attack
		SetState(MOVEMENT);
		SetDestTile(m_pTarget->GetCurrentTile());
		SetPath(CAISystem::GetInstance()->FindPath(GetCurrentTile(), GetDestTile()));
	}
	

	// Move to our destination
#pragma region Move
	if (GetState() == MOVEMENT && GetDestTile())
	{
		m_fMovementTimer += fElapsedTime;
		// We are at the destination tile or are we in range of our target? then stop
		if(m_vPath.size() == 0 || (GetDestTile() == GetCurrentTile()) || (m_pTarget && IsTargetInRange()))
		{
			m_vPath.clear();
			SetNextTile(NULL);
			SetDestTile(NULL);
			SetState(IDLE);
			return;
		}
		// Is are we at our next tile and is it the dest and is it occupied? then just stop
		else if(GetDestTile() == GetNextTile() && GetDestTile()->bIsOccupied)
		{
			m_vPath.clear();
			SetNextTile(NULL);
			SetDestTile(NULL);
			SetState(IDLE);
			return;
		}
		// 
		if(!GetNextTile() && m_vPath.size())
		{
			POINT& path = m_vPath.back();
			SetNextTile( m_pTE->GetTile(0, path.x, path.y )  );
			ChangeDirection(GetNextTile());
			m_vPath.pop_back();
			if(GetNextTile()->bIsOccupied && GetNextTile()->pUnit != this)
			{
				SetNextTile(NULL);
				SetPath(CAISystem::GetInstance()->FindPath(this->GetCurrentTile(), GetDestTile()));
				return;
			}
		}
		// Why dont we have a next tile? Lets find a path then!
		if(!GetNextTile())
		{
			SetPath(CAISystem::GetInstance()->FindPath(this->GetCurrentTile(), GetDestTile()));
			return;
		}
		// We reached the anchor of the next tile
		if(MoveUnit())
		{
			// Set prev. to unoccupied
			GetCurrentTile()->bIsOccupied = false;	
			GetCurrentTile()->pUnit = NULL;	
			SetCurrentTile(GetNextTile());
			// Start out the next one
			GetCurrentTile()->bIsOccupied = true;	
			GetCurrentTile()->pUnit = this;	
			// If we have a path, set the next tile
			if(m_vPath.size())
			{
				POINT& path = m_vPath.back();
				SetNextTile( m_pTE->GetTile(0, path.x, path.y )  );
				m_vPath.pop_back();
				if(GetNextTile()->bIsOccupied && GetNextTile()->pUnit != this)
					CAISystem::GetInstance()->FindPath(this->GetCurrentTile(), GetDestTile());
			}
			// Otherwise stop
			else
			{
				SetNextTile(NULL);
				SetDestTile(NULL);
				SetState(IDLE);
			}
			// If we have a next tile, change direction towards that one
			if(GetNextTile())
				ChangeDirection(GetNextTile());
			else
				return;
		}
	} 
#pragma endregion
	//---------------------------------------------------------------------------
	
}

void CUnit::RenderSelection()
{
	int nPosX = m_rLocalRect.left+ (int)((m_rLocalRect.right - m_rLocalRect.left)*.5f) - 32;
	int nPosY = m_rLocalRect.top+ (int)((m_rLocalRect.bottom - m_rLocalRect.top)*.5f);

	CSGD_TextureManager::GetInstance()->Draw(CGamePlayState::GetInstance()->GetSelectionID(), nPosX, nPosY);
	if(m_pTarget)
	{
		int nPosX = m_pTarget->GetLocalRect().left+ (int)((m_pTarget->GetLocalRect().right - m_pTarget->GetLocalRect().left)*.5f) - 32;
		int nPosY = m_pTarget->GetLocalRect().top+ (int)((m_pTarget->GetLocalRect().bottom - m_pTarget->GetLocalRect().top)*.5f);

		CSGD_TextureManager::GetInstance()->Draw(CGamePlayState::GetInstance()->GetSelectionID(), nPosX, nPosY, 1.f, 1.f, 0, 0, 0, 0, D3DCOLOR_ARGB(255, 255, 0, 0));
	}
	
}
void CUnit::Render(float fElapsedTime)
{
	if(m_bIsAlive)
	{
		if(m_bIsSelected)
		{
			RenderSelection();

			RenderHealth();
		}
		
	}
	if(CCamera::GetInstance()->IsOnScreen(GetGlobalRect()))
	{
		m_pAnimInstance->Render();
	}
}

void CUnit::ScanForEnemies()
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
	ptTopLeft.x = ptTileID.x-VISIBILITY;
	ptTopLeft.y = ptTileID.y-VISIBILITY;
	for (int x = ptTopLeft.x; x <= ptTopLeft.x + VISIBILITY*2; x++)
	{
		for (int y = ptTopLeft.y; y <= ptTopLeft.y + VISIBILITY*2; y++)
		{
			// Don't go out of bounds
			if(x >= Map->GetMapWidth() || x < 0)
				continue;
			// Don't go out of bounds
			if(y >= Map->GetMapHeight() || y < 0)
				continue;
			//Map->GetTile(0, x, y)->vColor = D3DCOLOR_ARGB(255, 0, 0, 255);
			
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
				
				// If we dont have a target, set one
				if(this->GetTarget() == NULL)
				{
					// If the unit is IDLE, and finds an enemy, set the target
					SetTarget(pUnit);
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
	// Start at top left corner and check all surrounding
	ptTopLeft.x = ptTileID.x-VISIBILITY;
	ptTopLeft.y = ptTileID.y-VISIBILITY;
	for (int x = ptTopLeft.x; x < ptTopLeft.x + VISIBILITY*2; x++)
	{
		for (int y = ptTopLeft.y; y < ptTopLeft.y + VISIBILITY*2; y++)
		{
			// Don't go out of bounds
			if(x >= Map->GetMapWidth() || x < 0)
				continue;
			// Don't go out of bounds
			if(y >= Map->GetMapHeight() || y < 0)
				continue;
			Map->SetVisible(x, y, true, NULL);
			//Map->GetTile(0, x, y)->vColor = D3DCOLOR_ARGB(255, 255, 0, 0);
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
	if(m_pTarget->GetHealth() <= 0)
	{
		m_pTarget = NULL;
		SetState(IDLE);
		return;
	}
	ChangeDirection(m_pTarget->GetCurrentTile());
	if(m_fAttackTimer >= m_fAttackSpeed)
	{
		m_pTarget->SetCurrentHP(m_pTarget->GetHealth() - GetAttackPower()+m_nBonus);
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

CTile* CUnit::PlaceOnSurrounding(CTile* pCenterTile)
{
	CTileEngine* Map = CTileEngine::GetInstance();

	POINT ptTileID = pCenterTile->ptTileLoc;
	// Start at top left corner and check all surrounding
	CTile* pCheckTile = NULL;
	int nLayer = 1;
	//-1, -1
	//-1, 0
	//-1, 1
	//------
	// 0, -1
	// 0, 0
	// 0, 1
	//------
	// 1, -1
	// 1, 0
	// 1, 1
	while(true)
	{
		for (int x = ptTileID.x-nLayer; x < ptTileID.x + nLayer; x++)
		{
			for (int y = ptTileID.y-nLayer; y < ptTileID.y + nLayer; y++)
			{
				// Don't go out of bounds
				if(x >= Map->GetMapWidth() || x < 0)
					continue;
				// Don't go out of bounds
				if(y >= Map->GetMapHeight() || y < 0)
					continue;
				if((x >= Map->GetMapWidth() || x < 0) && (y >= Map->GetMapHeight() || y < 0))
					return NULL;
				if(!Map->GetTile(0, x, y)->bIsOccupied)
					return Map->GetTile(0, x, y);
			}
		}
		nLayer++;
	}
}

bool CUnit::MoveUnit()
{
	POINT ptStart = GetCurrentTile()->ptLocalAnchor;
	POINT ptEnd	  = GetNextTile()->ptLocalAnchor;
	float fPercent =  m_fMovementTimer/(1/m_fMovementSpeed) ;
	if(fPercent >=1)
	{
		SetPosX((float)ptEnd.x);
		SetPosY((float)ptEnd.y);
		m_fMovementTimer = 0.f;
		return true;
	}
	int xPos = (int)Lerp((float)ptStart.x, (float)ptEnd.x, fPercent);
	int yPos = (int)Lerp((float)ptStart.y, (float)ptEnd.y, fPercent);
	SetPosX((float)xPos);
	SetPosY((float)yPos);
	return false;
}

bool CUnit::IsTargetInRange()
{

	if((m_pTarget->GetCurrentTile()->ptTileLoc.x <= GetCurrentTile()->ptTileLoc.x + m_nRange 
		&& m_pTarget->GetCurrentTile()->ptTileLoc.x >= GetCurrentTile()->ptTileLoc.x - m_nRange)
	&& (m_pTarget->GetCurrentTile()->ptTileLoc.y <= GetCurrentTile()->ptTileLoc.y + m_nRange 
		&& m_pTarget->GetCurrentTile()->ptTileLoc.y >= GetCurrentTile()->ptTileLoc.y - m_nRange))
		return true;

	return false;
}
