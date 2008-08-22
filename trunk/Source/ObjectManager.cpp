////////////////////////////////////////////////
//	File	:	"ObjectManager.cpp"
//
//	Author	:	Sean Hamstra (SH)
//
//	Purpose	:	To contain and manage all of our game objects.
/////////////////////////////////////////////////
#include "ObjectManager.h"
#include "Sheet.h"
#include "CUnit.h"
#include "CGamePlayState.h"
#include "CEventSystem.h"
#include "HUDState.h"
#include "CFactory.h"
#include "CGame.h"
#include "WinBattleState.h"
#include "LoseBattleState.h"
#include "MainMenuState.h"
#include "CTileEngine.h"
#include <algorithm>

ObjectManager::ObjectManager(void)
{
	m_pPE = CParticleEngine::GetInstance();
	m_fPartTimer = 0.0f;
	m_nBloodEmiiter = -1;
	m_pCamera = CCamera::GetInstance();
	Map = CTileEngine::GetInstance();
	m_pCAI = CAISystem::GetInstance();
}

ObjectManager::~ObjectManager(void)
{
}


ObjectManager* ObjectManager::GetInstance(void)
{
	static ObjectManager instance;	// Lazy instantiation
									// Since it exists for entire game
	return &instance;
}

void ObjectManager::UpdateObjects(float fElapsedTime)
{
	m_fTimer += fElapsedTime;
	int nPlayerUnits = 0;
	int nEnemyUnits = 0;
	std::sort(m_vObjectList.begin(), m_vObjectList.end(),  SortObjects());

		for (unsigned int i = 0; i < m_vObjectList.size(); ++i)
		{
			if(static_cast<CUnit*>(m_vObjectList[i])->IsActive())
			{
				// if they aren't on screen, update every other second
				if(!((CUnit*)(m_vObjectList[i]))->IsOnScreen() && m_fTimer<.4f )
				{
					m_vObjectList[i]->Update(fElapsedTime);
				}
				else if(((CUnit*)(m_vObjectList[i]))->IsOnScreen())
				{
					m_vObjectList[i]->Update(fElapsedTime);
				}
				if(static_cast<CUnit*>(m_vObjectList[i])->IsPlayerUnit())
					++nPlayerUnits;
				else
					++nEnemyUnits;
			}

		}
	
	m_pPE->Update(fElapsedTime);

    if(nEnemyUnits <=0)
	{
		if(CGame::GetInstance()->GetTutorialMode())
		{
			CGame::GetInstance()->ChangeState(CMainMenuState::GetInstance());
			return;
		}
		CGame::GetInstance()->PopCurrentState();
		CGame::GetInstance()->PushState(CWinBattleState::GetInstance());
		CGame::GetInstance()->AddWins();

	}
	if(nPlayerUnits <= 0)
	{
		if(CGame::GetInstance()->GetTutorialMode())
		{
			CGame::GetInstance()->ChangeState(CMainMenuState::GetInstance());
			return;
		}
		CGame::GetInstance()->PopCurrentState();
		CGame::GetInstance()->PushState(CLoseBattleState::GetInstance());
		CGame::GetInstance()->AddLoses();

	}
	m_pPE->Update(fElapsedTime);
	if(m_fTimer > .8f)
		m_fTimer = 0.f;
}

void ObjectManager::RenderObjects(float fElapsedTime)
{
	for (unsigned int i=0; i < m_vObjectList.size(); i++)
	{
		m_vObjectList[i]->Render(fElapsedTime);
	}
	m_pPE->Render(fElapsedTime);
}

void ObjectManager::AddObject(CBase* pObject)
{
	//	Check for valid pointer
	if (pObject == NULL)
		return;

	//	Add object to object list
	m_vObjectList.push_back(pObject);

	//	Increase its ref count
	pObject->AddRef();
}

void ObjectManager::RemoveObject(CBase* pObject)
{
	//	Check for valid pointer
	if (pObject == NULL)
		return;

	for (vector<CBase*>::iterator iter = m_vObjectList.begin();
		 iter != m_vObjectList.end();
		 iter++)
	{
		// Is this the object we are looking for
		if ((*iter) == pObject)
		{
			// Decrement ref count
			(*iter)->Release();
			(*iter) = NULL;
			// Remove the object from the list
			iter = m_vObjectList.erase(iter);
			break;
		}
	}

}

