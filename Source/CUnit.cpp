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
	m_fHealTimer		= 0.f;
	m_bIsPlayerUnit		= true;	
	m_bIsGrouped		= false;		
	m_bIsAlive			= true;
	m_bIsSelected		= false;	
	m_fDeathTimer		= 0.f;
	m_fScanTimer		= 0.f;
	m_bIsActive			= true;

	m_pTarget			= NULL;		

	m_nDirectionFacing	= SOUTH_WEST; 
	m_nState			= IDLE;	
	m_fAttackTimer		= 0.f;
	SetType(nType);
	SetNextTile(NULL);
	SetDestTile(NULL);
	SetCurrentTile(NULL);
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
	if(m_bIsActive)
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
		//----------------------------------------
	}
	if(!m_bIsAlive)
	{
		m_fDeathTimer += fElapsedTime;
		if(m_fDeathTimer >= 7.f)
			m_bIsActive = false;
		return;
	}



	// Make 6 tiles around unit visible
	if(m_bIsPlayerUnit)
		UpdateVisibility();
	//----------------------------------

	if(m_nCurrentHP <= 0 && m_bIsAlive == true)
	{
		m_bIsAlive = false;
		m_bIsSelected = false;
		
		//Start death animation and fading
		m_pAnimInstance->Stop(m_nDirectionFacing, m_nState);
		m_nState = DYING;
		m_pAnimInstance->Play(m_nDirectionFacing, m_nState);
		m_pAnimInstance->SetLooping(false);
		m_pAnimInstance->SetPlayer(IsPlayerUnit());
		m_pAnimInstance->StartFadeTimer(m_nDirectionFacing, m_nState);
		
		// Clear the tile
		m_pCurrentTile->bIsOccupied = false;
		m_pCurrentTile->pUnit = NULL;
	
	}
	else if(m_nState != RETREAT && m_nCurrentHP <= ( m_nMaxHP * .25) && m_bIsAlive == true 
		&& !IsPlayerUnit() && ( !m_pCurrentTile->bIsEnemySpawn && !m_pCurrentTile->bIsPlayerSpawn ))
	{
		m_nState = RETREAT;
		m_pTarget = NULL;
		SetDestTile(NULL);
		SetNextTile(NULL);
		ObjectManager::GetInstance()->GetSpawnPointDest(this);
		SetPath(CAISystem::GetInstance()->FindPath(m_pCurrentTile, m_pDestinationTile));
		ChangeDirection(m_pCurrentTile);
	}
	// If the target is dead dont mind him
	if(m_pTarget && m_pTarget->GetHealth() <= 0)
	{
		m_pTarget = NULL;
		SetState(IDLE);
		// Change animation to idle
		ChangeDirection(m_pCurrentTile);
	}
	

	switch (m_nState)
	{
	case IDLE:
		m_fScanTimer += fElapsedTime;
		m_fHealTimer += fElapsedTime;
		if (m_fHealTimer > 2.f  && m_nCurrentHP < m_nMaxHP)
		{
			m_nCurrentHP += 1;
			m_fHealTimer = 0.f;
		}

		// If we aren't moving then make sure we are on the anchor point!
		SetPosX((float)m_pCurrentTile->ptLocalAnchor.x);
		SetPosY((float)m_pCurrentTile->ptLocalAnchor.y);
		//-----------------------------------------------------
		
		if((((int)m_fScanTimer) < 1.) && !m_pTarget && m_nCurrentHP > ( m_nMaxHP * .25))
		{
			ScanForEnemies();
			
		}
		else if(m_fScanTimer > 1.f)
			m_fScanTimer = 0.f;
		if(m_pTarget)
		{
			if(IsTargetInRange())
			{
				SetState(COMBAT);
				// Face the target
				ChangeDirection(m_pTarget->m_pCurrentTile);
			}
			else
			{
				SetState(MOVEMENT);
				SetDestTile(m_pTarget->m_pCurrentTile);
				SetPath(CAISystem::GetInstance()->FindPath(m_pCurrentTile, m_pDestinationTile));
				if(m_vPath.size())
				{
					// Found a path, set next tile
					POINT& path = m_vPath.back();
					SetNextTile( m_pTE->GetTile(0, path.x, path.y )  );
					ChangeDirection(m_pNextTile);
					m_vPath.pop_back();
				}
				else
				{
					// No path available, go back to IDLE
					SetState(IDLE);
					ChangeDirection(m_pCurrentTile);
				}
			}
		}

		break;
	case COMBAT:
		// If we aren't moving then make sure we are on the anchor point!
		SetPosX((float)m_pCurrentTile->ptLocalAnchor.x);
		SetPosY((float)m_pCurrentTile->ptLocalAnchor.y);
		//-----------------------------------------------------
		m_fAttackTimer += fElapsedTime;
		if(m_pTarget)
		{
			if(IsTargetInRange())
			{
				ResolveCombat();
			}
			else
			{
				// Go back to idle, which will reset path and move to target
				SetState(IDLE);
				ChangeDirection(m_pCurrentTile);
			}
		}
		else
		{
			// No target, Go back to IDLE to chill
			SetState(IDLE);
			ChangeDirection(m_pCurrentTile);
		}
		break;
	case MOVEMENT:
		// No destination, then go back to IDLE
		if(!m_pDestinationTile)
		{
			SetState(IDLE);
			SetNextTile(NULL);
			ChangeDirection(m_pCurrentTile);
		}
		// Reached destination or in range of target
		if((m_pCurrentTile == m_pDestinationTile) || (m_pTarget && IsTargetInRange()))
		{
			m_vPath.clear();
			SetDestTile(NULL);
			SetNextTile(NULL);
			SetState(IDLE);
			ChangeDirection(m_pCurrentTile);
			break;
		}
		// If we have a destination but not a next, set it
		if(m_pDestinationTile && !m_pNextTile)
		{	
			if(m_vPath.size())
			{
				POINT& path = m_vPath.back();
				SetNextTile( m_pTE->GetTile(0, path.x, path.y )  );
				ChangeDirection(m_pNextTile);
				m_vPath.pop_back();
				// If the next tile isn't right next to us, we cant move there
				if(!IsTileAdjacent(m_pCurrentTile, m_pNextTile))
				{
					m_vPath.clear();
					SetNextTile(NULL);
					break;
				}
			}
			// No path to dest? find one
			else
			{
				SetPath(CAISystem::GetInstance()->FindPath(m_pCurrentTile, m_pDestinationTile));
				break;
			}
		}
		
		// Next one is occupied, stop and find a new path next run
		if(m_pNextTile->bIsOccupied)
		{
			// If that is the dest, just stop
			if(m_pNextTile == m_pDestinationTile)
			{
				SetDestTile(NULL);
				SetState(IDLE);
				ChangeDirection(m_pCurrentTile);
			}
			// Clear the old path and next tile
			m_vPath.clear();
			SetNextTile(NULL);
			break;
		}
		// Returns true if we make it to the next tile
		if(MoveUnit(fElapsedTime))
		{
			// Set prev. to unoccupied
			m_pCurrentTile->bIsOccupied = false;	
			m_pCurrentTile->pUnit = NULL;	
			SetCurrentTile(m_pNextTile);
			// Start out the next one
			m_pCurrentTile->bIsOccupied = true;	
			m_pCurrentTile->pUnit = this;	
			// Clear next tile, will be reset next run if path exists
			SetNextTile(NULL);
			
			if(m_pTarget && !IsTargetInView())
			{
				m_pTarget = NULL;
				m_nState = IDLE;
				ChangeDirection(m_pCurrentTile);
			}
			break;
		}
		break;
	case RETREAT:
		// No destination, then go back to IDLE
		if(!m_pDestinationTile)
		{
			SetState(IDLE);
			SetNextTile(NULL);
			ChangeDirection(m_pCurrentTile);
		}
		// Reached destination or in range of target
		if((m_pCurrentTile == m_pDestinationTile) || (!IsPlayerUnit() && m_pCurrentTile->bIsEnemySpawn) || (IsPlayerUnit() && m_pCurrentTile->bIsPlayerSpawn))
		{
			m_vPath.clear();
			SetDestTile(NULL);
			SetNextTile(NULL);
			SetState(IDLE);
			ChangeDirection(m_pCurrentTile);
			break;
		}
		// If we have a destination but not a next, set it
		if(m_pDestinationTile && !m_pNextTile)
		{	
			if(m_vPath.size())
			{
				POINT& path = m_vPath.back();
				SetNextTile( m_pTE->GetTile(0, path.x, path.y )  );
				ChangeDirection(m_pNextTile);
				m_vPath.pop_back();
				// If the next tile isn't right next to us, we cant move there
				if(!IsTileAdjacent(m_pCurrentTile, m_pNextTile))
				{
					m_vPath.clear();
					SetNextTile(NULL);
					break;
				}
			}
			// No path to dest? find one
			else
			{
				SetPath(CAISystem::GetInstance()->FindPath(m_pCurrentTile, m_pDestinationTile));
				break;
			}
		}
		
		// Next one is occupied, stop and find a new path next run
		if(m_pNextTile->bIsOccupied)
		{
			// If that is the dest, just stop
			if(m_pNextTile == m_pDestinationTile)
			{
				SetDestTile(NULL);
				SetState(IDLE);
				ChangeDirection(m_pCurrentTile);
			}
			// Clear the old path and next tile
			m_vPath.clear();
			SetNextTile(NULL);
			break;
		}
		// Returns true if we make it to the next tile
		if(MoveUnit(fElapsedTime))
		{
			// Set prev. to unoccupied
			m_pCurrentTile->bIsOccupied = false;	
			m_pCurrentTile->pUnit = NULL;	
			SetCurrentTile(m_pNextTile);
			// Start out the next one
			m_pCurrentTile->bIsOccupied = true;	
			m_pCurrentTile->pUnit = this;	
			// Clear next tile, will be reset next run if path exists
			SetNextTile(NULL);
			break;
		}
		break;
	case DYING:

		break;
	}
	
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
	if(!m_bIsActive)
		return;

	if(IsOnScreen() && m_pCurrentTile->bIsVisible)
	{
		if(m_bIsAlive)
		{
			if(m_bIsSelected)
			{
				RenderSelection();

				RenderHealth();
			}
			
		}
	
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
	POINT ptTileID = m_pCurrentTile->ptTileLoc;

	// Start at top left corner and check all surrounding
	int nLayer = 1;
	while(nLayer <= VISIBILITY)
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
				/*	if((x >= Map->GetMapWidth() || x < 0) && (y >= Map->GetMapHeight() || y < 0))
						return NULL;*/
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
		
					if(this->GetTarget() == NULL)
					{
						// If unit finds an enemy, set the target
						SetTarget(pUnit);
						return;
					}
				}

			}
		}
		nLayer++;
	}
}
void CUnit::UpdateVisibility()
{
	CTileEngine* Map = CTileEngine::GetInstance();

	POINT ptTileID = m_pCurrentTile->ptTileLoc;
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
	if(pTileFacing == m_pCurrentTile || pTileFacing == NULL)
	{
		m_pAnimInstance->StopAllAnimations();
		return;
	}
	if(pTileFacing->ptLocalAnchor.y < m_pCurrentTile->ptLocalAnchor.y && pTileFacing->ptLocalAnchor.x < m_pCurrentTile->ptLocalAnchor.x)
	{
		// If this already is our animation, and it's playing just return
		if(m_nDirectionFacing == NORTH_WEST && !m_pAnimInstance->IsFlipped() && m_pAnimInstance->IsPlaying(m_nDirectionFacing, GetState()))
			return;
		
		m_pAnimInstance->StopAllAnimations();
		m_nDirectionFacing = NORTH_WEST;
		m_pAnimInstance->SetFlip(false);
		m_pAnimInstance->Play(m_nDirectionFacing, m_nState);
		m_pAnimInstance->SetPlayer(IsPlayerUnit());
		return;
	}
	else if(pTileFacing->ptLocalAnchor.y < m_pCurrentTile->ptLocalAnchor.y && pTileFacing->ptLocalAnchor.x > m_pCurrentTile->ptLocalAnchor.x)
	{
		if(m_nDirectionFacing == NORTH_WEST && m_pAnimInstance->IsFlipped() && m_pAnimInstance->IsPlaying(m_nDirectionFacing, GetState()))
			return;
		m_pAnimInstance->StopAllAnimations();
		m_nDirectionFacing = NORTH_WEST;
		m_pAnimInstance->SetFlip(true);
		m_pAnimInstance->Play(m_nDirectionFacing, m_nState);
		m_pAnimInstance->SetPlayer(IsPlayerUnit());

		return;
	}
	else if(pTileFacing->ptLocalAnchor.y > m_pCurrentTile->ptLocalAnchor.y && pTileFacing->ptLocalAnchor.x < m_pCurrentTile->ptLocalAnchor.x)
	{
		if(m_nDirectionFacing == SOUTH_WEST && !m_pAnimInstance->IsFlipped() && m_pAnimInstance->IsPlaying(m_nDirectionFacing, GetState()))
			return;
		m_pAnimInstance->StopAllAnimations();
		m_nDirectionFacing = SOUTH_WEST;
		m_pAnimInstance->SetFlip(false);
		m_pAnimInstance->Play(m_nDirectionFacing, m_nState);
		m_pAnimInstance->SetPlayer(IsPlayerUnit());

		return;
	}
	else if(pTileFacing->ptLocalAnchor.y > m_pCurrentTile->ptLocalAnchor.y && pTileFacing->ptLocalAnchor.x > m_pCurrentTile->ptLocalAnchor.x)
	{
		if(m_nDirectionFacing == SOUTH_WEST && m_pAnimInstance->IsFlipped() && m_pAnimInstance->IsPlaying(m_nDirectionFacing, GetState()))
			return;
		m_pAnimInstance->StopAllAnimations();
		m_nDirectionFacing = SOUTH_WEST;
		m_pAnimInstance->SetFlip(true);
		m_pAnimInstance->Play(m_nDirectionFacing, m_nState);
		m_pAnimInstance->SetPlayer(IsPlayerUnit());

		return;
	}
	else if(pTileFacing->ptLocalAnchor.x < m_pCurrentTile->ptLocalAnchor.x)
	{
		if(m_nDirectionFacing == WEST && !m_pAnimInstance->IsFlipped() && m_pAnimInstance->IsPlaying(m_nDirectionFacing, GetState()))
			return;
		m_pAnimInstance->StopAllAnimations();
		m_nDirectionFacing = WEST;
		m_pAnimInstance->SetFlip(false);
		m_pAnimInstance->Play(m_nDirectionFacing, m_nState);
		m_pAnimInstance->SetPlayer(IsPlayerUnit());

		return;
	}
	else if(pTileFacing->ptLocalAnchor.x > m_pCurrentTile->ptLocalAnchor.x)
	{
		if(m_nDirectionFacing == WEST && m_pAnimInstance->IsFlipped() && m_pAnimInstance->IsPlaying(m_nDirectionFacing, GetState()))
			return;
		m_pAnimInstance->StopAllAnimations();
		m_nDirectionFacing = WEST;
		m_pAnimInstance->SetFlip(true);
		m_pAnimInstance->Play(m_nDirectionFacing, m_nState);
		m_pAnimInstance->SetPlayer(IsPlayerUnit());

		return;
	}
	else if(pTileFacing->ptLocalAnchor.y < m_pCurrentTile->ptLocalAnchor.y)
	{
		if(m_nDirectionFacing == NORTH && !m_pAnimInstance->IsFlipped() && m_pAnimInstance->IsPlaying(m_nDirectionFacing, GetState()))
			return;
		m_pAnimInstance->StopAllAnimations();
		m_nDirectionFacing = NORTH;
		m_pAnimInstance->SetFlip(false);
		m_pAnimInstance->Play(m_nDirectionFacing, m_nState);
		m_pAnimInstance->SetPlayer(IsPlayerUnit());

		return;
	}
	else if(pTileFacing->ptLocalAnchor.y > GetLocalRect().top)
	{	
		if(m_nDirectionFacing == SOUTH && !m_pAnimInstance->IsFlipped() && m_pAnimInstance->IsPlaying(m_nDirectionFacing, GetState()))
			return;
		m_pAnimInstance->StopAllAnimations();
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
	CalcAttackBonus();
	if(m_fAttackTimer >= m_fAttackSpeed)
	{
		m_pTarget->SetCurrentHP(m_pTarget->GetHealth() - GetAttackPower()+m_nBonus);
		m_fAttackTimer = 0.f;
	}
	if(m_pTarget->GetHealth() <= 0)
	{
		m_pTarget = NULL;
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
	if(m_pCurrentTile->nType == MOUNTAIN)
		m_nBonus++;
	// Forest enemy defense bonus
	if(GetTarget()->m_pCurrentTile->nType == FOREST)
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

bool CUnit::MoveUnit(float fElapsedTime)
{
	m_fMovementTimer += fElapsedTime;

	POINT ptStart = m_pCurrentTile->ptLocalAnchor;
	POINT ptEnd	  = m_pNextTile->ptLocalAnchor;
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

	if((m_pTarget->m_pCurrentTile->ptTileLoc.x <= m_pCurrentTile->ptTileLoc.x + m_nRange 
		&& m_pTarget->m_pCurrentTile->ptTileLoc.x >= m_pCurrentTile->ptTileLoc.x - m_nRange)
	&& (m_pTarget->m_pCurrentTile->ptTileLoc.y <= m_pCurrentTile->ptTileLoc.y + m_nRange 
		&& m_pTarget->m_pCurrentTile->ptTileLoc.y >= m_pCurrentTile->ptTileLoc.y - m_nRange))
		return true;

	return false;
}

bool CUnit::IsTargetInView()
{

	if((m_pTarget->m_pCurrentTile->ptTileLoc.x <= m_pCurrentTile->ptTileLoc.x + VISIBILITY 
		&& m_pTarget->m_pCurrentTile->ptTileLoc.x >= m_pCurrentTile->ptTileLoc.x - VISIBILITY)
	&& (m_pTarget->m_pCurrentTile->ptTileLoc.y <= m_pCurrentTile->ptTileLoc.y + VISIBILITY 
		&& m_pTarget->m_pCurrentTile->ptTileLoc.y >= m_pCurrentTile->ptTileLoc.y - VISIBILITY))
		return true;

	return false;
}
bool CUnit::IsTileAdjacent(CTile* pTile1, CTile* pTile2)
{

	if((pTile1->ptTileLoc.x		<= pTile2->ptTileLoc.x + 1 
		&& pTile1->ptTileLoc.x	>= pTile2->ptTileLoc.x - 1)
	&& (pTile1->ptTileLoc.y		<= pTile2->ptTileLoc.y + 1 
		&& pTile1->ptTileLoc.y	>= pTile2->ptTileLoc.y - 1))
		return true;

	return false;
}
