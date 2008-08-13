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

ObjectManager::ObjectManager(void)
{
	pPE = CParticleEngine::GetInstance();
	m_nEmitterID = pPE->LoadBineryEmitter("Resource/KQ_DustCload.dat", 128, 128);
	Map = CTileEngine::GetInstance();
	m_pCAI = CAISystem::GetInstance();
	ftime = GetTickCount();
}

ObjectManager::~ObjectManager(void)
{
}
void ObjectManager::CheckCollisions()
{
	for (unsigned int i=0; i < m_vObjectList.size(); i++)
	{
		for(unsigned int j=0; j < m_vObjectList.size(); j++)
			// Don't check it against itself
			if(m_vObjectList[i]!=m_vObjectList[j])
 				if (m_vObjectList[i]->CheckCollisions(m_vObjectList[j]))
				break;
	}
}

ObjectManager* ObjectManager::GetInstance(void)
{
	static ObjectManager instance;	// Lazy instantiation
									// Since it exists for entire game
	return &instance;
}

void ObjectManager::UpdateObjects(float fElapsedTime)
{
	
	for (int i = 0; i < m_vObjectList.size(); ++i)
	{
		m_vObjectList[i]->Update(fElapsedTime);
		if ( GetTickCount() - ftime > 1000)
		{
			CheckCollisions();
			ftime = GetTickCount();
		}
	}
	pPE->Update(fElapsedTime);
}

void ObjectManager::RenderObjects(float fElapsedTime)
{
	for (unsigned int i=0; i < m_vObjectList.size(); i++)
	{
		m_vObjectList[i]->Render(fElapsedTime);
	}
	pPE->Render(fElapsedTime);

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
		Frame* pFrame = (Frame*)pEvent->GetParam();
		for (unsigned int i = 0; i < m_vObjectList.size(); i++)
		{
			if(m_vObjectList[i]->GetType() == UNIT_CAVALRY)
			{
				pPE->SetPostion((int)m_vObjectList[i]->GetPosX() - pFrame->ptAnchorX + pFrame->ptAccessories[0].x, (int)m_vObjectList[i]->GetPosY() - pFrame->ptAnchorY + pFrame->ptAccessories[0].y, m_nEmitterID);
				pPE->SetIsRunning(m_nEmitterID, true);
				return;
			}
		}
		

	}
}
void ObjectManager::UpdatePlayerUnitStartTile(void)
{
	for (int i = 0; i < Map->GetMapWidth(); ++i)
	{
		for (int j = 0; j < Map->GetMapWidth(); ++j)
		{
			if (Map->GetTile(0, i, j).bIsEnemySpawn)
			{
				CFactory::CreateComputerUnit(rand()%6);
			}
		}
	}

	// go through
	for (unsigned int i=0; i < m_vObjectList.size(); i++)
	{
		// check if object is a player unit
		if (static_cast<CUnit*>(m_vObjectList[i])->IsPlayerUnit() )
		{
			// check to see if it has a location already
			if (static_cast<CUnit*>(m_vObjectList[i])->GetPosX() <= 0 && static_cast<CUnit*>(m_vObjectList[i])->GetPosY() <= 0)
			{
				//check the map for spawn point
				for (int j =0; j < Map->GetMapWidth(); ++j )
				{
					for (int k =0; k < Map->GetMapHeight(); ++k )
					{
						// if map loc is a spawn point set unit current loc there
						if (Map->GetTile(0,j,k).bIsPlayerSpawn && Map->GetTile(0,j,k).bIsOccupied == false)
						{
							//POINT spawn = Map->GetLocalAnchor( j, k);
							static_cast<CUnit*>(m_vObjectList[i])->SetPosX(Map->GetLocalAnchor(0, j, k).x);
							static_cast<CUnit*>(m_vObjectList[i])->SetPosY(Map->GetLocalAnchor(0, j, k).y);
							Map->SetOccupy(j, k, true, i);
							static_cast<CUnit*>(m_vObjectList[i])->SetCurrentTile(Map->GetTile(0, j,k));
							static_cast<CUnit*>(m_vObjectList[i])->SetDestTile(Map->GetTile(0, j,k));
							break;
						}

					}

				}
			}
		}
		else
		{
			if (static_cast<CUnit*>(m_vObjectList[i])->GetPosX() <= 0 && static_cast<CUnit*>(m_vObjectList[i])->GetPosY() <= 0)
			{
				//check the map for spawn point
				for (int j =0; j < Map->GetMapWidth(); ++j )
				{
					for (int k =0; k < Map->GetMapHeight(); ++k )
					{
						// if map loc is a spawn point set unit current loc there
						if (Map->GetTile(0,j,k).bIsEnemySpawn && Map->GetTile(0,j,k).bIsOccupied == false)
						{
							//POINT spawn = Map->GetLocalAnchor( j, k);
							static_cast<CUnit*>(m_vObjectList[i])->SetPosX(Map->GetLocalAnchor(0, j, k).x);
							static_cast<CUnit*>(m_vObjectList[i])->SetPosY(Map->GetLocalAnchor(0, j, k).y);
							Map->SetOccupy(j, k, true, i);
							static_cast<CUnit*>(m_vObjectList[i])->SetCurrentTile(Map->GetTile(0, j,k));
							static_cast<CUnit*>(m_vObjectList[i])->SetDestTile(Map->GetTile(0, j,k));
							break;
						}

					}

				}
			}
		}
	}
}
void ObjectManager::UpdatePlayerUnitDestTile(CTile destTile)
{
	for (unsigned int i=0; i < m_vObjectList.size(); i++)
	{
		if (static_cast<CUnit*>(m_vObjectList[i])->IsPlayerUnit() )
		{
			if (static_cast<CUnit*>(m_vObjectList[i])->IsSelected())
			{
				static_cast<CUnit*>(m_vObjectList[i])->SetDestTile(destTile);
				static_cast<CUnit*>(m_vObjectList[i])->SetState(MOVEMENT);
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

		if(IntersectRect(&rIntersect, &static_cast<CUnit*>(m_vObjectList[i])->GetLocalRect(), &toCheck))
		{
			if(static_cast<CUnit*>(m_vObjectList[i])->IsAlive() && static_cast<CUnit*>(m_vObjectList[i])->IsSelected() == false)
			{
				static_cast<CUnit*>(m_vObjectList[i])->SetSelected(true);
				++nSelectedAmount;
			}

		}
		else
		{
			static_cast<CUnit*>(m_vObjectList[i])->SetSelected(false);
		}
		if(nSelectedAmount > 8)
		{
			for(unsigned int j = i; j < m_vObjectList.size(); j++)
				static_cast<CUnit*>(m_vObjectList[j])->SetSelected(false);
			break;
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
		if(static_cast<CUnit*>(m_vObjectList[i])->IsSelected())
		{
			static_cast<CUnit*>(m_vObjectList[i])->ChangeDirection(pMousePos);
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

