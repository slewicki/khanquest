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
	static float xPos = 20;
	static float yPos = 20;
	CUnit* unit = new CUnit(nType);

	// Use default shallow copy since no dynamic info in creation
	*(unit) = CGame::GetInstance()->GetPlayerUnitInfo(nType);

	// For now give it a default position
	unit->SetPosX(xPos);
	unit->SetPosY(yPos);
	// Row by Row
	if(xPos >= 720.f)
	{
		xPos = 20.f;
		yPos += 100.f;
	}
	xPos+=100.f;
	
	//unit->SetDirection(SOUTH_WEST);
	//unit->SetState(MOVEMENT);
	// Register Events


	// Add to manager
	ObjectManager::GetInstance()->AddObject(unit);

	// Let it know we aren't hanging on to it
	unit->Release();

}

void CFactory::CreateComputerUnit(int nType)
{
	CUnit* unit = new CUnit(nType);
	*(unit) = CGame::GetInstance()->GetCPUUnitInfo(nType);
	
	// Register Events
	
	// Add to manager
	ObjectManager::GetInstance()->AddObject(unit);

	// Let it know we aren't hanging on to it
	unit->Release();

}