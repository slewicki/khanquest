//////////////////////////////////////////////////////////
//	File:	"CFactory.h"
//
//	Author:	Sean Hamstra (SH)
//
//	Purpose: To create all of our game objects
//////////////////////////////////////////////////////////

#include "CFactory.h"
#include "CGame.h"


CFactory::CFactory(void)
{
}
CFactory::~CFactory(void)
{
}

void CFactory::CreatePlayerUnit(int nType)
{
	CUnit* unit = new CUnit();

	// Use default shallow copy since no dynamic info in creation
	*(unit) = CGame::GetInstance()->GetPlayerUnitInfo(nType);
	
	// Events
	
	// Add to manager
	ObjectManager::GetInstance()->AddObject(unit);

	// Let it know we aren't hanging on to it
	unit->Release();


}

void CFactory::CreateComputerUnit(int nType)
{
	CUnit* unit = new CUnit();
	*(unit) = CGame::GetInstance()->GetCPUUnitInfo(nType);
	
	// Events
	
	// Add to manager
	ObjectManager::GetInstance()->AddObject(unit);

	// Let it know we aren't hanging on to it
	unit->Release();


}