void ObjectManager::RemoveAllObjects(void)
{
	//	Call Release() on all objects.
	for (unsigned int i=0; i < m_vObjectList.size(); i++)
	{
		m_vObjectList[i]->Release();
		m_vObjectList[i] = NULL;
	}

	//	Clear the vector
	m_vObjectList.clear();
}
void ObjectManager::EventHandler(CEvent* pEvent)
{
	if(pEvent->GetEventID() == "Play")
	{
		//Frame* pFrame = (Frame*)pEvent->GetParam();
		//for (unsigned int i = 0; i < m_vObjectList.size(); i++)
		//{
		//	if(m_vObjectList[i]->GetType() == UNIT_CAVALRY)
		//	{
		//		//pPE->SetPostion((int)m_vObjectList[i]->GetPosX() - pFrame->ptAnchorX + pFrame->ptAccessories[0].x, (int)m_vObjectList[i]->GetPosY() - pFrame->ptAnchorY + pFrame->ptAccessories[0].y);
		//		//pPE->SetIsRunning(true);
		//		return;
		//	}
		//}
	}

	if(pEvent->GetEventID() == "Attack_Sound")
	{
		CUnit* pUnit = (CUnit*)pEvent->GetParam();
		//if(!CSGD_WaveManager::GetInstance()->IsWavePlaying(pUnit->GetAttackSoundID()))
		//{
			CSGD_WaveManager::GetInstance()->SetVolume(pUnit->GetAttackSoundID(),CGame::GetInstance()->GetSFXVolume()+20);
			CSGD_WaveManager::GetInstance()->Play(pUnit->GetAttackSoundID());
		//}
			
		
	}
	if(pEvent->GetEventID() == "Dying_Sound")
	{
		CUnit* pUnit = (CUnit*)pEvent->GetParam();
		
			if(pUnit->IsOnScreen())
			{
				CSGD_WaveManager::GetInstance()->SetVolume(pUnit->GetDeathSoundID(),CGame::GetInstance()->GetSFXVolume());
				CSGD_WaveManager::GetInstance()->Play(pUnit->GetDeathSoundID());
				
			}
		
	}
	
	/*if(pEvent->GetEventID() == "Remove")
	{
		for(unsigned int i = 0; i < m_vObjectList.size(); i++)
			if(static_cast<CUnit*>(m_vObjectList[i])->IsAlive() == false)
			{
				RemoveObject(m_vObjectList[i]);
				break;
			}
	}*/
}

