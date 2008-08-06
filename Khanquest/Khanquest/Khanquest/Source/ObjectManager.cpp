////////////////////////////////////////////////
//	File	:	"ObjectManager.cpp"
//
//	Author	:	Sean Hamstra (SH)
//
//	Purpose	:	To contain and manage all of our game objects.
/////////////////////////////////////////////////
#include "ObjectManager.h"
#include "Sheet.h"
#include "CGamePlayState.h"
ObjectManager::ObjectManager(void)
{
	pPE = CParticleEngine::GetInstance();
	pPE->LoadBineryEmitter("Resource/KQ_DustCload.dat", 128, 128);
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
 				m_vObjectList[i]->CheckCollisions(m_vObjectList[j]);
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
	vector<CBase*>::iterator iter = m_vObjectList.begin();

	while(iter != m_vObjectList.end())
	{
		(*iter)->Update(fElapsedTime);
		iter++;
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
				//pPE->SetPostion((int)m_vObjectList[i]->GetPosX() - pFrame->ptAnchorX + pFrame->ptAccessories[0].x, (int)m_vObjectList[i]->GetPosY() - pFrame->ptAnchorY + pFrame->ptAccessories[0].y);
				//pPE->SetIsRunning(true);
				return;
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
			static_cast<CUnit*>(m_vObjectList[i])->SetSelected(true);
			++nSelectedAmount;
		}
		else
			static_cast<CUnit*>(m_vObjectList[i])->SetSelected(false);
		
		if(nSelectedAmount > 8)
		{
			for(unsigned int j = i; j < m_vObjectList.size(); j++)
				static_cast<CUnit*>(m_vObjectList[j])->SetSelected(false);
			
			break;
		}

	}
}
void ObjectManager::GetSelectedUnit()
{
	for(unsigned int i = 0; i < m_vObjectList.size(); ++i)
	{
		if(static_cast<CUnit*>(m_vObjectList[i])->IsSelected())
			static_cast<CUnit*>(m_vObjectList[i])->DamageUnit();
	}
}