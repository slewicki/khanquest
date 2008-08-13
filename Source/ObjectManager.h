////////////////////////////////////////////////
//	File	:	"ObjectManager.h"
//
//	Author	:	Sean Hamstra (SH)
//
//	Purpose	:	To contain and manage game objects.
/////////////////////////////////////////////////
#pragma once
#include "CBase.h"
#include <vector>
#include "CParticleEngine.h"
#include "IListener.h"
#include "CTile.h"
#include "CTileEngine.h"
#include "CAISystem.h"
using std::vector;

class ObjectManager : public IListener
{
private:
	vector<CBase*> m_vObjectList;
	CParticleEngine* pPE;
	int m_nEmitterID;
	CTileEngine* Map;
	CAISystem*		m_pCAI;

	////////////////////////////////////////////
	//	Function:	"ObjectManager(Constructor)"
	////////////////////////////////////////////
	ObjectManager(void);
	ObjectManager(const ObjectManager&);
	ObjectManager& operator=(const ObjectManager&);

	////////////////////////////////////////////
	//	Function:	"~ObjectManager(Destructor)"
	////////////////////////////////////////////
	~ObjectManager(void);

public:
	//////////////////////////////////////////////////////
	// Function: “GetInstance”
	//
	// Purpose: To return an instance of the class
	//////////////////////////////////////////////////////
	static ObjectManager* GetInstance(void);

	//////////////////////////////////////////////////////
	// Function: “UpdateObjects”
	//
	// Purpose: To update all objects
	//////////////////////////////////////////////////////
	void UpdateObjects(float fElapsedTime);

	//////////////////////////////////////////////////////
	// Function: “CheckCollisions”
	//
	// Purpose: Call checkcollisions on all objects
	//////////////////////////////////////////////////////
	void CheckCollisions();

	//////////////////////////////////////////////////////
	// Function: “RenderObjects”
	//
	// Purpose: To render all objects
	//////////////////////////////////////////////////////
	void RenderObjects(float fElapsedTimer);

	//////////////////////////////////////////////////////
	// Function: “AddObject”
	//
	// Purpose: To add an object to the list
	//////////////////////////////////////////////////////
	void AddObject(CBase* pObject);

	//////////////////////////////////////////////////////
	// Function: “RemoveObject”
	//
	// Purpose: To remove an object from the list
	//////////////////////////////////////////////////////
	void RemoveObject(CBase* pObject);
	
	//////////////////////////////////////////////////////
	// Function: “RemoveAllObjects”
	//
	// Purpose: To remove all objects from the list
	//////////////////////////////////////////////////////
	void RemoveAllObjects(void);

	//////////////////////////////////////////////////////
	// Function: “HandleEvent”
	//
	// Purpose: Handle event.
	//////////////////////////////////////////////////////
	void EventHandler(CEvent* pEvent);

	//////////////////////////////////////////////////////
	// Function: “UpdatePlayerUnitStartTile”
	//
	// Purpose: give units a start tile.
	//////////////////////////////////////////////////////
	void UpdatePlayerUnitStartTile(void);

	//////////////////////////////////////////////////////
	// Function: “UpdatePlayerUnitDestTile”
	//
	// Purpose: give units a start tile.
	//////////////////////////////////////////////////////
	void UpdatePlayerUnitDestTile(CTile currentTile);

	/////////////////////////////////////////////////////
	// Function: "SetSeletedUnit"
	//
	// Purpose: Enable the units to show that they are selected.
	/////////////////////////////////////////////////////
	void SetSelectedUnit(RECT toCheck);

	/////////////////////////////////////////////////////
	// Function: "GetSeletedUnit"
	//
	// Purpose: Get The selected Units.
	/////////////////////////////////////////////////////
	vector<CBase*> GetSelectedUnits();

	
	/////////////////////////////////////////////////////
	// Function: "MoveSeletedUnit"
	//
	// Purpose: Move the selected Units in the direction of the mouse click.
	/////////////////////////////////////////////////////
	void MoveSelectedUnits(POINT pMousePos);

	CBase* GetSelectedUnit(unsigned int nIndex) 
	{ 
		if(nIndex >= 0 && nIndex <= m_vObjectList.size())
			return m_vObjectList[nIndex];
		else
			return 0;
	}
	//
	// Purpose: Gets units from the object manager
	/////////////////////////////////////////////////////
	vector<CBase*> GetUnits();

};