void ObjectManager::UpdatePlayerUnitStartTile(void)
{
	int nInfantry = 0;
	int nArcher = 0;
	int nCavalry = 0;
	int nCavalryArcher = 0;
	int nWarElephant = 0;
	int nAxmen = 0;
	switch(CGame::GetInstance()->GetSelectedCity()->GetID())
	{
	case KCITY1:
		nCavalry = 2;
		nWarElephant = 2;
		break;
	case KCITY2:
		nCavalry = 3;
		nWarElephant = 4;
		nCavalryArcher = 2;
		break;
	case KCITY3:
		nCavalry = 4;
		nWarElephant = 6;
		nCavalryArcher = 3;
		break;
	case XCITY1:
		nInfantry = 6;
		nAxmen = 6;
		break;
	case XCITY2:
		nInfantry = 10;
		nAxmen = 12;
		nCavalry = 2;
		break;
	case XCITY3:
		nInfantry = 15;
		nAxmen = 16;
		nCavalry = 5;
		break;
	case JCITY1:
		nCavalry = 0;
		nInfantry = 4;
		nCavalryArcher = 3;
		nArcher = 3;
		break;
	case JCITY2:
		nCavalry = 3;
		nInfantry = 6;
		nCavalryArcher = 4;
		nArcher = 6;
		break;
	case JCITY3:
		nCavalry = 4;
		nInfantry = 12;
		nCavalryArcher = 4;
		nArcher = 18;
		break;
	default:
		// Bad news bears.
		
		break;
	}
	
	for (int i = 0; i < nInfantry; i++)
		CFactory::CreateComputerUnit(UNIT_INFANTRY);

	for (int i = 0; i < nCavalry; i++)
		CFactory::CreateComputerUnit(UNIT_CAVALRY);

	for (int i = 0; i < nCavalryArcher; i++)
		CFactory::CreateComputerUnit(UNIT_CAVALRY_ARCHER);

	for (int i = 0; i < nAxmen; i++)
		CFactory::CreateComputerUnit(UNIT_AXMEN);

	for (int i = 0; i < nArcher; i++)
		CFactory::CreateComputerUnit(UNIT_ARCHER);

	for (int i = 0; i < nWarElephant; i++)
		CFactory::CreateComputerUnit(UNIT_WAR_ELEPHANT);


	for (unsigned int i = 0; i < m_vObjectList.size(); i++)
	{
		
		// Go thru map until end or until the unit has a tile
		for (int k = 0; k < Map->GetMapWidth() && (((CUnit*)m_vObjectList[i])->GetCurrentTile() == NULL); ++k)
		{
			// Go thru map until end or until the unit has a tile
			for (int j = 0; j < Map->GetMapHeight() && (((CUnit*)m_vObjectList[i])->GetCurrentTile() == NULL); ++j)
			{
				// If enemy spawn point, and unoccupied and is enemy, place it here
				if (Map->GetTile(0, k, j)->bIsEnemySpawn && !((CUnit*)m_vObjectList[i])->IsPlayerUnit() && !Map->GetTile(0, k, j)->bIsOccupied)
				{
					((CUnit*)m_vObjectList[i])->SetCurrentTile(Map->GetTile(0, k, j));
					Map->GetTile(0, k, j)->bIsOccupied = true;
					Map->GetTile(0, k, j)->pUnit = ((CUnit*)m_vObjectList[i]);
				}
				// If player spawn point and unoccupied and player unit, place here
				else if(Map->GetTile(0, k, j)->bIsPlayerSpawn && ((CUnit*)m_vObjectList[i])->IsPlayerUnit() && !Map->GetTile(0, k, j)->bIsOccupied)
				{
					((CUnit*)m_vObjectList[i])->SetCurrentTile(Map->GetTile(0, k, j));
					Map->GetTile(0, k, j)->bIsOccupied = true;
					Map->GetTile(0, k, j)->pUnit = ((CUnit*)m_vObjectList[i]);
				}
			}
		}
		// If we went thru the whole map and didn't find a spawn point, remove the unit
		if(!((CUnit*)m_vObjectList[i])->GetCurrentTile())
		{
			this->RemoveObject(((CUnit*)m_vObjectList[i]));
		}
		
	}
	
}
void ObjectManager::UpdatePlayerUnitDestTile(CTile* destTile)
{
	if(!destTile || destTile->bIsCollision)
		return;
	for (unsigned int i=0; i < m_vObjectList.size(); i++)
	{
		if (static_cast<CUnit*>(m_vObjectList[i])->IsPlayerUnit() )
		{
			if (static_cast<CUnit*>(m_vObjectList[i])->IsSelected())
			{
				// If its occupied
				if(destTile->bIsOccupied)
				{
					// and an enemy
					if(destTile->pUnit && !destTile->pUnit->IsPlayerUnit())
						// then thats the new target
						static_cast<CUnit*>(m_vObjectList[i])->SetTarget(destTile->pUnit);
					
				}
				else
				{
					// otherwise, thats where we're going
					static_cast<CUnit*>(m_vObjectList[i])->SetTarget(NULL);
					static_cast<CUnit*>(m_vObjectList[i])->SetDestTile(destTile);
					static_cast<CUnit*>(m_vObjectList[i])->SetNextTile(NULL);
					static_cast<CUnit*>(m_vObjectList[i])->SetState(MOVEMENT);
	
				}
			}
		}
	}
}
void ObjectManager::SetSelectedUnit(RECT toCheck)
{
	RECT rIntersect;
	int nSelectedAmount = 0;
	for(unsigned int i = 0; i < m_vObjectList.size(); ++i)
	{
		static_cast<CUnit*>(m_vObjectList[i])->SetSelected(false);
		if(IntersectRect(&rIntersect, &static_cast<CUnit*>(m_vObjectList[i])->GetLocalRect(), &toCheck))
		{
			if(nSelectedAmount == 0 && static_cast<CUnit*>(m_vObjectList[i])->IsAlive() && static_cast<CUnit*>(m_vObjectList[i])->IsSelected() == false && !static_cast<CUnit*>(m_vObjectList[i])->IsPlayerUnit())
			{	
				static_cast<CUnit*>(m_vObjectList[i])->SetSelected(true);
				nSelectedAmount = 8;
			}
			else if(static_cast<CUnit*>(m_vObjectList[i])->IsAlive() && static_cast<CUnit*>(m_vObjectList[i])->IsSelected() == false && static_cast<CUnit*>(m_vObjectList[i])->IsPlayerUnit())
			{
				if(nSelectedAmount <= 7)
				{
					static_cast<CUnit*>(m_vObjectList[i])->SetSelected(true);
					++nSelectedAmount;

				}
				
			}
			
		}
		
	}
	CHUDState::GetInstance()->UpdateSelected();
}

vector<CBase*> ObjectManager::GetSelectedUnits()
{
	vector<CBase*> vSelectedUnits;
	for(unsigned int i = 0; i < m_vObjectList.size(); ++i)
	{
		if(static_cast<CUnit*>(m_vObjectList[i])->IsSelected())
		{
			vSelectedUnits.push_back(m_vObjectList[i]);
		}
	}	
	return vSelectedUnits;
}
void ObjectManager::MoveSelectedUnits(POINT pMousePos)
{
	for(unsigned int i = 0; i < m_vObjectList.size(); ++i)
	{
		if(static_cast<CUnit*>(m_vObjectList[i])->IsSelected() && static_cast<CUnit*>(m_vObjectList[i])->IsPlayerUnit())
		{
			static_cast<CUnit*>(m_vObjectList[i])->ChangeDirection(CTileEngine::GetInstance()->GetTile(0,CTileEngine::GetInstance()->IsoMouse(pMousePos.x, pMousePos.y, 0).x, CTileEngine::GetInstance()->IsoMouse(pMousePos.x, pMousePos.y, 0).y));
			static_cast<CUnit*>(m_vObjectList[i])->ClearPath();
			static_cast<CUnit*>(m_vObjectList[i])->SetPath(m_pCAI->FindPath(static_cast<CUnit*>(m_vObjectList[i])->GetCurrentTile(), static_cast<CUnit*>(m_vObjectList[i])->GetDestTile()));
		}
	}	
}

vector<CBase*> ObjectManager::GetUnits()
{
	vector<CBase*> vSelectedUnits;
	for(unsigned int i = 0; i < m_vObjectList.size(); ++i)
	{
		vSelectedUnits.push_back(static_cast<CUnit*>(m_vObjectList[i]));
	}	
	return vSelectedUnits;
}

void ObjectManager::GetSpawnPointDest(CUnit* pUnit)
{
	// go through
	for (unsigned int i = 0; i < m_vObjectList.size(); i++)
	{
		if ( ((CUnit*)m_vObjectList[i]) == pUnit )
		{
			// Go thru map until end or until the unit has a dest tile
			for (int k = 0; k < Map->GetMapWidth() && (((CUnit*)m_vObjectList[i])->GetDestTile() == NULL); ++k)
			{
				// Go thru map until end or until the unit has a dest tile
				for (int j = 0; j < Map->GetMapWidth() && (((CUnit*)m_vObjectList[i])->GetDestTile() == NULL); ++j)
				{
					// If enemy spawn point, and unoccupied and is enemy, set its dest
					if (Map->GetTile(0, k, j)->bIsEnemySpawn && !((CUnit*)m_vObjectList[i])->IsPlayerUnit() && !Map->GetTile(0, k, j)->bIsOccupied)
					{
						((CUnit*)m_vObjectList[i])->SetDestTile(Map->GetTile(0, k, j));
					}
					// If player spawn point and unoccupied and player unit, place here
					else if(Map->GetTile(0, k, j)->bIsPlayerSpawn && ((CUnit*)m_vObjectList[i])->IsPlayerUnit() && !Map->GetTile(0, k, j)->bIsOccupied)
					{
						((CUnit*)m_vObjectList[i])->SetDestTile(Map->GetTile(0, k, j));
					}
				}
			}
		}
	}
}
void ObjectManager::SetSelectedUnitsRetreat()
{
	for (unsigned int i = 0; i < m_vObjectList.size(); i++)
	{
		if ( ((CUnit*)m_vObjectList[i])->IsPlayerUnit() && ((CUnit*)m_vObjectList[i])->IsSelected() )
		{
			((CUnit*)m_vObjectList[i])->SetState(RETREAT);
		}
	}
}